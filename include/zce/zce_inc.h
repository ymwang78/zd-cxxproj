﻿#pragma once

#ifdef _WIN32

#    define NOMINMAX
#    if defined(_UNICODE)
#        //include <vld.h>
#        define ZCE_ZDB_MONGODB 0
#        define ZCE_ZDB_MYSQL 0
#        define ZCE_ZDB_PGSQL 0
#        define ZCE_ZDB_REDIS 0
#        define ZCE_SUPPORT_PYVM 0
#        define ZCE_SUPPORT_LUAVM 0
#        define ZCE_SUPPORT_CCVM 0
#        define ZCE_SUPPORT_SSL 0
#    else
#        define ZCE_ZDB_MONGODB 0
#        define ZCE_ZDB_MYSQL 0
#        define ZCE_ZDB_PGSQL 1
#        define ZCE_ZDB_REDIS 1
#        define ZCE_SUPPORT_PYVM 1
#        define ZCE_SUPPORT_LUAVM 1
#        define ZCE_SUPPORT_CCVM 1
#        define ZCE_SUPPORT_SSL 1
#    endif

#    include <winsock2.h>
#    include <ws2tcpip.h>
#    include <stdio.h>
#    include <stdlib.h>
#    include <stdint.h>
#    include <windows.h>
#    include <direct.h>
#    include <io.h>
#    include <sys/stat.h>
#    define ZDP_GEP
#elif defined(__APPLE__)
#    include <unistd.h>
#    include <fcntl.h>
#    include <sys/socket.h>
#    include <sys/poll.h>
#    include <netinet/in.h>
#    include <netinet/tcp.h>
#    include <arpa/inet.h>
#    include <netdb.h>
#    include <memory.h>
#    include <sys/types.h>
#    include <sys/wait.h>
#    include <libkern/OSAtomic.h>
#    include "TargetConditionals.h"
#    if TARGET_IPHONE_SIMULATOR
#    elif TARGET_OS_IPHONE
#    elif TARGET_OS_MAC
#        include <CoreServices/CoreServices.h>
#    else
#    endif
#    define SOCKET int
#    define SOCKET_ERROR -1
#    define INVALID_SOCKET -1
#else
#    include <arpa/inet.h>
#    include <fcntl.h>
#    include <memory.h>
#    include <netdb.h>
#    include <netinet/in.h>
#    include <netinet/tcp.h>
#    include <pthread.h>
#    include <stdarg.h>
#    include <stdlib.h>
#    include <string.h>
#    include <sys/poll.h>
#    include <sys/resource.h>
#    include <sys/socket.h>
#    include <sys/syscall.h>
#    include <sys/stat.h>
#    include <sys/time.h>
#    include <sys/types.h>
#    include <sys/wait.h>
#    include <unistd.h>
#    define SOCKET int
#    define SOCKET_ERROR -1
#    define INVALID_SOCKET -1
#    ifdef __ANDROID__
#        include <android/log.h>
#        include <jni.h>
#        define HASNOT_UUID 1
#        define ZCE_API __attribute__((visibility("default")))
#    else
#        define ZCE_ZDB_MONGODB 0
#        define ZCE_ZDB_MYSQL 0
#        define ZCE_ZDB_PGSQL 1
#        define ZCE_ZDB_REDIS 1
#        define ZCE_SUPPORT_PYVM 1
#        define ZCE_SUPPORT_LUAVM 1
#        define ZCE_SUPPORT_CCVM 1
#        define ZCE_SUPPORT_SSL 1
#    endif
#endif

#if defined(_WIN32)
#    if defined(_WIN64)
#        define FORCE_UNDEFINED_SYMBOL(x) __pragma(comment(linker, "/include:" #x))
#    else
#        define FORCE_UNDEFINED_SYMBOL(x) __pragma(comment(linker, "/include:_" #x))
#    endif
#else
#    define FORCE_UNDEFINED_SYMBOL(x) \
        extern "C" void x(void);      \
        void (*__##x##_fp)(void) = &x;
#endif

#ifdef __cpluscplus
#    include <string>
#    if defined(__APPLE__)
#        include <cstddef>
#    endif  //__APPLE__
#    include <gsl/gsl>
#endif  //__cpluscplus

#ifndef ZCE_API
#    define ZCE_API
#endif

#ifndef ZCE_OBJECT_NO_MONITOR
#    define ZCE_OBJECT_DECLARE zce_object_counter_proxy object_proxy_
#    define ZCE_OBJECT_IMPLEMENT(x) , object_proxy_(x##_object_counter)
#    define ZCE_OBJECT_IMPLEMENT_EMPTY(x)   :object_proxy_(x##_object_counter)
#    define ZCE_OBJECT_INSTANCE(x) static zce_object_counter x##_object_counter(#x);
#else
#    define ZCE_OBJECT_DECLARE
#    define ZCE_OBJECT_IMPLEMENT(x)
#    define ZCE_OBJECT_IMPLEMENT_EMPTY(x)
#    define ZCE_OBJECT_INSTANCE(x)
#endif

#ifdef _WIN32
#    define ZCE_PATH_SEPERATOR '\\'
#    define ZCE_SYSPATH_SEPERATOR ";"
#else
#    define ZCE_PATH_SEPERATOR '/'
#    define ZCE_SYSPATH_SEPERATOR ":"
#endif
#define ZCE_WIDE_STRING_HELPER(str) L##str
#define ZCE_WIDE_STRING(str) ZCE_WIDE_STRING_HELPER(str)

enum ERV_ZCE_ERROR {
    ZCE_ERROR_OK = 0,

    ZDB_SUCCE_COMMON = 0x2000000,       // ZDB SUCCESS
    ZDB_SUCCE_MULTIRESPONSE,

    ZCE_ERROR_BASE = 0x81000000,        // keep for stdc error

    ZCE_ERROR_COMMON = 0x81010000,      // ZCE ERROR
    ZCE_ERROR_MALLOC = 0x81010001,      // 内存分配错误
    ZCE_ERROR_UNSUPPORT = 0x81010002,   // 遇到未支持的功能要求
    ZCE_ERROR_SHRTLEN = 0x81010003,     // 数据比要求的太短
    ZCE_ERROR_EXCDLEN = 0x81010004,     // 数据比要求的太长
    ZCE_ERROR_CORRUPT = 0x81010005,     // 数据不一致
    ZCE_ERROR_SYNTAX = 0x81010006,      // 数据语义错误
    ZCE_ERROR_ZIP = 0x81010007,         // 压缩出错
    ZCE_ERROR_TIMEOUT = 0x81010008,     // 接收响应超时
    ZCE_ERROR_CONVERTOR = 0x81010009,   // 转换错误
    ZCE_ERROR_CLOSED = 0x8101000a,      // 连接已经关闭
    ZCE_ERROR_TOCLOSE = 0x8101000b,     // 连接将要关闭
    ZCE_ERROR_PREVNULL = 0x8101000c,    // ISTEAM PREV NOT EXISTS
    ZCE_ERROR_OVERFLOW = 0x8101000d,    // 溢出，例如过大的数字
    ZCE_ERROR_INVALID = 0x8101000e,     // 无效的参数
    ZCE_ERROR_DUPLICATED = 0x8101000f,  // 重复的数据
    ZCE_ERROR_UNINIT = 0x81010010,      // 未初始化
    ZCE_ERROR_BADPRC = 0x81010011,      // 处理错误
    ZCE_ERROR_NORESOURCE = 0x81010012,  // 资源不足
    ZCE_ERROR_VERSION = 0x81010013,     // 版本不匹配
    ZCE_ERROR_READ = 0x81010014,        // 读取文件网络等失败

    ZCE_ERROR_UVBASE = 0x81020000,

    ZDB_ERROR_COMMON = 0x82000000,          // ZDB ERROR
    ZDB_ERROR_CONNECTION,
    ZDB_ERROR_SQLINVALID,
    ZDB_ERROR_COLUMNOVERLOW,
    ZDB_ERROR_BINDERROR,
    ZDB_ERROR_PARAMCNT,
    ZDB_ERROR_FOPEN,
    ZDB_ERROR_FILEEXPAND,                   // 文件增长错误
    ZDB_ERROR_TAGNOTEXISTS,

    ZDB_ERROR_SQLITE_COMMON = 0x82010000,  // 以下错误代码对应SQLITE自身的错误代码
    ZDB_ERROR_SQLITE_BACKUP_CONFILICT,
    ZDB_ERROR_SQLITE_BACKUP_FAILED,

    ZDB_ERROR_MYSQL_COMMON = 0x82020000,  // 以下错误代码对应MYSQL自身的错误代码
    ZDB_ERROR_PGSQL_COMMON = 0x82030000,  // 以下错误代码对应PGSQL自身的错误代码
    ZDB_ERROR_REDIS_COMMON = 0x82040000,  // 以下错误代码对应REDIS自身的错误代码

    ZVM_SUCCESS_BASE = 0x02100000,
    ZVM_ERROR_BASE = 0x82100000,  // ZVM ERROR
    ZVM_ERROR_INVALID,            // 无效的虚拟机
    ZVM_ERROR_INVALIDMETHOD,      // 无效的方法
    ZVM_ERROR_INVALIDPARAM,       // 无效的参数
    ZVM_ERROR_DISCONNECT,         // 连接已经断开
    ZVM_ERROR_INCONNECT,          // 已经在连接中

    // 登记已知库的错误代码范围
    ZAP_ERROR_BASE = 0x83000000,  // ZAP ERROR
    ZAP_ERROR_COIN = 0x83010000,  // COIN ERROR
    ZAP_ERROR_ZMPC = 0x83020000,  // ZMPC ERROR
};

#include <zce/zce_types.h>
