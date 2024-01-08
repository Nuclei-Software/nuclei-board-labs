#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "nuclei_sdk_soc.h"
#include "share/util.h"
#include "nmsis_bench.h"
#include "api_sm4.h"
BENCH_DECLARE_VAR()

uint8_t pts [1][16] = {
    {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
     0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10},
};

uint8_t mks [1][16] = {
    {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
     0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10},
};

uint8_t cts [1][16] = {
    {0x68, 0x1E, 0xDF, 0x34, 0xD2, 0x06, 0x96, 0x5E,
     0x86, 0xB3, 0xE9, 0x4F, 0x53, 0x6E, 0x42, 0x46},
};

int main(void) {

    printf("benchmark_name : sm4\r\n");

    uint64_t start_instrs;
    int tr = 0;

    for(int test = 0; test < 1; test ++) {
        uint8_t * pt = pts[test];
        uint8_t * mk = mks[test];
        uint8_t   ct [16];
        uint8_t   fi [16];

        uint32_t  erk[32];
        uint32_t  drk[32];
        BENCH_START(sm4_key_schedule_enc);
        sm4_key_schedule_enc(erk, mk);
        BENCH_END(sm4_key_schedule_enc);

        BENCH_START(sm4_key_schedule_dec);
        sm4_key_schedule_dec(drk, mk);
        BENCH_END(sm4_key_schedule_dec);

        BENCH_START(sm4_block_enc);
        sm4_block_enc_dec   (ct,pt,erk);
        BENCH_END(sm4_block_enc);

        BENCH_START(sm4_block_dec);
        sm4_block_enc_dec   (fi,ct,drk);
        BENCH_END(sm4_block_dec);

        for(int i = 0; i < 16; i ++) {

            if(ct[i] != cts[test][i]) {
                tr |= 1;
            }
            if(fi[i] != pt[i]) {
                tr |= 2;
            }
        }

        if(tr) {
            printf("Failed with code: %d\n", tr);
            break;
        }

        BENCH_START(sm4_key_schedule_enc_ref);
        sm4_key_schedule_enc_ref(erk, mk);
        BENCH_END(sm4_key_schedule_enc_ref);

        BENCH_START(sm4_key_schedule_dec_ref);
        sm4_key_schedule_dec_ref(drk, mk);
        BENCH_END(sm4_key_schedule_dec_ref);

        BENCH_START(sm4_block_enc_ref);
        sm4_block_enc_dec_ref   (ct,pt,erk);
        BENCH_END(sm4_block_enc_ref);

        BENCH_START(sm4_block_dec_ref);
        sm4_block_enc_dec_ref   (fi,ct,drk);
        BENCH_END(sm4_block_dec_ref);


        for(int i = 0; i < 16; i ++) {

            if(ct[i] != cts[test][i]) {
                tr |= 1; 
            }
            if(fi[i] != pt[i])        {
                tr |= 2;
            }

        }

        if(tr) {
            printf("Failed with code: %d\n", tr);
            break;
        }
    }
    if (tr == 0) {
      printf("test pass\r\n");
    }

    return 0;
}
