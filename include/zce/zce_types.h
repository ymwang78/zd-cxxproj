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
#include <stdint.h>
#include <stddef.h>  // size_t, ptrdiff_t
#include <time.h>    // struct timespec

#ifdef _WIN32
#    include <WinSock2.h>
#    include <Windows.h>
#    include <ws2tcpip.h>
#    if defined(_MSC_VER)
#        include <BaseTsd.h>
#        ifndef _SSIZE_T_DEFINED
typedef SSIZE_T ssize_t;
#            define _SSIZE_T_DEFINED
#        endif
#    endif
typedef struct iovec {
    unsigned long iov_len; /* Length of data.  */
    char* iov_base;        /* Pointer to data.  */
} iovec;
typedef DWORD threadid_t;
typedef HANDLE handle_t;
#    define ZCE_INVALID_HANDLE INVALID_HANDLE_VALUE
#else  // Linux / Unix / macOS
#    include <sys/types.h>
#    include <sys/socket.h>
#    include <sys/uio.h>
#    include <netinet/in.h>
#    include <arpa/inet.h>
#    include <pthread.h>
#    include <unistd.h>
#    define ZCE_INVALID_HANDLE -1
typedef pthread_t threadid_t;
typedef int handle_t;
#endif
typedef int8_t zce_int8;
typedef uint8_t zce_uint8;
typedef int16_t zce_int16;
typedef uint16_t zce_uint16;
typedef int32_t zce_int32;
typedef uint32_t zce_uint32;
typedef int64_t zce_int64;
typedef uint64_t zce_uint64;
typedef unsigned char zce_byte;
typedef char zce_char;
typedef float zce_float;
typedef double zce_double;
typedef struct timespec zce_timespec_t;
typedef zce_int64 zce_timestamp;
typedef struct zce_sockaddr_pipe {
    zce_uint16 family;
    zce_uint16 flags; /*bit0: 0server,1 client;*/
    unsigned char path[96];
} zce_sockaddr_pipe;

typedef union zce_sockaddr_t {
    struct sockaddr sa;
    struct sockaddr_in sa_in;
    struct sockaddr_in6 sa_in6;
    struct sockaddr_storage sa_stor;
    struct zce_sockaddr_pipe sa_pipe;
} zce_sockaddr_t;

typedef struct zce_addr_t {
    zce_byte translayer;
    zce_byte ipclass;
    zce_sockaddr_t addr;
} zce_addr_t;

#ifdef __cplusplus

#    include <string>
#    include <vector>
#    include <string_view>
#    include <memory>
#    include <optional>
#    if __has_include(<zce/zce_matrix.h>)
#        include <zce/zce_matrix.h>
#    endif

#    ifdef _UNICODE
using zce_tstring = std::u16string;
using zce_tchar = char16_t;
#    else
using zce_tstring = std::string;
using zce_tchar = char;
#    endif

using zce_char16 = char16_t;
using zce_ustr = std::u16string;
using zce_ustrview = std::u16string_view;
using zce_string = std::string;
using zce_astring = std::string;

using zce_bytevec = std::vector<zce_byte>;
using zce_ustrvec = std::vector<zce_ustr>;
using zce_strvec = std::vector<zce_string>;
using zce_astrvec = std::vector<zce_astring>;
using zce_tstrvec = std::vector<zce_tstring>;

namespace zce {
class Allocator;
class Object;
class ObjectCounter;
class Reactor;
class RefBlock;
class TaskQueue;
class Timer;
class TimerDoozer;
class Tss;

namespace zdp {
struct zds_context_t;
}
}  // namespace zce

#endif  // __cplusplus