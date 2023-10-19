#ifndef __zce_log_h__
#define __zce_log_h__

#include <zce/zce_inc.h>
#include <stdarg.h>

#ifndef _WIN32
#define __cdecl 
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum _zlog_level
{
    ZLOG_TRACE = 0,
    ZLOG_DEBUG = 1,            /* debug */
    ZLOG_INFOR = 2,            /* info */
    ZLOG_WARNI = 3,            /* warn */
    ZLOG_ERROR = 4,            /* error */
    ZLOG_FATAL = 5,            /* fatal */
    ZLOG_BIZDT = 100,          /* bizdata */
    ZLOG_NONEL = 255,          /* none */
};

typedef struct zlog_param {
    const char* name;
    int max_filesize;
    const char* logpath;
}zlog_param;

typedef struct _zlog_handle {
    int unused;
}*zlog_handle;

zlog_handle ZCE_API zlog_init(zlog_handle sharehandle, zlog_param* param);

void ZCE_API        zlog_fini();

void ZCE_API        zlog_setlevel(unsigned level);

unsigned ZCE_API    zlog_getlevel();

int ZCE_API         zlog_snprintf(char* buffer, unsigned size, unsigned level, const char* funcname, const char* filename, int linenumber, const char* fmt, ...);

int ZCE_API         zlog_vsnprintf(char* buffer, unsigned size, unsigned level, const char* funcname, const char* filename, int linenumber, const char* fmt, va_list argptr);

void ZCE_API        zlog_logv(unsigned level, const char* funcname, const char* filename, int linenumber, const char* fmt, ...);

void ZCE_API        zlog_vlog(unsigned level, const char* funcname, const char* filename, int linenumber, const char* fmt, va_list argptr);

void ZCE_API        zlog_bizremote(const char* ip, unsigned short port);

void ZCE_API        zlog_setremote(const char* ip, unsigned short port);

void ZCE_API        zlog_rawprint(unsigned level, const char* fmt);

#ifdef _WIN32
#   define ZLOG(lv, fn, ...)            do { if(zlog_getlevel() <= (lv)) zlog_logv((lv), __FUNCTION__, __FILE__, __LINE__, (fn), __VA_ARGS__); } while (0)
#   define ZLOG_SYSCALL(lv, s)          ZLOG((lv), "%s: os error: %d", (s), GetLastError())
#   define ZLOG_CEXCEPTION(lv, s, c)    ZLOG((lv), "%s: seh, code = 0x%08x", (s), (c))
#else
#   define ZLOG(lv, fn, ...)            do { if(zlog_getlevel() <= (lv)) zlog_logv((lv), __FUNCTION__, __FILE__, __LINE__, (fn), ##__VA_ARGS__); } while (0)
#   define ZLOG_SYSCALL(lv, s)          ZLOG((lv), "%s: os errno: %d", (s), errno)
#endif

#define ZCE_TRACE                       ZCE_DEBUG((ZLOG_TRACE, "(tick:%lld)", zce_tick()))
#define ZCE_TRACE_OID                   ZCE_DEBUG((ZLOG_TRACE, "oid:%llx tick:%lld", __get_oid(), zce_tick()))
#define ZCE_ASSERT(X)                   do{ if(!(X)) ZLOG(ZLOG_FATAL, "{%s:%d} assertion failed for '%s'", __FILE__, __LINE__, (#X)); } while(0)
#define ZCE_ASSERT_TEXT(X, Y)           do{ if(!(X)) ZLOG(ZLOG_FATAL, "{%s:%d} assertion failed for '%s'", __FILE__, __LINE__, (Y)); } while(0)
#define ZCE_ASSERT_RETURN(X, Y)         do{ if(!(X)) { ZLOG(ZLOG_FATAL, "{%s:%d} assertion failed for '%s'", __FILE__, __LINE__, (#X)); return Y; } } while(0)

//#   define ZCE_ASSERT(X)                do{ if(!(X)) zlog_logv(ZLOG_FATAL, __PRETTY_FUNCTION__, __FILE__, __LINE__, "assertion failed for '%s'.\n", (#X)); } while(0)

#define ZCE_DEBUG(X)                    ZLOG X
#define ZCE_ERROR(X)                    ZLOG X

#define ZLOG_ICEEXCEPTION(lv, ex, s)    ZLOG((lv), "%s: ice exception: name = %s, file = %s, line = %d", (s), ex.ice_name().c_str(), ex.ice_file(), ex.ice_line())
#define ZLOG_CPPEXCEPTION(lv, s)        ZLOG((lv), "%s: unknown exception", (s))

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

#include <zce/zce_smartptr.h>
#include <sstream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& o, const ::std::vector<T>& t) {
    o << t.size() << "[";
    for (auto iter = t.begin(); iter != t.end(); ++iter) {
        o << (*iter) << "|";
    }
    o << "]";
    return o;
}

template<typename T>
struct hex_t {
    T v;
    hex_t(T iv) : v(iv) {
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& ost, const hex_t<T>& t)
{
    ost << std::hex << t.v << std::dec;
    return ost;
}

class ZCE_API zce_logstr : public zce_smartptr_mtbase
{
private:
    _zlog_level level_;
    int line_;
    const char* func_;
    const char* fname_;
    const char* splitter_;
    std::ostringstream oss_;

public:

    zce_logstr(_zlog_level level, const char* func, const char* fname, int line, const char* splitter = "|");

    void flush();

    std::ostringstream& oss() {
        return oss_;
    }

    template <typename T>
    void log_args(std::ostream& ost, const T& t) {
        ost << t;
        if (splitter_ )
            ost << splitter_;
    }

    void log_args(std::ostream& ost, const char* t) {
        if (t == 0) {
            ost << "null";
        }
        else {
            ost << t;
        }
        if (splitter_)
            ost << splitter_;
    }

    template<typename T, typename... Args>
    void log_args(std::ostream& ost, const T& t, const Args&... args) {
        log_args(ost, t);
        log_args(ost, args...);
    }

    template<typename... Args>
    void write(Args... args) {
        log_args(oss_, args...);
        flush();
    }
};

#define ZTRACE(...) do { if(zlog_getlevel() <= ZLOG_TRACE) zce_logstr(ZLOG_TRACE, __FUNCTION__, __FILE__, __LINE__).write(__VA_ARGS__); } while(0)
#define ZDEBUG(...) do { if(zlog_getlevel() <= ZLOG_DEBUG) zce_logstr(ZLOG_DEBUG, __FUNCTION__, __FILE__, __LINE__).write(__VA_ARGS__); } while(0)
#define ZINFOR(...) do { if(zlog_getlevel() <= ZLOG_INFOR) zce_logstr(ZLOG_INFOR, __FUNCTION__, __FILE__, __LINE__).write(__VA_ARGS__); } while(0)
#define ZWARNI(...) do { if(zlog_getlevel() <= ZLOG_WARNI) zce_logstr(ZLOG_WARNI, __FUNCTION__, __FILE__, __LINE__).write(__VA_ARGS__); } while(0)
#define ZERROR(...) do { if(zlog_getlevel() <= ZLOG_ERROR) zce_logstr(ZLOG_ERROR, __FUNCTION__, __FILE__, __LINE__).write(__VA_ARGS__); } while(0)
#define ZFATAL(...) do { if(zlog_getlevel() <= ZLOG_FATAL) zce_logstr(ZLOG_FATAL, __FUNCTION__, __FILE__, __LINE__).write(__VA_ARGS__); } while(0)

#define ZBIZDT(...) 
//#define ZBIZDT(...) do { if(zlog_getlevel() <= ZLOG_BIZDT) zce_logstr(ZLOG_BIZDT, __FUNCTION__, __FILE__, __LINE__).write(__VA_ARGS__); } while(0)

#endif

#endif
