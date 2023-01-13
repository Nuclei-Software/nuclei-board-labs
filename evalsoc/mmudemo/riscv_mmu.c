#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "nuclei_sdk_soc.h"
#include "riscv_mmu.h"

#define PFN_ALIGN(x)    (((unsigned long)(x) + (PAGE_SIZE - 1)) & PAGE_MASK)
#define PFN_UP(x)       (((x) + PAGE_SIZE - 1) >> PAGE_SHIFT)
#define PFN_DOWN(x)     ((x) >> PAGE_SHIFT)
#define PFN_PHYS(x)     ((unsigned long)(x) << PAGE_SHIFT)
#define PHYS_PFN(x)     ((unsigned long)((x) >> PAGE_SHIFT))

#define PAGE_SHIFT      (12)
#define PAGE_SIZE       (1UL << PAGE_SHIFT)
#define PAGE_MASK       (~(PAGE_SIZE - 1))

#define PGDIR_SHIFT     (30)
#define PGDIR_SIZE      (1UL << PGDIR_SHIFT)
#define PGDIR_MASK      (~(PGDIR_SIZE - 1))

#define PMD_SHIFT       (21)
#define PMD_SIZE        (1UL << PMD_SHIFT)
#define PMD_MASK        (~(PMD_SIZE - 1))

#define pte_val(x)      ((x).pte)
#define pmd_val(x)      ((x).pmd)
#define pgd_val(x)      ((x).pgd)
#define pgprot_val(x)   ((x).pgprot)

#define __pte(x)        ((pte_t) {(x)})
#define __pmd(x)        ((pmd_t) {(x)})
#define __pgd(x)        ((pgd_t) {(x)})
#define __pgprot(x)     ((pgprot_t) {(x)})

#define PTRS_PER_PGD    (PAGE_SIZE / sizeof(pgd_t)) /* Number of entries in the page global directory */
#define PTRS_PER_PMD    (PAGE_SIZE / sizeof(pmd_t)) /* Number of entries in the page middle table */
#define PTRS_PER_PTE    (PAGE_SIZE / sizeof(pte_t)) /* Number of entries in the page table */

#define PTE_INDEX(a)    (((a) >> PAGE_SHIFT) & (PTRS_PER_PTE - 1))
#define PMD_INDEX(a)    (((a) >> PMD_SHIFT) & (PTRS_PER_PMD - 1))
#define PGD_INDEX(a)    (((a) >> PGDIR_SHIFT) & (PTRS_PER_PGD - 1))

#define __page_val_to_pfn(_val)  (((_val) & _PAGE_PFN_MASK) >> _PAGE_PFN_SHIFT)

#define __ASSERT(test, fmt, ...)              \
    do {                                      \
        if (!(test)) {                        \
            printf(fmt, ##__VA_ARGS__);       \
        }                                     \
    } while (false)


typedef union
{
    struct
    {
        uint64_t ppn  : 44;
        uint64_t asid : 16;
        uint64_t mode : 4;
    } bit;

    uint64_t all;
} risv64_satp;


/* Number of entries for each table (512) */
#define PAGE_NUM_ENTRIES    ((1UL << PAGE_SHIFT) / sizeof(uint64_t))
#define PAGE_NUM_TOTAL      (16)

struct mmu_table_t
{
    uint64_t table[PAGE_NUM_ENTRIES];
};
static_assert(sizeof(struct mmu_table_t) == 4096, "");

struct multi_table_t
{
    struct mmu_table_t multi[PAGE_NUM_TOTAL];
};

struct multi_table_t kernel_mmu_table __attribute__((aligned(PAGE_NUM_ENTRIES *sizeof(uint64_t))));
static uint64_t mmu_alloc_index = 1;

static inline uintptr_t mmu_alloc_page(void)
{
    if (mmu_alloc_index >= PAGE_NUM_TOTAL)
    {
        return 0;
    }
    else
    {
        return (uintptr_t)(&kernel_mmu_table.multi[mmu_alloc_index++]);
    }
}

static inline uintptr_t pgd_addr_end(uintptr_t start, uintptr_t end)
{
    uintptr_t pgd_boundary = (start + PGDIR_SIZE) & PGDIR_MASK;
    return (end > pgd_boundary) ? pgd_boundary : end;
}

static inline uintptr_t pmd_addr_end(uintptr_t start, uintptr_t end)
{
    uintptr_t pmd_boundary = (start + PMD_SIZE) & PMD_MASK;
    return (end > pmd_boundary) ? pmd_boundary : end;
}

static inline pgd_t pfn_pgd(uint64_t pfn, pgprot_t prot)
{
    uint64_t prot_val = pgprot_val(prot);
    return __pgd((pfn << _PAGE_PFN_SHIFT) | prot_val);
}

static inline pmd_t pfn_pmd(uint64_t pfn, pgprot_t prot)
{
    uint64_t prot_val = pgprot_val(prot);
    return __pmd((pfn << _PAGE_PFN_SHIFT) | prot_val);
}

static inline pte_t pfn_pte(uint64_t pfn, pgprot_t prot)
{
    uint64_t prot_val = pgprot_val(prot);
    return __pte((pfn << _PAGE_PFN_SHIFT) | prot_val);
}

static inline uint64_t _pgd_pfn(pgd_t pgd)
{
    return __page_val_to_pfn(pgd_val(pgd));
}

static inline uint64_t _pmd_pfn(pmd_t pmd)
{
    return __page_val_to_pfn(pmd_val(pmd));
}

static inline uint64_t _pte_pfn(pte_t pte)
{
    return __page_val_to_pfn(pte_val(pte));
}

static inline int pte_none(pte_t pte)
{
    return (pte_val(pte) == 0);
}

static inline int pmd_none(pmd_t pmd)
{
    return (pmd_val(pmd) == 0);
}

static inline int pgd_none(pgd_t pgd)
{
    return (pgd_val(pgd) == 0);
}

static inline pte_t *get_pte_virt(uintptr_t pa)
{
    return (pte_t *)pa;
}

static inline pmd_t *get_pmd_virt(uintptr_t pa)
{
    return (pmd_t *)pa;
}


static void create_pte_mapping(pte_t *ptep, uintptr_t va, uintptr_t pa, uint64_t sz, pgprot_t prot)
{
    /* check minimum alignment requirement for given mmap region */
    __ASSERT(((va | pa | sz) & (PAGE_SIZE - 1)) == 0, "address/size are not page aligned\n");
    __ASSERT(ptep != NULL, "PTE base address is NULL\n");

    uint64_t pte_idx = 0;

    uintptr_t va_temp = va;
    uintptr_t pa_temp = pa;
    uint64_t sz_temp = sz;

    while (1)
    {
        if (sz_temp > 0)
        {
            pte_idx = PTE_INDEX(va_temp);

            if (pte_none(ptep[pte_idx]))
            {
                ptep[pte_idx] = pfn_pte(PFN_DOWN(pa_temp), __pgprot(pgprot_val(prot) | _PAGE_BASE | _PAGE_TABLE));
            }
            else
            {
                __ASSERT(0, "pte size or address is overlap\n");
            }

            va_temp += PAGE_SIZE;
            pa_temp += PAGE_SIZE;
            sz_temp -= PAGE_SIZE;
        }
        else
        {
            break;
        }
    }
}

static void create_pmd_mapping(pmd_t *pmdp, uintptr_t va, uintptr_t pa, uint64_t sz, pgprot_t prot)
{
    pte_t *ptep = NULL;
    uintptr_t pte_phys = 0;
    uint64_t pmd_idx = 0;

    /* check minimum alignment requirement for given mmap region */
    __ASSERT(((va | pa | sz) & (PAGE_SIZE - 1)) == 0, "address/size are not page aligned\n");
    __ASSERT(pmdp != NULL, "PMD base address is NULL\n");

    uintptr_t va_temp = va;
    uintptr_t pa_temp = pa;
    uint64_t sz_temp = sz;

pmd_block_mapping:
    while (1)
    {
        if ((sz_temp >= PMD_SIZE) && ((va_temp & (PMD_SIZE - 1)) == 0))
        {
            pmd_idx = PMD_INDEX(va_temp);

            /** if the mapping size is block, set the PTE to leaf description **/
            if (pmd_none(pmdp[pmd_idx]))
            {
                pmdp[pmd_idx] = pfn_pmd(PFN_DOWN(pa_temp), __pgprot(pgprot_val(prot) | _PAGE_BASE | _PAGE_LEAF));
            }
            else
            {
                __ASSERT(0, "pmd size or address is overlap\n");
            }

            va_temp += PMD_SIZE;
            pa_temp += PMD_SIZE;
            sz_temp -= PMD_SIZE;
        }
        else
        {
            break;
        }
    }

    while (1)
    {
        if (sz_temp > 0)
        {
            pmd_idx = PMD_INDEX(va_temp);

            if (pmd_none(pmdp[pmd_idx]))
            {
                pte_phys = mmu_alloc_page();
                __ASSERT(pte_phys != 0, "alloc pte page error\n");

                pmdp[pmd_idx] = pfn_pmd(PFN_DOWN(pte_phys), PAGE_TABLE);
                ptep = get_pte_virt(pte_phys);
                memset(ptep, 0, PAGE_SIZE);
            }
            else
            {
                pte_phys = PFN_PHYS(_pmd_pfn(pmdp[pmd_idx]));
                ptep = get_pte_virt(pte_phys);
            }

            uintptr_t pte_start = va_temp;
            uintptr_t pte_end = va_temp + sz_temp;
            uintptr_t pte_next = pmd_addr_end(pte_start, pte_end);

            uint64_t pte_size = pte_next - pte_start;
            create_pte_mapping(ptep, va_temp, pa_temp, pte_size, prot);

            va_temp += pte_size;
            pa_temp += pte_size;
            sz_temp -= pte_size;

            goto pmd_block_mapping;
        }
        else
        {
            break;
        }
    }
}

void create_pgd_mapping(pgd_t *pgdp, uintptr_t va, uintptr_t pa, uint64_t sz, pgprot_t prot)
{
    pmd_t *pmdp = NULL;
    uintptr_t pmd_phys = 0;
    uint64_t pgd_idx = 0;

    /* check minimum alignment requirement for given mmap region */
    __ASSERT(((va | pa | sz) & (PAGE_SIZE - 1)) == 0, "address/size are not page aligned\n");
    __ASSERT(pgdp != NULL, "PGD base address is NULL\n");

    uintptr_t va_temp = va;
    uintptr_t pa_temp = pa;
    uint64_t sz_temp = sz;

pgd_block_mapping:
    while (1)
    {
        if ((sz_temp >= PGDIR_SIZE) && ((va_temp & (PGDIR_SIZE - 1)) == 0))
        {
            pgd_idx = PGD_INDEX(va_temp);

            /** if the mapping size is block, set the PTE to leaf description **/
            if (pgd_none(pgdp[pgd_idx]))
            {
                pgdp[pgd_idx] = pfn_pgd(PFN_DOWN(pa_temp), __pgprot(pgprot_val(prot) | _PAGE_BASE | _PAGE_LEAF));
            }
            else
            {
                __ASSERT(0, "pgd size or address is overlap\n");
            }

            va_temp += PGDIR_SIZE;
            pa_temp += PGDIR_SIZE;
            sz_temp -= PGDIR_SIZE;
        }
        else
        {
            break;
        }
    }

    while (1)
    {
        if (sz_temp > 0)
        {
            pgd_idx = PGD_INDEX(va_temp);

            if (pgd_none(pgdp[pgd_idx]))
            {
                pmd_phys = mmu_alloc_page();
                __ASSERT(pmd_phys != 0, "alloc pmd page error\n");

                pgdp[pgd_idx] = pfn_pgd(PFN_DOWN(pmd_phys), PAGE_TABLE);
                pmdp = get_pmd_virt(pmd_phys);
                memset(pmdp, 0, PAGE_SIZE);
            }
            else
            {
                pmd_phys = PFN_PHYS(_pgd_pfn(pgdp[pgd_idx]));
                pmdp = get_pmd_virt(pmd_phys);
            }

            uintptr_t pmd_start = va_temp;
            uintptr_t pmd_end = va_temp + sz_temp;
            uintptr_t pmd_next = pgd_addr_end(pmd_start, pmd_end);

            uint64_t pmd_size = pmd_next - pmd_start;
            create_pmd_mapping(pmdp, va_temp, pa_temp, pmd_size, prot);

            va_temp += pmd_size;
            pa_temp += pmd_size;
            sz_temp -= pmd_size;

            goto pgd_block_mapping;
        }
        else
        {
            break;
        }
    }
}

pgd_t *mmu_get_pdg_address(void)
{
    return (pgd_t *)(&kernel_mmu_table.multi[0]);
}

static inline void __SFENCE_VMA ()
{
    __ASM volatile ("sfence.vma");
}

void mmu_enable(void)
{
    uint64_t mmu_table_base = (uint64_t)mmu_get_pdg_address();

    risv64_satp rv64_satp = {.all = 0};
    rv64_satp.bit.mode = SATP_MODE_SV39;
    rv64_satp.bit.asid = 0;
    rv64_satp.bit.ppn = mmu_table_base >> PAGE_SHIFT;
    __RV_CSR_WRITE(CSR_SATP, rv64_satp.all);
    __SFENCE_VMA();
}


