﻿#pragma once

#ifdef _WIN32
#   define NOMINMAX
#   if defined(_UNICODE)
#       //include <vld.h>
#       define ZCE_ZDB_MYSQL 0
#       define ZCE_ZDB_PGSQL 0
#       define ZCE_ISSERVICE_MODE  0
#       define ZCE_SUPPORT_PYVM 0
#       define ZCE_SUPPORT_LUAVM 0
#       define ZCE_SUPPORT_SSL 0
#   else
#       define ZCE_ZDB_MYSQL 0
#       define ZCE_ZDB_PGSQL 1
#       define ZCE_ISSERVICE_MODE  0
#       define ZCE_SUPPORT_PYVM  1
#       define ZCE_SUPPORT_LUAVM 1
#       define ZCE_SUPPORT_SSL 1
#   endif
#   include <winsock2.h>
#   include <ws2tcpip.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <stdint.h>
#   include <windows.h>
#   include <direct.h>
#   include <io.h>
#   include <sys/stat.h>
#   define ZDP_GEP
#elif defined(__APPLE__)
#   include <unistd.h>
#   include <fcntl.h>
#   include <sys/socket.h>
#   include <sys/poll.h>
#   include <netinet/in.h>
#   include <netinet/tcp.h>
#   include <arpa/inet.h>
#   include <netdb.h>
#   include <memory.h>
#   include <sys/types.h>
#   include <sys/wait.h>
#   include <libkern/OSAtomic.h>
#   include "TargetConditionals.h"
#   if TARGET_IPHONE_SIMULATOR
#   elif TARGET_OS_IPHONE
#   elif TARGET_OS_MAC
#       include <CoreServices/CoreServices.h>
#   else
#   endif
#   define SOCKET int
#   define SOCKET_ERROR -1
#   define INVALID_SOCKET -1
#else
#   include <arpa/inet.h>
#   include <fcntl.h>
#   include <memory.h>
#   include <netdb.h>
#   include <netinet/in.h>
#   include <netinet/tcp.h>
#   include <pthread.h>
#   include <stdarg.h>
#   include <stdlib.h>
#   include <string.h>
#   include <sys/poll.h>
#   include <sys/resource.h>
#   include <sys/socket.h>
#   include <sys/syscall.h>
#   include <sys/stat.h>
#   include <sys/time.h>
#   include <sys/types.h>
#   include <sys/wait.h>
#   include <unistd.h>
#   define SOCKET int
#   define SOCKET_ERROR -1
#   define INVALID_SOCKET -1
#   ifdef __ANDROID__
#       include <android/log.h>
#       include <jni.h>
#       define HASNOT_UUID 1
#       define ZCE_API __attribute__ ((visibility ("default")))
#   else
#       define ZCE_ZDB_MYSQL 1
#       define ZCE_ZDB_PGSQL 1
#       define ZCE_ISSERVICE_MODE  0
#       define ZCE_SUPPORT_PYVM  0
#       define ZCE_SUPPORT_LUAVM 1
#       define ZCE_SUPPORT_SSL 1
#   endif
#endif

#ifdef __cpluscplus
#   include <string>
#   if defined(__APPLE__)
#       include <cstddef>
#   endif //__APPLE__
#endif //__cpluscplus

#ifndef ZCE_API
#   define ZCE_API
#endif

#ifndef ZCE_OBJECT_NO_MONITOR
#   define ZCE_OBJECT_DECLARE              zce_object_counter_proxy object_proxy_
#   define ZCE_OBJECT_IMPLEMENT(x)         ,object_proxy_(x##_object_counter)
#   define ZCE_OBJECT_IMPLEMENT_EMPTY(x)   :object_proxy_(x##_object_counter)
#   define ZCE_OBJECT_INSTANCE(x)          static zce_object_counter x##_object_counter(#x);
#else
#   define ZCE_OBJECT_DECLARE 
#   define ZCE_OBJECT_IMPLEMENT(x)
#   define ZCE_OBJECT_IMPLEMENT_EMPTY(x)
#   define ZCE_OBJECT_INSTANCE(x)
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

enum ERV_ZCE_ERROR
{
    ZCE_ERROR_OK = 0,

    ZDB_SUCCE_COMMON = 0x2000000, //ZDB SUCCESS
    ZDB_SUCCE_MULTIRESPONSE,

    ZCE_ERROR_BASE = 0x81000000, //keep for stdc error

    ZCE_ERROR_COMMON = 0x81010000, //ZCE ERROR
    ZCE_ERROR_MALLOC,  //内存分配错误
    ZCE_ERROR_UNSUPPORT,//遇到未支持的功能要求
    ZCE_ERROR_SHRTLEN,  //数据比要求的太短
    ZCE_ERROR_EXCDLEN,  //数据比要求的太长
    ZCE_ERROR_CORRUPT,  //数据不一致
    ZCE_ERROR_SYNTAX,   //数据语义错误
    ZCE_ERROR_ZIP,      //压缩出错
    ZCE_ERROR_TIMEOUT,  //接收响应超时
    ZCE_ERROR_CONVERTOR,
    ZCE_ERROR_CLOSED,   //连接已经关闭
    ZCE_ERROR_TOCLOSE,  //连接将要关闭
    ZCE_ERROR_PREVNULL, //ISTEAM PREV NOT EXISTS
    ZCE_ERROR_OVERFLOW, //溢出，例如过大的数字
    ZCE_ERROR_INVALID,  //无效的参数

    ZCE_ERROR_UVBASE = 0x81020000,

    ZDB_ERROR_COMMON = 0x82000000, //ZDB ERROR
    ZDB_ERROR_CONNECTION,
    ZDB_ERROR_SQLINVALID,
    ZDB_ERROR_COLUMNOVERLOW,
    ZDB_ERROR_BINDERROR,
    ZDB_ERROR_PARAMCNT,
    ZDB_ERROR_FOPEN,
    ZDB_ERROR_FILEEXPAND, //文件增长错误
    ZDB_ERROR_TAGNOTEXISTS,

    ZDB_ERROR_SQLITE_COMMON = 0x82010000, //以下错误代码对应SQLITE自身的错误代码
    ZDB_ERROR_SQLITE_BACKUP_CONFILICT,
    ZDB_ERROR_SQLITE_BACKUP_FAILED,

    ZDB_ERROR_MYSQL_COMMON = 0x82020000, //以下错误代码对应MYSQL自身的错误代码
    ZDB_ERROR_PGSQL_COMMON = 0x82030000, //以下错误代码对应PGSQL自身的错误代码

    ZVM_SUCCESS_BASE = 0x03000000,
    ZVM_ERROR_BASE = 0x83000000, //ZVM ERROR
    ZVM_ERROR_INVALID, //无效的虚拟机
    ZVM_ERROR_INVALIDMETHOD, //无效的方法
    ZVM_ERROR_INVALIDPARAM, //无效的参数
    ZVM_ERROR_DISCONNECT, //连接已经断开
    ZVM_ERROR_INCONNECT, //已经在连接中

    ZUA_ERROR_BASE = 0x88010000,
    ZUA_ERROR_RPC_BASE = 0x88010100,
    ZUA_ERROR_RPC_SECURITY_DENYMETHOD,
    ZUA_ERROR_RPC_METHOD_NOTEXISTS,
    ZUA_ERROR_RPC_SVCNAME_NOTEXISTS,

    ZUA_ERROR_CACHE_BASE = 0x88010200,
    ZUA_ERROR_CACHE_REDIS_INVALIDCONN,
};

#include <zce/zce_types.h>
