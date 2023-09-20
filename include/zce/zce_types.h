// ***************************************************************
//  zce_types   version:  1.0   -  date: 6/22/2015
//  -------------------------------------------------------------
//  Yongming Wang(wym@zhidu.biz)
//  Zhidu Network Co. Ltd.
//  -------------------------------------------------------------
//  Copyright (C) 2015 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once
#ifndef __zce_types_h__
#define __zce_types_h__

#ifndef ZCE_TYPES_DEFINED
#define ZCE_TYPES_DEFINED

#ifdef _WIN32
    typedef DWORD       threadid_t;
    typedef HANDLE      handle_t;
    typedef int         socklen_t;
    typedef struct timespec timespec_t;
    typedef long long          int zce_int64;
    typedef unsigned long long int zce_uint64;
#else
    typedef pthread_t   threadid_t;
    typedef int         handle_t;
#if defined(__GNUC__) && (__WORDSIZE == 64)
    typedef long int          zce_int64;
    typedef unsigned long int zce_uint64;
#else
    typedef long long int          zce_int64;
    typedef unsigned long long int zce_uint64;
#endif

#endif

typedef char         zce_ntchars;
typedef char         zce_char;
typedef char         zce_int8;
typedef short        zce_int16;
typedef int          zce_int32;
typedef unsigned char       zce_byte;
typedef unsigned char       zce_uint8;
typedef unsigned short      zce_uint16;
typedef unsigned int        zce_uint32;
typedef float               zce_float;
typedef double              zce_double;
typedef int                 zce_serial;

#endif //ZCE_TYPES_DEFINED

//to simplify type cast or type safety
typedef union _zce_sockaddr_t
{
    struct sockaddr sa;
    struct sockaddr_in sa_in;
    struct sockaddr_in6 sa_in6;
    struct sockaddr_storage sa_stor;
} zce_sockaddr_t;

typedef struct _zce_addr_t
{
    zce_byte        translayer;
    zce_byte        ipclass;
    zce_sockaddr_t  addr;
} zce_addr_t;

#ifdef __cplusplus
#   include <string>
#   include <vector>
#   ifdef _UNICODE
    typedef std::wstring    zce_string;
    typedef wchar_t         zce_tchar;
#   else //_UNICODE
    typedef std::string     zce_string;
    typedef char            zce_tchar;
#   endif //_UNICODE
    typedef std::wstring    zce_wstring;
    typedef std::string     zce_astring;
    typedef std::vector<zce_byte> zce_bytevec;

    typedef zce_string zce_str;
    typedef zce_astring zce_astr;
    typedef zce_wstring zce_wstr;
    typedef std::vector<zce_string> zce_strvec;
    typedef std::vector<zce_astring> zce_astrvec;
    typedef std::vector<zce_wstring> zce_wstrvec;
#endif //__cplusplus

#endif // __zce_types_h__

