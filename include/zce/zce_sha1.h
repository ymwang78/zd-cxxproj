#pragma once
#ifndef __ZCE_SHA1_H__
#define __ZCE_SHA1_H__

#include <zce/zce_config.h>

typedef struct _SHA1_CTX zce_sha1_ctx;

void ZCE_API zce_sha1_init(zce_sha1_ctx*);

void ZCE_API zce_sha1_update(zce_sha1_ctx*,
    const unsigned char *data,
    uint32_t len);

void ZCE_API zce_sha1_final(unsigned char digest[20],
    zce_sha1_ctx * context);

void ZCE_API zce_sha1(unsigned char hash_out[20],
    const unsigned char *str,
    size_t len);

void ZCE_API zce_hmac_sha1(unsigned char out[20],
    const unsigned char* msg,
    size_t msglen,
    const unsigned char* secret,
    size_t secretlen);

///////////////////////////////////////////////////////////////////////////////

/****************************** MACROS ******************************/
#define SHA256_DIGEST_SIZE 32            // SHA256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
typedef struct SHA256_CTX zce_sha256_ctx;

/*********************** FUNCTION DECLARATIONS **********************/
void ZCE_API zce_sha256_init(zce_sha256_ctx *ctx);

void ZCE_API zce_sha256_update(zce_sha256_ctx *ctx, const unsigned char data[], size_t len);

void ZCE_API zce_sha256_final(zce_sha256_ctx *ctx, unsigned char hash[]);

void ZCE_API zce_sha256(unsigned char hash_out[SHA256_DIGEST_SIZE],
    const unsigned char *str,
    size_t len);

void ZCE_API zce_hmac_sha256(unsigned char out[SHA256_DIGEST_SIZE],
    const unsigned char* msg,
    size_t msglen,
    const unsigned char* secret,
    size_t secretlen);

#endif /* __ZCE_SHA1_H__ */

