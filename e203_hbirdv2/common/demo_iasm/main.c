// See LICENSE for license details.
#include <stdio.h>
#include "hbird_sdk_hal.h"

int main(void)
{

    printf("\n##############################################################\n");
    printf("##############################################################\n");
    printf("##############################################################\n");
    printf("##############################################################\n");
    printf("\nThis is a Demo which using the inline ASM to execute ADD operations\n");
    printf("We will use the inline assembly 'add' instruction to add two operands 100 and 200\n");
    printf("The expected result is 300\n");
    printf("\n\nIf the result is 300, then print PASS, otherwise FAIL\n");

    int sum;
    int add1 = 100;
    int add2 = 200;

    __asm__ __volatile__(
        "add %[dest],%[src1],%[src2]"
             :[dest]"=r"(sum)
             :[src1]"r"(add1), [src2]"r"(add2)
    );  

    if(sum == 300) {
        printf("!!! PASS !!!\n");
    }else{
        printf("!!! FAIL !!!\n");
    }

    return 0;
}


