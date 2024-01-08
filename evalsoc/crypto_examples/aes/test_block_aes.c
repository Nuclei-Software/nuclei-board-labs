#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "nuclei_sdk_soc.h"
#include "nmsis_bench.h"
#include "share/util.h"

#include "api_aes.h"
#include "api_aes_ref.h"

BENCH_DECLARE_VAR()

size_t test_rdrandom(unsigned char * dest, size_t len) {

    // Dumb random seed generation. Originally used to read
    // /dev/random, but this caused hangs on some machines.
    srand(time(NULL));

    for(size_t i =0; i < len; i ++) {
        dest[i] = (unsigned char)rand();
    }

    return len;

}

int8_t verify_results_uint8(uint8_t * ref, uint8_t * opt, int length)
{
    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (ref[i] != opt[i])
        {
            printf("Mismatch at %d, ref 0x%x, actual 0x%x\r\n", i, ref[i], opt[i]);
            flag = 1;
            break;
        }
    }

    return flag;
}

void test_aes_128(int num_tests) {

    // Start with known inputs from FIPS 197, Appendix B.
    uint8_t  key [AES_128_KEY_BYTES ] = {0x2b ,0x7e ,0x15 ,0x16 ,0x28 ,0xae ,0xd2 ,0xa6 ,0xab ,0xf7 ,0x15 ,0x88 ,0x09 ,0xcf ,0x4f ,0x3c};
    uint8_t  pt  [AES_BLOCK_BYTES   ] = {0x32 ,0x43 ,0xf6 ,0xa8 ,0x88 ,0x5a ,0x30 ,0x8d ,0x31 ,0x31 ,0x98 ,0xa2 ,0xe0 ,0x37 ,0x07 ,0x34};
    uint32_t erk [AES_128_RK_WORDS  ] = {0}; //!< Roundkeys (encrypt)
    uint32_t drk [AES_128_RK_WORDS  ] = {0}; //!< Roundkeys (decrypt)
    uint8_t  ct  [AES_BLOCK_BYTES   ] = {0};
    uint8_t  pt2 [AES_BLOCK_BYTES   ] = {0};

    uint32_t erk_ref [AES_128_RK_WORDS  ] = {0}; //!< Roundkeys (encrypt)
    uint32_t drk_ref [AES_128_RK_WORDS  ] = {0}; //!< Roundkeys (decrypt)
    uint8_t  ct_ref  [AES_BLOCK_BYTES   ] = {0};
    uint8_t  pt2_ref [AES_BLOCK_BYTES   ] = {0};

    int8_t ret;

    for(int i = 0; i < num_tests; i ++) {

        printf("#\n# AES 128 test %d/%d\n",i + 1 , num_tests);
        // enc key
        BENCH_START(aes_128_enc_key_schedule);
        aes_128_enc_key_schedule(erk, key    );
        BENCH_END(aes_128_enc_key_schedule);

        BENCH_START(aes_128_enc_key_schedule_ref);
        aes_128_enc_key_schedule_ref(erk_ref, key    );
        BENCH_END(aes_128_enc_key_schedule_ref);

        // aes_128 ecb encrypt
        BENCH_START(aes_128_ecb_encrypt);
        aes_128_ecb_encrypt     (ct , pt, erk);
        BENCH_END(aes_128_ecb_encrypt);
        
        BENCH_START(aes_128_ecb_encrypt_ref);
        aes_128_ecb_encrypt_ref     (ct_ref , pt, erk_ref);
        BENCH_END(aes_128_ecb_encrypt_ref);

        ret = verify_results_uint8(ct_ref, ct, AES_BLOCK_BYTES);
        if (ret != 0) {
          printf("aes_128_ecb_encrypt test fail!\r\n");
        }

        // dec key
        BENCH_START(aes_128_dec_key_schedule);
        aes_128_dec_key_schedule(drk, key    );
        BENCH_END(aes_128_dec_key_schedule);

        BENCH_START(aes_128_dec_key_schedule_ref);
        aes_128_dec_key_schedule_ref(drk_ref, key    );
        BENCH_END(aes_128_dec_key_schedule_ref);

        // aes_128 ecb decrypt
        BENCH_START(aes_128_ecb_decrypt);
        aes_128_ecb_decrypt     (pt2, ct, drk);
        BENCH_END(aes_128_ecb_decrypt);

        BENCH_START(aes_128_ecb_decrypt_ref);
        aes_128_ecb_decrypt_ref     (pt2_ref, ct, drk_ref);
        BENCH_END(aes_128_ecb_decrypt_ref);

        ret = verify_results_uint8(pt2_ref, pt2, AES_BLOCK_BYTES);
        if (ret != 0) {
          printf(" aes_128_ecb_decrypt test fail!\r\n");
        }
        

        // New random inputs
        test_rdrandom(pt    , AES_BLOCK_BYTES   );
        test_rdrandom(key   , AES_128_KEY_BYTES );

    }
}

void test_aes_192(int num_tests) {

    // Start with known inputs from
    //  https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/AES_Core192.pdf
    uint8_t  key [AES_192_KEY_BYTES ] = {
        0x8E,0x73,0xB0,0xF7,
        0xDA,0x0E,0x64,0x52,
        0xC8,0x10,0xF3,0x2B,
        0x80,0x90,0x79,0xE5,
        0x62,0xF8,0xEA,0xD2,
        0x52,0x2C,0x6B,0x7B,
    };
    uint8_t  pt  [AES_BLOCK_BYTES   ] = {
        0x6B,0xC1,0xBE,0xE2,
        0x2E,0x40,0x9F,0x96,
        0xE9,0x3D,0x7E,0x11,
        0x73,0x93,0x17,0x2A,
    };
    uint32_t erk [AES_192_RK_WORDS  ]; //!< Roundkeys (encrypt)
    uint32_t drk [AES_192_RK_WORDS  ]; //!< Roundkeys (decrypt)
    uint8_t  ct  [AES_BLOCK_BYTES   ];
    uint8_t  pt2 [AES_BLOCK_BYTES   ];

    uint32_t erk_ref [AES_192_RK_WORDS  ]; //!< Roundkeys (encrypt)
    uint32_t drk_ref [AES_192_RK_WORDS  ]; //!< Roundkeys (decrypt)
    uint8_t  ct_ref  [AES_BLOCK_BYTES   ];
    uint8_t  pt2_ref [AES_BLOCK_BYTES   ];

    for(int i = 0; i < num_tests; i ++) {
        printf("#\n# AES 192 test %d/%d\n",i + 1, num_tests);
        for(int i = 0; i < AES_192_RK_WORDS; i ++) {
            erk[i] = 0;
            drk[i] = 0;
            erk_ref[i] = 0;
            drk_ref[i] = 0;
        }
        // enc key
        BENCH_START(aes_192_enc_key_schedule);
        aes_192_enc_key_schedule(erk, key    );
        BENCH_END(aes_192_enc_key_schedule);

        BENCH_START(aes_192_enc_key_schedule_ref);
        aes_192_enc_key_schedule_ref(erk_ref, key    );
        BENCH_END(aes_192_enc_key_schedule_ref);

        // aes_192 ecb encrypt
        BENCH_START(aes_192_ecb_encrypt);
        aes_192_ecb_encrypt     (ct , pt, erk);
        BENCH_END(aes_192_ecb_encrypt);

        BENCH_START(aes_192_ecb_encrypt_ref);
        aes_192_ecb_encrypt_ref     (ct_ref , pt, erk_ref);
        BENCH_END(aes_192_ecb_encrypt_ref);
        
        // dec key
        BENCH_START(aes_192_dec_key_schedule);
        aes_192_dec_key_schedule(drk, key    );
        BENCH_END(aes_192_dec_key_schedule);

        BENCH_START(aes_192_dec_key_schedule_ref);
        aes_192_dec_key_schedule_ref(drk_ref, key    );
        BENCH_END(aes_192_dec_key_schedule_ref);

        // aes_192 ecb decrypt
        BENCH_START(aes_192_ecb_decrypt);
        aes_192_ecb_decrypt     (pt2, ct, drk);
        BENCH_END(aes_192_ecb_decrypt);

        BENCH_START(aes_192_ecb_decrypt_ref);
        aes_192_ecb_decrypt_ref     (pt2_ref, ct, drk_ref);
        BENCH_END(aes_192_ecb_decrypt_ref);

        verify_results_uint8(ct_ref, ct, AES_BLOCK_BYTES);
        verify_results_uint8(pt2_ref, pt2, AES_BLOCK_BYTES);

        // New random inputs
        test_rdrandom(pt    , AES_BLOCK_BYTES   );
        test_rdrandom(key   , AES_192_KEY_BYTES );

    }

}

void test_aes_256(int num_tests) {

    // Start with known inputs from
    //  https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/AES_Core256.pdf
    uint8_t  key [AES_256_KEY_BYTES ] = {
        0x60,0x3D,0xEB,0x10,
        0x15,0xCA,0x71,0xBE,
        0x2B,0x73,0xAE,0xF0,
        0x85,0x7D,0x77,0x81,
        0x1F,0x35,0x2C,0x07,
        0x3B,0x61,0x08,0xD7,
        0x2D,0x98,0x10,0xA3,
        0x09,0x14,0xDF,0xF4
    };
    uint8_t  pt  [AES_BLOCK_BYTES   ] = {
        0x6B,0xC1,0xBE,0xE2,
        0x2E,0x40,0x9F,0x96,
        0xE9,0x3D,0x7E,0x11,
        0x73,0x93,0x17,0x2A,
    };
    uint32_t erk [AES_256_RK_WORDS  ]; //!< Roundkeys (encrypt)
    uint32_t drk [AES_256_RK_WORDS  ]; //!< Roundkeys (decrypt)
    uint8_t  ct  [AES_BLOCK_BYTES   ];
    uint8_t  pt2 [AES_BLOCK_BYTES   ];

    uint32_t erk_ref [AES_256_RK_WORDS  ]; //!< Roundkeys (encrypt)
    uint32_t drk_ref [AES_256_RK_WORDS  ]; //!< Roundkeys (decrypt)
    uint8_t  ct_ref  [AES_BLOCK_BYTES   ];
    uint8_t  pt2_ref [AES_BLOCK_BYTES   ];

    for(int i = 0; i < num_tests; i ++) {
        printf("#\n# AES 256 test %d/%d\n",i + 1, num_tests);
        for(int i = 0; i < AES_256_RK_WORDS; i ++) {
            erk[i] = 0;
            drk[i] = 0;
            erk_ref[i] = 0;
            drk_ref[i] = 0;
        }
        // enc key
        BENCH_START(aes_256_enc_key_schedule);
        aes_256_enc_key_schedule(erk, key    );
        BENCH_END(aes_256_enc_key_schedule);

        BENCH_START(aes_256_enc_key_schedule_ref);
        aes_256_enc_key_schedule_ref(erk_ref, key    );
        BENCH_END(aes_256_enc_key_schedule_ref);

        // aes_256 ecb encrypt
        BENCH_START(aes_256_ecb_encrypt);
        aes_256_ecb_encrypt     (ct , pt, erk);
        BENCH_END(aes_256_ecb_encrypt)

        BENCH_START(aes_256_ecb_encrypt_ref);
        aes_256_ecb_encrypt_ref     (ct_ref , pt, erk_ref);
        BENCH_END(aes_256_ecb_encrypt_ref);
        
        // dec key
        BENCH_START(aes_256_dec_key_schedule);
        aes_256_dec_key_schedule(drk, key    );
        BENCH_END(aes_256_dec_key_schedule);
        
        BENCH_START(aes_256_dec_key_schedule_ref);
        aes_256_dec_key_schedule_ref(drk_ref, key    );
        BENCH_END(aes_256_dec_key_schedule_ref);

        // aes_256 ecb decrypt
        BENCH_START(aes_256_ecb_decrypt);
        aes_256_ecb_decrypt     (pt2, ct, drk);
        BENCH_END(aes_256_ecb_decrypt);
        
        BENCH_START(aes_256_ecb_decrypt_ref);
        aes_256_ecb_decrypt_ref     (pt2_ref, ct, drk_ref);
        BENCH_END(aes_256_ecb_decrypt_ref);

        verify_results_uint8(ct_ref, ct, AES_BLOCK_BYTES);
        verify_results_uint8(pt2_ref, pt2, AES_BLOCK_BYTES);

        // New random inputs
        test_rdrandom(pt    , AES_BLOCK_BYTES   );
        test_rdrandom(key   , AES_256_KEY_BYTES );

    }
}

int main(void) {

    printf("benchmark_name : aes\n");
    BENCH_INIT();

    int num_tests = 3;

    test_aes_128(num_tests);
    test_aes_192(num_tests);
    test_aes_256(num_tests);
    printf("test complete!\r\n");
    return 0;
}
