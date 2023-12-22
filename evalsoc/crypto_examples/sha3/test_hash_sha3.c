#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "nuclei_sdk_soc.h"
#include "nmsis_bench.h"

#include "fips202.h"


BENCH_DECLARE_VAR()
#define TEST_HASH_INPUT_LENGTH 1024
uint8_t  hash_input[TEST_HASH_INPUT_LENGTH * 6];

size_t test_rdrandom(unsigned char * dest, size_t len) {

    // Dumb random seed generation. Originally used to read
    // /dev/random, but this caused hangs on some machines.
    srand(time(NULL));

    for(size_t i =0; i < len; i ++) {
        dest[i] = (unsigned char)rand();
    }

    return len      ;

}

int8_t verify_results_uint8(uint8_t * ref, uint8_t * opt, int length)
{
    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (ref[i] != opt[i])
        {
            printf("Mismatch at %u, ref %u, actual %u\r\n", i, ref[i], opt[i]);
            flag = 1;
            break;
        }
    }

    return flag;
}
int main(void) {

    printf("benchmark_name : sha3 \n");

    const int num_tests = 3;
        
    unsigned long long   hash_input_len = TEST_HASH_INPUT_LENGTH    ;
    unsigned char        hash_signature  [CRYPTO_HASH_SHA3_512_BYTES] ;
	  unsigned char        hash_signature_ref  [CRYPTO_HASH_SHA3_512_BYTES] ;

    int8_t ret;
    for(int i = 0; i < num_tests; i ++) {

        printf("#\n# test %d/%d\n",i + 1, num_tests);
        test_rdrandom(hash_input, hash_input_len);

        BENCH_START(FIPS202_SHA3_512);
        FIPS202_SHA3_512(
            hash_input    ,
            hash_input_len,
            hash_signature 
        );
        BENCH_END(FIPS202_SHA3_512);
        
        BENCH_START(FIPS202_SHA3_512_ref);
        FIPS202_SHA3_512_ref(
            hash_input    ,
            hash_input_len,
            hash_signature_ref
        );
        BENCH_END(FIPS202_SHA3_512_ref);
        ret = verify_results_uint8((uint8_t *)hash_signature_ref, (uint8_t *)hash_signature, CRYPTO_HASH_SHA3_512_BYTES);
        if (ret != 0) {
          printf("FIPS202_SHA3_512 test fail!\r\n");
        }

        hash_input_len += TEST_HASH_INPUT_LENGTH / 2;
    }
    printf("test complete!\r\n");

    return 0;
}
