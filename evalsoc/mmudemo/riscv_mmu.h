#ifndef __RISCV_MMU_H__
#define __RISCV_MMU_H__

#include <stdint.h>

/** Number of bits in a long int. */
#define BITS_PER_LONG    (8 * sizeof(uint64_t))

/**
 * @brief Create a contiguous bitmask starting at bit position @p l
 *        and ending at position @p h.
 */
#define GENMASK(h, l)  (((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

/*
 * pfn_t: encapsulates a page-frame number that is optionally backed
 * by memmap (struct page).  Whether a pfn_t has a 'struct page'
 * backing is indicated by flags in the high bits of the value.
 */
typedef struct
{
    uint64_t val;
} pfn_t;

/* Page Global Directory entry */
typedef struct
{
    uint64_t pgd;
} pgd_t;

/* Page Middle Directory entry */
typedef struct
{
    uint64_t pmd;
} pmd_t;

/* Page Table entry */
typedef struct
{
    uint64_t pte;
} pte_t;

typedef struct
{
    uint64_t pgprot;
} pgprot_t;


/*
 * risv64 PTE format:
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * | 63 | 62 61 | 60 54 | 53  10 | 9             8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 * | N  |   MT  |  RSV  |   PFN  | reserved for SW | D | A | G | U | X | W | R | V |
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
#define _PAGE_PFN_MASK  GENMASK(53, 10)

/*
 * [62:61] Svpbmt Memory Type definitions:
 *
 *  00 - PMA    Normal Cacheable, No change to implied PMA memory type
 *  01 - NC     Non-cacheable, idempotent, weakly-ordered Main Memory
 *  10 - IO     Non-cacheable, non-idempotent, strongly-ordered I/O memory
 *  11 - Rsvd   Reserved for future standard use
 */
#define _PAGE_NOCACHE_SVPBMT    (1UL << 61)
#define _PAGE_IO_SVPBMT         (2UL << 61)
#define _PAGE_PMA_SVPBMT        (0UL << 61)

#define _PAGE_ACCESSED_OFFSET   (6)
#define _PAGE_PFN_SHIFT         (10)

#define _PAGE_PRESENT           (1 << 0)
#define _PAGE_READ              (1 << 1)    /* Readable */
#define _PAGE_WRITE             (1 << 2)    /* Writable */
#define _PAGE_EXEC              (1 << 3)    /* Executable */
//#define _PAGE_USER              (1 << 4)    /* User */
#define _PAGE_USER              (0 << 4)    /* User */
#define _PAGE_GLOBAL            (1 << 5)    /* Global */
#define _PAGE_ACCESSED          (1 << 6)    /* Set by hardware on any access */
#define _PAGE_DIRTY             (1 << 7)    /* Set by hardware on any write */
#define _PAGE_SOFT              (1 << 8)    /* Reserved for software */

#define _PAGE_SPECIAL           (_PAGE_SOFT)
#define _PAGE_TABLE             (_PAGE_PRESENT)

/*
 * _PAGE_PROT_NONE is set on not-present pages (and ignored by the hardware) to
 * distinguish them from swapped out pages
 */
#define _PAGE_PROT_NONE         (_PAGE_GLOBAL)

/*
 * when all of R/W/X are zero, the PTE is a pointer to the next level
 * of the page table; otherwise, it is a leaf PTE.
 */
#define _PAGE_LEAF              (_PAGE_READ | _PAGE_WRITE | _PAGE_EXEC | _PAGE_PRESENT)


/* Page protection bits */
#define _PAGE_BASE              (_PAGE_DIRTY | _PAGE_ACCESSED | _PAGE_GLOBAL | _PAGE_USER)

#define PAGE_NONE               __pgprot(_PAGE_PROT_NONE | _PAGE_READ)
#define PAGE_READ               __pgprot(_PAGE_BASE | _PAGE_READ)
#define PAGE_WRITE              __pgprot(_PAGE_BASE | _PAGE_READ | _PAGE_WRITE)
#define PAGE_EXEC               __pgprot(_PAGE_BASE | _PAGE_EXEC)
#define PAGE_READ_EXEC          __pgprot(_PAGE_BASE | _PAGE_READ | _PAGE_EXEC)
#define PAGE_WRITE_EXEC         __pgprot(_PAGE_BASE | _PAGE_READ | _PAGE_EXEC | _PAGE_WRITE)
#define PAGE_TABLE              __pgprot(_PAGE_TABLE)

#define PAGE_COPY               PAGE_READ
#define PAGE_COPY_EXEC          PAGE_EXEC
#define PAGE_COPY_READ_EXEC     PAGE_READ_EXEC
#define PAGE_SHARED             PAGE_WRITE
#define PAGE_SHARED_EXEC        PAGE_WRITE_EXEC

/* Region definition data structure */
struct mmu_region
{
    /* Region Base Physical Address */
    uintptr_t base_pa;
    /* Region Base Virtual Address */
    uintptr_t base_va;
    /* Region size */
    uint64_t size;
    /* Region Name */
    const char *name;
    /* Region Attributes */
    pgprot_t attrs;
};

extern pgd_t *mmu_get_pdg_address(void);
extern void mmu_enable(void);
extern void create_pgd_mapping(pgd_t *pgdp, uintptr_t va, uintptr_t pa, uint64_t sz, pgprot_t prot);

#endif  /* __RISCV_MMU_H__ */

