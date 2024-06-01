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

