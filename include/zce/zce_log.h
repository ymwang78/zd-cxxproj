#pragma once
// ***************************************************************
//  zce_log   version:  1.0   -  date: 2002/11/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

#include <zce/zce_inc.h>
#include <stdarg.h>

#ifndef _WIN32
#    define __cdecl
#endif

#ifdef __cplusplus
#    include <iomanip>
extern "C" {
#endif

#ifndef ZCE_DEFINED_LOGLEVEL
typedef enum _zlog_level {
    ZLOG_TRACE = 0,   /* trace */
    ZLOG_DEBUG = 1,   /* debug */
    ZLOG_INFOR = 2,   /* info */
    ZLOG_WARNI = 3,   /* warn */
    ZLOG_ERROR = 4,   /* error */
    ZLOG_FATAL = 5,   /* fatal */
    ZLOG_BIZDT = 6,   /* bizdata */
    ZLOG_NONEL = 7,   /* none */
} ZLOG_LEVEL;
#    define ZCE_DEFINED_LOGLEVEL
#endif

typedef struct zlog_param {
    const char* name;
    int max_filesize;
    const char* logpath;
    const char* prefix;
} zlog_param;

typedef struct _zlog_handle {
    int unused;
}* zlog_handle;

zlog_handle ZCE_API zlog_init(zlog_handle sharehandle, zlog_param* param);

void ZCE_API zlog_fini();

void ZCE_API zlog_setlevel(unsigned level);

unsigned ZCE_API zlog_getlevel();

unsigned ZCE_API zlog_getseq();

int ZCE_API zlog_snprintf(char* buffer, unsigned size, unsigned level, const char* funcname,
                          const char* filename, int linenumber, const char* fmt, ...);

int ZCE_API zlog_vsnprintf(char* buffer, unsigned size, unsigned level, const char* funcname,
                           const char* filename, int linenumber, const char* fmt, va_list argptr);

void ZCE_API zlog_logv(unsigned level, const char* funcname, const char* filename, int linenumber,
                       const char* fmt, ...);

void ZCE_API zlog_vlog(unsigned level, const char* funcname, const char* filename, int linenumber,
                       const char* fmt, va_list argptr);

void ZCE_API zlog_bizremote(const char* ip, unsigned short port);

void ZCE_API zlog_setremote(const char* ip, unsigned short port);

void ZCE_API zlog_rawprint(unsigned level, const char* fmt);

void ZCE_API zlog_cleanup(int keep_days);

#ifdef _WIN32
#    define ZLOG(lv, fn, ...)                                                         \
        do {                                                                          \
            if (zlog_getlevel() <= (unsigned)(lv))                                              \
                zlog_logv((lv), __FUNCTION__, __FILE__, __LINE__, (fn), __VA_ARGS__); \
        } while (0)
#    define ZLOG_SYSCALL(lv, s) ZLOG((lv), "%s: os error: %d", (s), GetLastError())
#    define ZLOG_CEXCEPTION(lv, s, c) ZLOG((lv), "%s: seh, code = 0x%08x", (s), (c))
#else
#    define ZLOG(lv, fn, ...)                                                           \
        do {                                                                            \
            if (zlog_getlevel() <= (lv)) {                                              \
                zlog_logv((lv), __FUNCTION__, __FILE__, __LINE__, (fn), ##__VA_ARGS__); \
            }                                                                           \
        } while (0)
#    define ZLOG_SYSCALL(lv, s) ZLOG((lv), "%s: os errno: %d", (s), errno)
#endif

#define ZCE_TRACE ZCE_DEBUG((ZLOG_TRACE, "(tick:%lld)", zce_tick()))
#define ZCE_TRACE_OID ZCE_DEBUG((ZLOG_TRACE, "oid:%llx tick:%lld", __get_oid(), zce_tick()))
#define ZCE_ASSERT(X)                                                                              \
    do {                                                                                           \
        if (!(X)) ZLOG(ZLOG_FATAL, "{%s:%d} assertion failed for '%s'", __FILE__, __LINE__, (#X)); \
    } while (0)
#define ZCE_ASSERT_TEXT(X, Y)                                                                     \
    do {                                                                                          \
        if (!(X)) ZLOG(ZLOG_FATAL, "{%s:%d} assertion failed for '%s'", __FILE__, __LINE__, (Y)); \
    } while (0)
#define ZCE_ASSERT_RETURN(X, Y)                                                              \
    do {                                                                                     \
        if (!(X)) {                                                                          \
            ZLOG(ZLOG_FATAL, "{%s:%d} assertion failed for '%s'", __FILE__, __LINE__, (#X)); \
            return Y;                                                                        \
        }                                                                                    \
    } while (0)

// #   define ZCE_ASSERT(X)                do{ if(!(X)) zlog_logv(ZLOG_FATAL, __PRETTY_FUNCTION__,
// __FILE__, __LINE__, "assertion failed for '%s'.\n", (#X)); } while(0)

#define ZCE_DEBUG(X) ZLOG X
#define ZCE_ERROR(X) ZLOG X

#define ZLOG_ICEEXCEPTION(lv, ex, s)                                                             \
    ZLOG((lv), "%s: ice exception: name = %s, file = %s, line = %d", (s), ex.ice_name().c_str(), \
         ex.ice_file(), ex.ice_line())
#define ZLOG_CPPEXCEPTION(lv, s) ZLOG((lv), "%s: unknown exception", (s))

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#    include <zce/zce_object.h>
#    include <cstddef>
#    include <cstdint>
#    include <sstream>
#    include <vector>
#    include <functional>

class zce_loglevel {
  public:
    zce_loglevel(unsigned level, const char* filename);
};
#    define ZCE_CURRENT_LOGLEVEL(x) static zce_loglevel __loglevel(x, __FILE__)

template <typename T>
std::ostream& operator<<(std::ostream& o, const ::std::vector<T>& t) {
    o << t.size() << "[";
    for (auto iter = t.begin(); iter != t.end(); ++iter) {
        o << (*iter) << "|";
    }
    o << "]";
    return o;
}

template <typename T>
struct hex_t {
    T v;
    hex_t(T iv) : v(iv) {}
};

template <typename T>
std::ostream& operator<<(std::ostream& ost, const hex_t<T>& t) {
    ost << std::hex << t.v << std::dec;
    return ost;
}

namespace zce {

class ZCE_API Logger : public zce::Object {
  private:
    _zlog_level level_;
    int line_;
    std::string func_;
    std::string fname_;
    std::string splitter_;
    std::ostringstream oss_;

  public:
    Logger(_zlog_level level, const char* func, const char* fname, int line,
           const char* splitter = "|");

    void flush();

    std::ostringstream& oss() { return oss_; }

    template <typename T>
    void log_args(std::ostream& ost, const T& t) {
        ost << t;
        if (!splitter_.empty()) ost << splitter_;
    }

    void log_args(std::ostream& ost, float t) {
        ost << std::setprecision(8) << t;
        if (!splitter_.empty()) ost << splitter_;
    }

    void log_args(std::ostream& ost, double t) {
        ost << std::setprecision(16) << t;
        if (!splitter_.empty()) ost << splitter_;
    }

    void log_args(std::ostream& ost, const char* t) {
        if (t == 0) {
            ost << "null";
        } else {
            ost << t;
        }
        if (!splitter_.empty()) ost << splitter_;
    }

    void log_args(std::ostream& ost, const wchar_t* t);

    template <size_t N>
    void log_args(std::ostream& ost, const wchar_t (&t)[N]) {
        log_args(ost, static_cast<const wchar_t*>(t));
    }

    void log_args(std::ostream& ost, const std::wstring& t);

    void log_args(std::ostream& ost, const zce_sockaddr_t& t);

    template <typename T, typename... Args>
    void log_args(std::ostream& ost, const T& t, const Args&... args) {
        log_args(ost, t);
        log_args(ost, args...);
    }

    template <typename... Args>
    void write(Args... args) {
        log_args(oss_, args...);
        flush();
    }

    static void setCallback(
        std::function<void(unsigned level, const char* msg, size_t len)>&& callback);
};

}  // namespace zce

#    define ZTRACE(...)                                                                       \
        do {                                                                                  \
            if (zlog_getlevel() <= ZLOG_TRACE)                                                \
                zce::Logger(ZLOG_TRACE, __FUNCTION__, __FILE__, __LINE__).write(__VA_ARGS__); \
        } while (0)
#    define ZDEBUG(...)                                                                       \
        do {                                                                                  \
            if (zlog_getlevel() <= ZLOG_DEBUG)                                                \
                zce::Logger(ZLOG_DEBUG, __FUNCTION__, __FILE__, __LINE__).write(__VA_ARGS__); \
        } while (0)
#    define ZINFOR(...)                                                                       \
        do {                                                                                  \
            if (zlog_getlevel() <= ZLOG_INFOR)                                                \
                zce::Logger(ZLOG_INFOR, __FUNCTION__, __FILE__, __LINE__).write(__VA_ARGS__); \
        } while (0)
#    define ZWARNI(...)                                                                       \
        do {                                                                                  \
            if (zlog_getlevel() <= ZLOG_WARNI)                                                \
                zce::Logger(ZLOG_WARNI, __FUNCTION__, __FILE__, __LINE__).write(__VA_ARGS__); \
        } while (0)
#    define ZERROR(...)                                                                       \
        do {                                                                                  \
            if (zlog_getlevel() <= ZLOG_ERROR)                                                \
                zce::Logger(ZLOG_ERROR, __FUNCTION__, __FILE__, __LINE__).write(__VA_ARGS__); \
        } while (0)
#    define ZFATAL(...)                                                                       \
        do {                                                                                  \
            if (zlog_getlevel() <= ZLOG_FATAL)                                                \
                zce::Logger(ZLOG_FATAL, __FUNCTION__, __FILE__, __LINE__).write(__VA_ARGS__); \
        } while (0)

#    define ZBIZDT(...)
// #define ZBIZDT(...) do { if(zlog_getlevel() <= ZLOG_BIZDT) zce::Logger(ZLOG_BIZDT, __FUNCTION__,
// __FILE__, __LINE__).write(__VA_ARGS__); } while(0)

// LogCollector UDP 协议 v1：固定头 2 字节。
// 本协议头故意不使用 ZDS（ZDS 帧开销会破坏“固定头仅 2 字节”
// 的目标），采用手写 mask/shift 位域。
namespace logcollector {

constexpr uint8_t kProtocolVersion = 1;
constexpr size_t kHeaderSize = 2;
constexpr uint8_t kMaxAppLen = 15;  // app_len 占 4 bit
constexpr uint8_t kLevelCount = 8;  // level 占 3 bit，对应 8 个 ZLOG_LEVEL

constexpr uint8_t kVersionMask = 0xC0;
constexpr uint8_t kLevelMask = 0x38;
constexpr uint8_t kCompressMask = 0x06;
constexpr uint8_t kTypeMask = 0x01;
constexpr uint8_t kAppLenMask = 0xF0;

constexpr uint8_t kCompressNone = 0;
constexpr uint8_t kCompressZlib = 1;  // v1 预留，未实现
constexpr uint8_t kCompressBz2 = 2;   // v1 预留，未实现

constexpr uint8_t kTypeLog = 0;      // 普通日志内容
constexpr uint8_t kTypeCommand = 1;  // 交互 Command，v1 暂不处理

inline uint8_t getVersion(uint8_t flag) { return (flag >> 6) & 0x03; }
inline uint8_t getLevel(uint8_t flag) { return (flag >> 3) & 0x07; }
inline uint8_t getCompress(uint8_t flag) { return (flag >> 1) & 0x03; }
inline uint8_t getType(uint8_t flag) { return flag & 0x01; }
inline uint8_t getAppLen(uint8_t ext) { return (ext >> 4) & 0x0F; }

inline uint8_t makeFlag(uint8_t version, uint8_t level, uint8_t compress, uint8_t type) {
    return static_cast<uint8_t>(((version & 0x03) << 6) | ((level & 0x07) << 3) |
                                ((compress & 0x03) << 1) | (type & 0x01));
}

inline uint8_t makeExt(uint8_t app_len) {
    return static_cast<uint8_t>((app_len & 0x0F) << 4);
}

enum class ParseStatus {
    Ok,
    TooShort,
    BadVersion,
    BadCompress,
    Command,
};

struct ParsedPacket {
    uint8_t version = 0;
    uint8_t level = 0;
    uint8_t compress = 0;
    uint8_t type = 0;
    const char* app = nullptr;
    uint8_t app_len = 0;
    const char* content = nullptr;
    uint32_t content_len = 0;
};

ParseStatus parseLogPacket(const uint8_t* buf, size_t len, ParsedPacket& out);

}  // namespace logcollector

#endif
