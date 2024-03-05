#pragma once
#include <zce/zce_config.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct { zce_int32 space[24]; } zce_md5_ctx;

    void ZCE_API zce_md5_init(zce_md5_ctx* pms);

    void ZCE_API zce_md5_update(zce_md5_ctx* pms, const zce_byte* data, zce_uint32 nbytes);

    void ZCE_API zce_md5_final(zce_byte digest[16], zce_md5_ctx* pms);

    /* RFC1071 Internet checksum*/
    unsigned short ZCE_API zce_checksum(const zce_byte* addr, unsigned count);

    ///////////////////////////////////////////////////////////////////////////////

    typedef struct { zce_uint32 space[24]; } zce_sha1_ctx;

    void ZCE_API zce_sha1_init(zce_sha1_ctx*);

    void ZCE_API zce_sha1_update(zce_sha1_ctx*, const zce_byte* data, zce_uint32 len);

    void ZCE_API zce_sha1_final(zce_byte digest[20], zce_sha1_ctx* context);

    void ZCE_API zce_sha1(zce_byte hash_out[20], const zce_byte* str, size_t len);

    zce_byte* ZCE_API zce_hmac_sha1(zce_byte out[20], const zce_byte* msg, size_t msglen, const zce_byte* secret, size_t secretlen);

    ///////////////////////////////////////////////////////////////////////////////

    typedef struct { zce_uint32 space[28]; } zce_sha256_ctx;

    void ZCE_API zce_sha256_init(zce_sha256_ctx* ctx);

    void ZCE_API zce_sha256_update(zce_sha256_ctx* ctx, const zce_byte data[], zce_uint32 len);

    void ZCE_API zce_sha256_final(zce_sha256_ctx* ctx, zce_byte hash[]);

    void ZCE_API zce_sha256(zce_byte hash_out[32], const zce_byte* str, zce_uint32 len);

    zce_byte* ZCE_API zce_hmac_sha256(zce_byte out[32], const zce_byte* msg, size_t msglen, const zce_byte* secret, size_t secretlen);


#ifdef __cplusplus
}  /* end extern "C" */
#endif
