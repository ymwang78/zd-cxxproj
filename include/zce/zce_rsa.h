#pragma once
/* ***************************************************************
//  zce_rsa   version:  1.0  date: 2015/02/08 
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2015 - All Rights Reserved
// ***************************************************************
// 
// **************************************************************/
#ifndef __zce_rsa_h__
#define __zce_rsa_h__

#include <zce/zce_config.h>

namespace zce {
class RefBlock;
}

/*
 * RSA with OpenSSL. zce_rsa_public_encrypt(), zce_rsa_public_verify(), zce_rsa_private_sign()
 * and zce_rsa_private_decrypt() return 0 on success and -1 on any failure, a key that cannot be
 * parsed included (ymwang78/libzce#231: verify and sign used to return 0 for such a key, and
 * decrypt for a ciphertext longer than the modulus). The three that write dblock leave it empty
 * when they fail.
 *
 * A public key is given as PEM text starting with "-----BEGIN PUBLIC KEY-----", as
 * zce_rsa_genkey() writes it, or as a DER X.509 certificate, whose key is used.
 * A private key is given as PEM text starting with "-----BEGIN PRIVATE KEY-----" or
 * "-----BEGIN RSA PRIVATE KEY-----", or as a DER PKCS#12 file.
 * key / keylen is the key's text or bytes; it need not be NUL-terminated.
 */

/**
 * @brief Generates a 2048-bit RSA key pair.
 * @param pubkey Receives the public key, PEM, "-----BEGIN PUBLIC KEY-----".
 * @param prikey Receives the private key, PEM, unencrypted, "-----BEGIN PRIVATE KEY-----".
 * @return 0.
 */
int zce_rsa_genkey(zce::RefBlock& pubkey, zce::RefBlock& prikey);

int zce_randbytes(zce::RefBlock& pubkey, int len);

/**
 * @brief Encrypts input with a public key, RSAES-PKCS1-v1_5.
 * @param dblock Receives the ciphertext, as long as the key's modulus. What it held before is
 *        not used. On failure it is left empty.
 * @param input The plaintext, at most the modulus size - 11 bytes (245 for a 2048-bit key).
 * @param key The public key (see above).
 * @return 0, or -1 when the key cannot be parsed or encryption fails.
 */
int ZCE_API zce_rsa_public_encrypt(zce::RefBlock& dblock,
    const zce_byte* input,
    size_t inputlen,
    const char* key,
    size_t keylen);

/**
 * @brief Checks that dblock is a signature of input under a public key, RSASSA-PKCS1-v1_5 with
 *        SHA-256, as zce_rsa_private_sign() makes it.
 * @param dblock The signature. It is not changed.
 * @param input The signed message.
 * @param key The public key (see above).
 * @return 0 only when the signature is valid; -1 when it is not, and when the key cannot be
 *         parsed.
 */
int ZCE_API zce_rsa_public_verify(zce::RefBlock& dblock,
    const zce_byte* input,
    size_t inputlen,
    const char* key,
    size_t keylen);

/**
 * @brief Signs input with a private key, RSASSA-PKCS1-v1_5 with SHA-256.
 * @param dblock On entry, the private key's password, or empty when it has none. On success it
 *        is replaced by the signature, as long as the key's modulus; on failure it is left empty.
 * @param input The message to sign.
 * @param key The private key (see above).
 * @return 0, or -1 when the key cannot be parsed (or needs another password) or signing fails.
 */
int ZCE_API zce_rsa_private_sign(zce::RefBlock& dblock,
    const zce_byte* input,
    size_t inputlen,
    const char* key,
    size_t keylen);

/**
 * @brief Decrypts what zce_rsa_public_encrypt() encrypted, RSAES-PKCS1-v1_5.
 *
 * Success does not mean that input was encrypted under this key: since OpenSSL 3.2 a ciphertext
 * whose padding does not check out decrypts to a plaintext made up from the key and the
 * ciphertext instead of failing (implicit rejection, against Bleichenbacher-style attacks).
 * Authenticate the data some other way.
 * @param dblock On entry, the private key's password, or empty when it has none. On success it
 *        is replaced by the plaintext; on failure it is left empty.
 * @param input The ciphertext, at most as long as the key's modulus.
 * @param key The private key (see above).
 * @return 0, or -1 when the key cannot be parsed (or needs another password), input is longer
 *         than the modulus, or decryption fails.
 */
int ZCE_API zce_rsa_private_decrypt(zce::RefBlock& dblock,
    const zce_byte* input,
    size_t inputlen,
    const char* key,
    size_t keylen);

/**
 * @brief Encrypts input with AES, PKCS#7 padded.
 * @param dblock Receives the ciphertext. What it held before is not used. On failure it is left
 *        empty.
 * @param iv_dblock For "aes_256_cbc", the 16-byte IV. When it is shorter, a random IV replaces
 *        it, which the caller has to keep for decrypting. Not used for "aes_128_ecb".
 * @param key The key, exactly 16 bytes for "aes_128_ecb" and 32 for "aes_256_cbc".
 * @param mode "aes_128_ecb" or "aes_256_cbc", in any case.
 * @return The ciphertext length, or -1 for an unknown mode, a key whose length does not match
 *         the mode (ymwang78/libzce#232: it used to be read past its end or cut), or a failure
 *         in OpenSSL.
 */
int ZCE_API zce_aes_encrypt(zce::RefBlock& dblock,
    zce::RefBlock& iv_dblock,
    const zce_byte* input,
    size_t inputlen,
    const zce_byte* key,
    size_t keylen,
    const char* mode);

/**
 * @brief Decrypts what zce_aes_encrypt() made.
 * @param dblock Receives the plaintext. What it held before is not used. On failure it is left
 *        empty.
 * @param iv_dblock For "aes_256_cbc", the IV the ciphertext was made with: at least 16 bytes, of
 *        which the first 16 are used. Not used for "aes_128_ecb".
 * @param key The key, exactly 16 bytes for "aes_128_ecb" and 32 for "aes_256_cbc".
 * @param mode "aes_128_ecb" or "aes_256_cbc", in any case.
 * @return The plaintext length, or -1 for an unknown mode, a key whose length does not match the
 *         mode, a CBC IV shorter than 16 bytes, or a ciphertext that does not decrypt: bad
 *         padding, which is how a wrong key usually shows, or a length that is not a whole
 *         number of blocks.
 */
int ZCE_API zce_aes_decrypt(zce::RefBlock& dblock,
    const zce::RefBlock& iv_dblock,
    const zce_byte* input,
    size_t inputlen,
    const zce_byte* key,
    size_t keylen,
    const char* mode);

#endif // __zce_rsa_h__
