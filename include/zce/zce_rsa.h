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

class zce_dblock;

int zce_rsa_genkey(zce_dblock& pubkey, zce_dblock& prikey);

int zce_randbytes(zce_dblock& pubkey, int len);

int ZCE_API zce_rsa_public_encrypt(zce_dblock& dblock,
    const zce_byte* input,
    size_t inputlen,
    const char* key,
    size_t keylen);

int ZCE_API zce_rsa_public_verify(zce_dblock& dblock,
    const zce_byte* input,
    size_t inputlen,
    const char* key,
    size_t keylen);

int ZCE_API zce_rsa_private_sign(zce_dblock& dblock,
    const zce_byte* input,
    size_t inputlen,
    const char* key,
    size_t keylen);

int ZCE_API zce_rsa_private_decrypt(zce_dblock& dblock,
    const zce_byte* input,
    size_t inputlen,
    const char* key,
    size_t keylen);

int ZCE_API zce_aes_encrypt(zce_dblock& dblock,
    zce_dblock& iv_dblock,
    const zce_byte* input,
    size_t inputlen,
    const zce_byte* key,
    size_t keylen,
    const char* mode);

int ZCE_API zce_aes_decrypt(zce_dblock& dblock,
    zce_dblock& iv_dblock,
    const zce_byte* input,
    size_t inputlen,
    const zce_byte* key,
    size_t keylen,
    const char* mode);

#endif // __zce_rsa_h__
