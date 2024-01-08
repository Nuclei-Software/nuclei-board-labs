
#include <stdint.h>

/*!
@defgroup crypto_block_aes Crypto Block AES
@{

AES  |   Nk  | Nb   | Nr
-----|-------|------|---------------
128  |  4    | 4    | 10
192  |  6    | 4    | 12
256  |  8    | 4    | 14

*/

#ifndef __API_AES_REF_H__
#define __API_AES_REF_H__

/*!
@brief Key expansion function for the AES 128 parameterisation - encrypt
@param [out] rk - The expanded key schedule
@param [in]  ck - The cipher key to expand
*/
void    aes_128_enc_key_schedule_ref (
    uint32_t * const rk,
    uint8_t  * const ck
);

/*!
@brief Key expansion function for the AES 192 parameterisation - encrypt
@param [out] rk - The expanded key schedule
@param [in]  ck - The cipher key to expand
*/
void    aes_192_enc_key_schedule_ref (
    uint32_t * const rk,
    uint8_t  * const ck
);

/*!
@brief Key expansion function for the AES 256 parameterisation - encrypt
@param [out] rk - The expanded key schedule
@param [in]  ck - The cipher key to expand
*/
void    aes_256_enc_key_schedule_ref (
    uint32_t * const rk,
    uint8_t  * const ck
);


/*!
@brief Key expansion function for the AES 128 parameterisation - decrypt
@param [out] rk - The expanded key schedule
@param [in]  ck - The cipher key to expand
*/
void    aes_128_dec_key_schedule_ref (
    uint32_t * const rk,
    uint8_t  * const ck
);


/*!
@brief Key expansion function for the AES 192 parameterisation - decrypt
@param [out] rk - The expanded key schedule
@param [in]  ck - The cipher key to expand
*/
void    aes_192_dec_key_schedule_ref (
    uint32_t * const rk,
    uint8_t  * const ck
);

/*!
@brief Key expansion function for the AES 256 parameterisation - decrypt
@param [out] rk - The expanded key schedule
@param [in]  ck - The cipher key to expand
*/
void    aes_256_dec_key_schedule_ref (
    uint32_t * const rk,
    uint8_t  * const ck
);


/*!
@brief single-block AES 128 encrypt function
@param [out] ct - Output cipher text
@param [in]  pt - Input plaintext
@param [in]  rk - The expanded key schedule
@param [in]  nr - Number of encryption rounds to perform.
*/
void    aes_128_ecb_encrypt_ref (
    uint8_t     ct [AES_BLOCK_BYTES],
    uint8_t     pt [AES_BLOCK_BYTES],
    uint32_t  * rk
);

/*!
@brief single-block AES 192 encrypt function
@param [out] ct - Output cipher text
@param [in]  pt - Input plaintext
@param [in]  rk - The expanded key schedule
@param [in]  nr - Number of encryption rounds to perform.
*/
void    aes_192_ecb_encrypt_ref (
    uint8_t     ct [AES_BLOCK_BYTES],
    uint8_t     pt [AES_BLOCK_BYTES],
    uint32_t  * rk
);

/*!
@brief single-block AES 256 encrypt function
@param [out] ct - Output cipher text
@param [in]  pt - Input plaintext
@param [in]  rk - The expanded key schedule
@param [in]  nr - Number of encryption rounds to perform.
*/
void    aes_256_ecb_encrypt_ref (
    uint8_t     ct [AES_BLOCK_BYTES],
    uint8_t     pt [AES_BLOCK_BYTES],
    uint32_t  * rk
);

/*!
@brief single-block AES 128 decrypt function
@param [out] pt - Output plaintext
@param [in]  ct - Input cipher text
@param [in]  rk - The expanded key schedule
@param [in]  nr - Number of decryption rounds to perform.
*/
void    aes_128_ecb_decrypt_ref (
    uint8_t     pt [AES_BLOCK_BYTES],
    uint8_t     ct [AES_BLOCK_BYTES],
    uint32_t  * rk
);

/*!
@brief single-block AES 192 decrypt function
@param [out] pt - Output plaintext
@param [in]  ct - Input cipher text
@param [in]  rk - The expanded key schedule
@param [in]  nr - Number of decryption rounds to perform.
*/
void    aes_192_ecb_decrypt_ref (
    uint8_t     pt [AES_BLOCK_BYTES],
    uint8_t     ct [AES_BLOCK_BYTES],
    uint32_t  * rk
);


/*!
@brief single-block AES 256 decrypt function
@param [out] pt - Output plaintext
@param [in]  ct - Input cipher text
@param [in]  rk - The expanded key schedule
@param [in]  nr - Number of decryption rounds to perform.
*/
void    aes_256_ecb_decrypt_ref (
    uint8_t     pt [AES_BLOCK_BYTES],
    uint8_t     ct [AES_BLOCK_BYTES],
    uint32_t  * rk
);

#endif

//! @}
