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

class zce::RefBlock;

int zce_rsa_genkey(zce::RefBlock& pubkey, zce::RefBlock& prikey);

int zce_randbytes(zce::RefBlock& pubkey, int len);

int ZCE_API zce_rsa_public_encrypt(zce::RefBlock& dblock,
    const zce_byte* input,
    size_t inputlen,
    const char* key,
    size_t keylen);

int ZCE_API zce_rsa_public_verify(zce::RefBlock& dblock,
    const zce_byte* input,
    size_t inputlen,
    const char* key,
    size_t keylen);

int ZCE_API zce_rsa_private_sign(zce::RefBlock& dblock,
    const zce_byte* input,
    size_t inputlen,
    const char* key,
    size_t keylen);

int ZCE_API zce_rsa_private_decrypt(zce::RefBlock& dblock,
    const zce_byte* input,
    size_t inputlen,
    const char* key,
    size_t keylen);

int ZCE_API zce_aes_encrypt(zce::RefBlock& dblock,
    zce::RefBlock& iv_dblock,
    const zce_byte* input,
    size_t inputlen,
    const zce_byte* key,
    size_t keylen,
    const char* mode);

int ZCE_API zce_aes_decrypt(zce::RefBlock& dblock,
    const zce::RefBlock& iv_dblock,
    const zce_byte* input,
    size_t inputlen,
    const zce_byte* key,
    size_t keylen,
    const char* mode);

#endif // __zce_rsa_h__
