
/*!
@addtogroup crypto_block_aes_reference AES Reference
@brief Byte-wise Reference implementation of AES.
@ingroup crypto_block_aes
@{
*/

#include "../api_aes.h"
#include "../api_aes_ref.h"

//! AES Inverse SBox
static const uint8_t d_sbox[256] = { 
0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81,
0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e,
0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23,
0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 0x08, 0x2e, 0xa1, 0x66,
0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 0x72,
0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65,
0xb6, 0x92, 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46,
0x57, 0xa7, 0x8d, 0x9d, 0x84, 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 0xd0, 0x2c, 0x1e, 0x8f, 0xca,
0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 0x3a, 0x91,
0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6,
0x73, 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8,
0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f,
0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2,
0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 0x1f, 0xdd, 0xa8,
0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93,
0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb,
0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6,
0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

//! Combined inverse subbytes and shiftrows transformations.
static void aes_subbytes_shiftrows_dec(
    uint8_t     pt[16]       //!< Current block state
){
    uint8_t tmp;
    
    // row 0
    pt[ 0] = d_sbox[pt[ 0]];
    pt[ 4] = d_sbox[pt[ 4]];
    pt[ 8] = d_sbox[pt[ 8]];
    pt[12] = d_sbox[pt[12]];

    // row 1
    tmp    = d_sbox[pt[13]];
    pt[13] = d_sbox[pt[ 9]];
    pt[ 9] = d_sbox[pt[ 5]];
    pt[ 5] = d_sbox[pt[ 1]];
    pt[ 1] = tmp;
    
    // row 2
    tmp    = d_sbox[pt[10]];
    pt[10] = d_sbox[pt[ 2]];
    pt[ 2] = tmp;

    tmp    = d_sbox[pt[14]];
    pt[14] = d_sbox[pt[ 6]];
    pt[ 6] = tmp;

    // row 3
    tmp    = d_sbox[pt[ 7]];
    pt[ 7] = d_sbox[pt[11]];
    pt[11] = d_sbox[pt[15]];
    pt[15] = d_sbox[pt[ 3]];
    pt[ 3] = tmp;
}

#define XT2(x) ((x << 1) ^ (x & 0x80 ? 0x1b : 0x00))
#define XT3(x) (XT2(x) ^ x)
#define XT4(x) XT2(XT2(x))
#define XT8(x) XT2(XT4(x))
#define XT9(x) (XT8(x) ^ x)
#define XTB(x) (XT8(x) ^ XT2(x) ^ x)
#define XTD(x) (XT8(x) ^ XT4(x) ^ x)
#define XTE(x) (XT8(x) ^ XT4(x) ^ XT2(x))

//! Inverse mix columns transformation.
static void aes_mix_columns_dec(
    uint8_t     pt[16]       //!< Current block state
){
    // Col 0
    for(int i = 0; i < 4; i ++) {
        uint8_t b0,b1,b2,b3;
        uint8_t s0,s1,s2,s3;
        
        s0 = pt[4*i+0];
        s1 = pt[4*i+1];
        s2 = pt[4*i+2];
        s3 = pt[4*i+3];

        b0 = XTE(s0) ^ XTB(s1) ^ XTD(s2) ^ XT9(s3);
        b1 = XT9(s0) ^ XTE(s1) ^ XTB(s2) ^ XTD(s3);
        b2 = XTD(s0) ^ XT9(s1) ^ XTE(s2) ^ XTB(s3);
        b3 = XTB(s0) ^ XTD(s1) ^ XT9(s2) ^ XTE(s3);

        pt[4*i+0] = b0;
        pt[4*i+1] = b1;
        pt[4*i+2] = b2;
        pt[4*i+3] = b3;
    }
}

// defined in aes_enc.c
extern void    aes_key_schedule (
    uint32_t * const rk , //!< Output Nk*(Nr+1) word cipher key.
    uint8_t  * const ck , //!< Input Nk byte cipher key
    const int  Nk , //!< Number of words in the key.
    const int  Nr   //!< Number of rounds.
);

void    aes_128_dec_key_schedule_ref (
    uint32_t * const rk,
    uint8_t  * const ck
){
    aes_key_schedule(rk, ck, AES_128_NK, AES_128_NR);
}

void    aes_192_dec_key_schedule_ref (
    uint32_t * const rk,
    uint8_t  * const ck
){
    aes_key_schedule(rk, ck, AES_192_NK, AES_192_NR);
}


void    aes_256_dec_key_schedule_ref (
    uint32_t * const rk,
    uint8_t  * const ck
){
    aes_key_schedule(rk, ck, AES_256_NK, AES_256_NR);
}


void    aes_ecb_decrypt (
    uint8_t     pt [AES_BLOCK_BYTES],
    uint8_t     ct [AES_BLOCK_BYTES],
    uint32_t  * rk                  ,
    int         nr
){
    for(int i = 0; i < 16; i ++) {
        pt[i] = ct[i] ^ ((uint8_t*)rk)[(16*nr) + i];
    }

    for(int round = nr -1; round >= 1; round --) {
        
        aes_subbytes_shiftrows_dec(pt);
    
        for(int i = 0; i < 16; i ++) {
            pt[i] ^= ((uint8_t*)rk)[(16*round) + i];
        }

        aes_mix_columns_dec(pt);

    }
        
    aes_subbytes_shiftrows_dec(pt);

    for(int i = 0; i < 16; i ++) {
        pt[i] ^= ((uint8_t*)rk)[i];
    }
}

void    aes_128_ecb_decrypt_ref (
    uint8_t     pt [AES_BLOCK_BYTES],
    uint8_t     ct [AES_BLOCK_BYTES],
    uint32_t  * rk
){
    aes_ecb_decrypt(pt,ct,rk,AES_128_NR);
}

void    aes_192_ecb_decrypt_ref (
    uint8_t     pt [AES_BLOCK_BYTES],
    uint8_t     ct [AES_BLOCK_BYTES],
    uint32_t  * rk
){
    aes_ecb_decrypt(pt,ct,rk,AES_192_NR);
}

void    aes_256_ecb_decrypt_ref (
    uint8_t     pt [AES_BLOCK_BYTES],
    uint8_t     ct [AES_BLOCK_BYTES],
    uint32_t  * rk
){
    aes_ecb_decrypt(pt,ct,rk,AES_256_NR);
}

//!@}
