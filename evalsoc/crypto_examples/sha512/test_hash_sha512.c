#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "nuclei_sdk_soc.h"
#include "nmsis_bench.h"

#include "api_sha512.h"
BENCH_DECLARE_VAR()

#define TEST_HASH_INPUT_LENGTH 1024
uint8_t message[TEST_HASH_INPUT_LENGTH * 6];

size_t test_rdrandom(unsigned char * dest, size_t len) {

    // Dumb random seed generation. Originally used to read
    // /dev/random, but this caused hangs on some machines.
    srand(time(NULL));

    for(size_t i =0; i < len; i ++) {
        dest[i] = (unsigned char)rand();
    }

    return len;

}

int8_t verify_results_uint64(uint64_t * ref, uint64_t * opt, int length)
{
    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (ref[i] != opt[i])
        {
            printf("Mismatch at %llu, ref %u, actual %llu\r\n", i, ref[i], opt[i]);
            flag = 1;
            break;
        }
    }

    return flag;
}
int main(void) {
    
    printf("benchmark_name : sha512\n");

    const int num_tests = 3;

    size_t     message_len  = TEST_HASH_INPUT_LENGTH  ;

    uint64_t   digest    [8];
    uint64_t   digest_ref    [8];
    int8_t ret;
    for(int i = 0; i < num_tests; i ++) {

        printf("#\n# test %d/%d\n",i + 1, num_tests);
        test_rdrandom(message, message_len);

        BENCH_START(sha512_hash);

        sha512_hash(
            digest,
            message    ,
            message_len
        );
        BENCH_END(sha512_hash);
        
       BENCH_START(sha512_hash_ref);

        sha512_hash_ref(
            digest_ref,
            message    ,
            message_len
        );
        BENCH_END(sha512_hash_ref);
        ret = verify_results_uint64((uint64_t *)digest_ref, (uint64_t *)digest, 8);
        if (ret != 0) {
          printf("sha256_hash test fail!\r\n");
        }

        message_len += TEST_HASH_INPUT_LENGTH / 2;
    }
    printf("test complete!!\r\n");

    return 0;
}
