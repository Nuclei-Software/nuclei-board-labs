#include <stdio.h>
#include <stdint.h>
#include "riscv_mmu.h"

#include "nuclei_sdk_soc.h"

#define KiB(x) ((uint64_t)(x) << 10l)
#define MiB(x) (KiB(x) << 10l)
#define GiB(x) (MiB(x) << 10l)

typedef enum
{
    PRIVILEGE_USER_MODE = 0,
    PRIVILEGE_SUPERVISOR_MODE = 1,
    PRIVILEGE_HYPERVISOR_MODE = 2,
    PRIVILEGE_MACHINE_MODE = 3,
} riscv_privilege_mode;

typedef void (*riscv_privilege_entry_t)(void);

//extern char _stack_top[];
uintptr_t _stack_top = 0xA2200000;

//extern char _text_start[];

uintptr_t _text_start = 0xA0000000;


//extern char _data_start[];
uintptr_t _data_start = 0xA2000000;

static void riscv_privilege_set_pmp(void)
{
    // Set PMP, S mode can access all address range
    __set_PMPxCFG(0, 0x1F);          // NAPOT、R/W/X
    __set_PMPADDRx(0, 0x3FFFFFFFFF); // 40bit physic address
}

static void riscv_privilege_switch_mode(riscv_privilege_entry_t mode_entry)
{
    uintptr_t mode_sp = (uintptr_t)_stack_top;
    __switch_mode(PRIVILEGE_SUPERVISOR_MODE, mode_sp, mode_entry);
}

void main_supervisor(void)
{
    // MMU.
    pgd_t *pgd_base = mmu_get_pdg_address();
    struct mmu_region region = {0};

    // TEXT & RODATA
    region.name = "mem_code";
    region.base_va = (uintptr_t)_text_start;
    region.base_pa = (uintptr_t)_text_start;;
    region.size = MiB(4);
    region.attrs.pgprot = _PAGE_READ | _PAGE_EXEC;
    create_pgd_mapping(pgd_base, region.base_va, region.base_pa, region.size, region.attrs);

    // DATA & BSS & STACK
    region.name = "mem_data";
    region.base_va = (uintptr_t)_data_start;
    region.base_pa = (uintptr_t)_data_start;;
    region.size = MiB(2);
    region.attrs.pgprot = _PAGE_READ | _PAGE_WRITE;
    create_pgd_mapping(pgd_base, region.base_va, region.base_pa, region.size, region.attrs);

    // Device
    region.name = "mem_device";
    region.base_va = (uintptr_t)0x10000000;
    region.base_pa = (uintptr_t)0x10000000;
    region.size = MiB(2);
    region.attrs.pgprot = _PAGE_READ | _PAGE_WRITE;
    create_pgd_mapping(pgd_base, region.base_va, region.base_pa, region.size, region.attrs);

    mmu_enable();

    printf("test after mmu!!\n");

    while (1);
}

void main(void)
{
    riscv_privilege_set_pmp();
    riscv_privilege_switch_mode(main_supervisor);

    while (1);
}
