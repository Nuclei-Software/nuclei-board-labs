#include <stdint.h>
#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "riscv_mmu.h"

#define KiB(x) ((uint64_t)(x) << 10l)
#define MiB(x) (KiB(x) << 10l)
#define GiB(x) (MiB(x) << 10l)

typedef enum {
    PRIVILEGE_USER_MODE = 0,
    PRIVILEGE_SUPERVISOR_MODE = 1,
    PRIVILEGE_HYPERVISOR_MODE = 2,
    PRIVILEGE_MACHINE_MODE = 3,
} riscv_privilege_mode;

typedef void (*riscv_privilege_entry_t)(void);

// NOTE: Match with DOWNLOAD=ddr mode linker script file

// extern char _stack_top[];
uintptr_t _stack_top = 0xA2200000;

// extern char _text_start[];

uintptr_t _text_start = 0xA0000000;

// extern char _data_start[];
uintptr_t _data_start = 0xA2000000;

typedef struct {
    uint32_t state;
} spinlock;

volatile spinlock lock;

volatile uint32_t lock_ready = 0;
volatile uint32_t cpu_count = 0;
volatile uint32_t finished = 0;

volatile uint32_t pt_done = 0;

// comment SPINLOCK_CAS to use AMOSWAP as spinlock
#define SPINLOCK_CAS

__STATIC_FORCEINLINE void spinlock_init(volatile spinlock *lock)
{
    lock->state = 0;
}

__STATIC_FORCEINLINE void spinlock_lock(volatile spinlock *lock)
{
#if defined(__riscv_atomic)
    uint32_t old;
    uint32_t backoff = 10;
    do {
#ifndef SPINLOCK_CAS
        // Use amoswap as spinlock
        old = __AMOSWAP_W((&(lock->state)), 1);
#else
        // use lr.w & sc.w to do CAS as spinlock
        old = __CAS_W((&(lock->state)), 0, 1);
#endif
        if (old == 0) {
            break;
        }
        for (volatile int i = 0; i < backoff; i++) {
            __NOP();
        }
        backoff += 10;
    } while (1);
#else
    lock->state = 1;
#endif
}

__STATIC_FORCEINLINE void spinlock_unlock(volatile spinlock *lock) { lock->state = 0; }

static void riscv_privilege_set_pmp(void)
{
    // Set PMP, S mode can access all address range
    __set_PMPxCFG(0, 0x1F);          // NAPOT, R/W/X
    __set_PMPADDRx(0, 0x3FFFFFFFFF); // 40bit physical address
}

static void riscv_privilege_switch_mode(unsigned long hartid,
                                        riscv_privilege_entry_t mode_entry)
{
    extern int __STACK_SIZE;
    uintptr_t mode_sp = (uintptr_t)((unsigned long)_stack_top -
                                    hartid * ((unsigned long)(&__STACK_SIZE)));
    __switch_mode(PRIVILEGE_SUPERVISOR_MODE, mode_sp, mode_entry);
}

void main_supervisor(void)
{
    spinlock_lock(&lock);
    printf("Main Supervisor Entry !!!\n");
    spinlock_unlock(&lock);

    // MMU.
    pgd_t *pgd_base = mmu_get_pdg_address();
    struct mmu_region region = {0};

    // TEXT & RODATA
    region.name = "mem_code";
    region.base_va = (uintptr_t)_text_start;
    region.base_pa = (uintptr_t)_text_start;
    ;
    region.size = MiB(4);
    region.attrs.pgprot = _PAGE_READ | _PAGE_EXEC;
    create_pgd_mapping(pgd_base, region.base_va, region.base_pa, region.size,
                       region.attrs);

    // DATA & BSS & STACK
    region.name = "mem_data";
    region.base_va = (uintptr_t)_data_start;
    region.base_pa = (uintptr_t)_data_start;
    ;
    region.size = MiB(2);
    region.attrs.pgprot = _PAGE_READ | _PAGE_WRITE;
    create_pgd_mapping(pgd_base, region.base_va, region.base_pa, region.size,
                       region.attrs);

    // Device
    region.name = "mem_device";
    region.base_va = (uintptr_t)0x10000000;
    region.base_pa = (uintptr_t)0x10000000;
    region.size = MiB(2);
    region.attrs.pgprot = _PAGE_READ | _PAGE_WRITE;
    create_pgd_mapping(pgd_base, region.base_va, region.base_pa, region.size,
                       region.attrs);

    // the page table is created done
    pt_done = 1;

    mmu_enable();

    spinlock_lock(&lock);
    printf("[Main Supervisor] If you can see this message, it proves that the "
           "MMU is working normally !!!\n");
    spinlock_unlock(&lock);

    while (1);
}

void other_main_supervisor(void)
{
    spinlock_lock(&lock);
    printf("Other main Supervisor Entry !!!\n");
    spinlock_unlock(&lock);

    // wait the core 0 create page table done
    while (pt_done == 0);

    mmu_enable();

    spinlock_lock(&lock);
    printf("[Other main Supervisor] If you can see this message, it proves "
           "that the MMU is working normally !!!\n");
    spinlock_unlock(&lock);

    while (1);
}

#if defined(SMP_CPU_CNT)
#if !defined(__riscv_atomic)
#error "RVA(atomic) extension is required for SMP"
#endif
int boot_hart_main(unsigned long hartid);
int other_harts_main(unsigned long hartid);
#endif

int main(void);

/* Reimplementation of smp_main for multi-harts */
int smp_main(void) { return main(); }

int main(void)
{
    int ret = 0;
    unsigned long hartid = __RV_CSR_READ(CSR_MHARTID);

#if defined(SMP_CPU_CNT)
    if (hartid == BOOT_HARTID) { // boot hart
        spinlock_init(&lock);
        lock_ready = 1;
        finished = 0;
        __SMP_RWMB();
        ret = boot_hart_main(hartid);
    } else { // other harts
        // wait for lock initialized
        while (lock_ready == 0);
        ret = other_harts_main(hartid);
    }
#else
    riscv_privilege_set_pmp();
    riscv_privilege_switch_mode(hartid, main_supervisor);
#endif
    return ret;
}

#if defined(SMP_CPU_CNT)
int boot_hart_main(unsigned long hartid)
{
    volatile unsigned long waitcnt = 0;

    spinlock_lock(&lock);
    printf("Hello world from hart %d\n", hartid);
    cpu_count += 1;
    spinlock_unlock(&lock);

    riscv_privilege_set_pmp();
    riscv_privilege_switch_mode(hartid, main_supervisor);

    // wait for all harts boot and print hello
    while (cpu_count < SMP_CPU_CNT) {
        waitcnt++;
        __NOP();
        // The waitcnt compare value need to be adjust according
        // to cpu frequency
        if (waitcnt >= SystemCoreClock) {
            break;
        }
    }
    if (cpu_count == SMP_CPU_CNT) {
        printf("All harts boot successfully!\n");
        finished = 1;
        return 0;
    } else {
        printf("Some harts boot failed, only %d/%d booted!\n", cpu_count,
               SMP_CPU_CNT);
        return -1;
    }
}

int other_harts_main(unsigned long hartid)
{
    spinlock_lock(&lock);
    printf("Hello world from hart %d\n", hartid);
    cpu_count += 1;
    spinlock_unlock(&lock);

    riscv_privilege_set_pmp();
    riscv_privilege_switch_mode(hartid, other_main_supervisor);

    // wait for all harts boot and print hello
    while (cpu_count < SMP_CPU_CNT);
    // wait for boot hart to set finished flag
    while (finished == 0);
    return 0;
}
#endif
