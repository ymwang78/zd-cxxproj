#pragma once

#   define ZLIC_DECLSPEC_EXPORT
#   define ZLIC_DECLSPEC_IMPORT

#ifndef ZLIC_API
#   ifdef ZLIC_API_EXPORTS
#       define ZLIC_API ZLIC_DECLSPEC_EXPORT
#   else
#       define ZLIC_API ZLIC_DECLSPEC_IMPORT
#   endif
#endif

enum ZLIC_ERROR {
    ZLIC_ERROR_NOLICENSE = -1,
    ZLIC_ERROR_CORRUPT = -2,
    ZLIC_ERROR_EXPIRED = -3,
    ZLIC_ERROR_PRODUCT = -4,
    ZLIC_ERROR_MACHINE = -5,
    ZLIC_ERROR_WRITE = -6,
};

#define ZLIC_VERIFY					(2048)				/**< 校验授权信息 */
#define ZLIC_GET_LICENSENUM			(2049)				/**< 获取授权数 */
#define ZLIC_GET_EXPIRETIME			(2050)				/**< 获取过期时间 */
#define ZLIC_GET_BUFFER             (2051)				/**< 获取缓冲 */

#define ZLIC_FALSE				    (0)
#define ZLIC_TRUE					(1)

#ifdef __cplusplus
extern "C" {
#endif

    /*生成License请求串, 把这个串发给授权方生成License Key*/
    ZLIC_API const char* zlic_getreq(const char* app);

    /*将授权方生成的License Key写入，本API会在EXE所在目录尝试写入{app}.zlic文件*/
    ZLIC_API int zlic_setkey(const char* app, const char* keystr);

    /*校验License Key是否合法，第一次调用会尝试从{app}.zlic文件读入时间*/
    ZLIC_API int   zlic_valid(const char* app, int trival_expire);

    /*获取License Key中的属性值*/
    ZLIC_API const unsigned char* zlic_fetch(const char* property_name, int* nLen);

#ifdef __cplusplus
}
#endif
