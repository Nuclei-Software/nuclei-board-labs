#include "nuclei_sdk_hal.h"
#include <stdio.h>

// used for jump table index < 32, cm.jt
// https://github.com/riscv/riscv-code-size-reduction/blob/main/Zc-specification/cm_jt.adoc
void __attribute__ ((naked)) zcmt_cmjt(void)
{
    asm volatile("cm.jt 0 \n");
}

// used for jump table index >= 32, cm.jalt
// https://github.com/riscv/riscv-code-size-reduction/blob/main/Zc-specification/cm_jalt.adoc
void __attribute__ ((naked)) zcmt_cmjalt(void)
{
#if __riscv_xlen == 32
    asm volatile(
            "add sp,sp,-4\n"
            "sw ra,0(sp) \n"
            "cm.jalt 255 \n"
            "lw ra,0(sp) \n"
            "add sp,sp,4\n"
            "ret");
#else
    asm volatile(
            "add sp,sp,-8\n"
            "sd ra,0(sp) \n"
            "cm.jalt 255 \n"
            "ld ra,0(sp) \n"
            "add sp,sp,8\n"
            "ret");
#endif
}

// Test func 1
void abc()
{
    printf("hello from abc\n");
}

// Test func 2
void cdf()
{
    printf("hello from cdf\n");
}

// Need to align at 64bytes, required by jvt csr
// The value in the BASE field must always be aligned on a 64-byte boundary. see https://github.com/riscv/riscv-code-size-reduction/blob/main/Zc-specification/jvt_csr.adoc
// For this encoding to decode as cm.jalt, index>=32, otherwise it decodes as cm.jt
// max entries are 0-255, total 256 entries
// TODO jump table need to place in a ram where cpu instruction fetch unit and data acess unit can both access it
static unsigned long jptbl[256] __ALIGNED(64) = {};

unsigned long update_jump_table(unsigned long index, unsigned long funptr)
{
    unsigned long *tbptr = (unsigned long *)__RV_CSR_READ(CSR_JVT);

    if (tbptr != jptbl) {
        printf("jump table not match!\n");
        return 1;
    }
    tbptr[index] = funptr;
    // fence.i is required to flush cache if cache present
    __FENCE_I();
}

// NOTE: About Zcmt table jump, see https://github.com/riscv/riscv-code-size-reduction/blob/main/Zc-specification/tablejump.adoc
void main(void)
{
    __RV_CSR_WRITE(CSR_JVT, jptbl);

    printf("Jump table located in 0x%x\n", __RV_CSR_READ(CSR_JVT));

    printf("--> Test cm.jt instruction!\n");
    // let cm.jt jump to abc
    update_jump_table(0, (unsigned long)abc);
    // before execute this function the jump table must be initialized
    zcmt_cmjt();

    // let cm.jt jump to cdf
    update_jump_table(0, (unsigned long)cdf);
    zcmt_cmjt();

    printf("--> Test cm.jalt instruction!\n");
    // let cm.jalt jump to abc
    update_jump_table(255, (unsigned long)abc);
    zcmt_cmjalt();

    // let cm.jalt jump to cdf
    update_jump_table(255, (unsigned long)cdf);
    zcmt_cmjalt();
}
