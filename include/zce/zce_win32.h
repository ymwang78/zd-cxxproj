#pragma once
#ifdef _WIN32

#include <zce/zce_config.h>
#include <zce/zce_types.h>

#ifdef __cplusplus

std::string ZCE_API zce_win32_crypt_protect(zce_byte* data, unsigned len, LPWSTR desc);

std::vector<zce_byte> ZCE_API zce_win32_crypt_unprotect(LPWSTR* desc, zce_byte* data, unsigned len);

#endif

#endif
