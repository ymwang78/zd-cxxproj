#pragma once
// ***************************************************************
//  zce_types   version:  1.0   -  date: 2002/06/22
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

//fix defines in different platform
#ifndef ZCE_TYPES_DEFINED
#define ZCE_TYPES_DEFINED


#   ifdef _WIN32

#   if !defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED)
        typedef intptr_t ssize_t;
#       define _SSIZE_T_
#       define _SSIZE_T_DEFINED
#   endif

        typedef DWORD threadid_t;
	    typedef HANDLE handle_t;
	    typedef int socklen_t;
	    struct iovec {
		    u_long iov_len;
		    char* iov_base;
	    };
#   else
	    typedef pthread_t   threadid_t;
	    typedef int         handle_t;
#   endif

#endif //ZCE_TYPES_DEFINED

#ifdef _WIN32
#   define ZCE_INVALID_HANDLE INVALID_HANDLE_VALUE
	typedef long long int zce_int64;
	typedef unsigned long long int zce_uint64;
#else
#   define ZCE_INVALID_HANDLE -1
#   if defined(__GNUC__) && (__WORDSIZE == 64)
        typedef long int zce_int64;
        typedef unsigned long int zce_uint64;
#   else
        typedef long long int zce_int64;
        typedef unsigned long long int zce_uint64;
#   endif
#endif

typedef char    zce_char;
typedef char    zce_int8;
typedef short   zce_int16;
typedef int     zce_int32;
typedef unsigned char   zce_byte;
typedef unsigned char   zce_uint8;
typedef unsigned short  zce_uint16;
typedef unsigned int    zce_uint32;
typedef float           zce_float;
typedef double          zce_double;
typedef struct timespec zce_timespec_t;
typedef zce_int64       zce_timestamp;  //same as pgsql timestamp, microsecond from 2000-1-1 00:00:00

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
        typedef std::wstring zce_tstring;
        typedef wchar_t      zce_tchar;
#   else //_UNICODE
        typedef std::string  zce_tstring;
        typedef char         zce_tchar;
#   endif //_UNICODE

    typedef std::string  zce_string;
	typedef std::string  zce_astring;
    typedef std::wstring zce_wstring;
	typedef std::vector<zce_byte>    zce_bytevec;
	typedef std::vector<zce_string>  zce_strvec;
	typedef std::vector<zce_astring> zce_astrvec;
    typedef std::vector<zce_tstring> zce_tstrvec;
    typedef std::vector<zce_wstring> zce_wstrvec;
#endif //__cplusplus
