#pragma once
// ***************************************************************
//  zce_api   version:  1.0  date: 2002/04/29
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of ZCE, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __zce_api_h__
#define __zce_api_h__

#include <zce/zce_config.h>
#include <zce/zce_types.h>
#include <zce/zce_api.h>
#include <vector>
#include <set>
#include <stdarg.h>
#include <sstream>

#ifdef __cplusplus
extern "C"
{
#endif

    int ZCE_API zce_init();

    void ZCE_API zce_fini();

    int ZCE_API zce_init_pyenv(const char* python_home);

    void ZCE_API zce_fini_pyenv();

    // Wire Python logging (ZceLogHandler) into ZCE log; optionally redirect
    // sys.stdout/stderr for print (stdout->trace, stderr->error).
    // redirect_stdio: 0=logging only, nonzero=logging + stdio. Requires zce_init_pyenv().
    int ZCE_API zce_setup_py_logging(int redirect_stdio);

    struct tm * ZCE_API zce_localtime_r(const time_t *t, struct tm *res);

    zce_uint64 ZCE_API zce_tick();

    zce_uint64 ZCE_API zce_nowms();

    zce_uint64 ZCE_API zce_atoi64(const char* str);

    int ZCE_API zce_localtime_str(char* buf, int size, bool msec);

    int ZCE_API zce_timespec_str(char* buf, int size, struct timespec*, bool msec);

    /**
     * @brief Format a timespec as UTC text with an explicit "+00" offset.
     *
     * Produces "YYYY-MM-DD HH:MM:SS.uuuuuu+00" (29 chars). Unlike zce_timespec_str(),
     * which renders local wall clock with no offset, this shape is unambiguous no
     * matter what time zone the reader is in -- which is what a PostgreSQL
     * timestamptz literal needs, since PostgreSQL would otherwise apply the session
     * TimeZone to an offset-less literal.
     *
     * @param buf  Output buffer.
     * @param size Size of @p buf; 30 bytes hold the result plus its NUL.
     * @param ts   Time to format; tv_nsec is clamped to [0, 1e9).
     * @return Number of characters the full text needs (snprintf semantics), or -1
     *         on a bad argument or an unrepresentable time.
     */
    int ZCE_API zce_timespec_utc_str(char* buf, int size, const struct timespec* ts);

    /**
     * @brief Microseconds since 2000-01-01 00:00:00 UTC -- the meaning of zce_timestamp.
     *
     * The epoch is UTC and the count is time-zone independent: the same instant has the
     * same zce_timestamp everywhere. Every producer of the type must agree on this,
     * which is what zce_to_timestamp() and zce_timestamp_from_asc() also do.
     *
     * Note this is NOT a Unix timestamp -- it is 946684800 seconds (10957 days) smaller.
     * Handing one to code that expects Unix time reads 30 years early.
     */
    zce_timestamp ZCE_API zce_timestamp_now();

    /**
     * @brief Convert Unix time (UTC seconds) to a zce_timestamp.
     *
     * Same scale as zce_timestamp_now(), so values from the two are directly comparable.
     *
     * @note Until 2026-08 this subtracted the local standard UTC offset, putting its
     *       results a whole time-zone offset away from zce_timestamp_now() (8 hours on
     *       UTC+8). zce_to_timet() added the same offset back, so round-trips always
     *       looked right and only cross-constructor comparisons and formatting were
     *       wrong.
     *
     *       Upgrading shifts the numeric value by that offset, so it is not enough to
     *       rebuild in place:
     *       - Values that only live inside one process, or that cross a process boundary,
     *         need producers and consumers rebuilt and deployed together.
     *       - Values already PERSISTED by the old implementation keep the old encoding on
     *         disk. Rebuilding does not touch them, and a new reader will interpret them
     *         as UTC and land a local offset away from the instant that was meant. Such
     *         data must be migrated, or versioned so readers can tell the two encodings
     *         apart. The old implementation computed
     *             ts_old = (t - timezone) * 1000000 - epoch_2000_unix_us
     *         so the migration is exactly
     *             ts_new = ts_old + timezone * 1000000LL
     *         where @c timezone is the POSIX global (@c _timezone on Windows): seconds
     *         WEST of UTC, which is the NEGATION of the "UTC+08"-style wall-clock offset.
     *         On UTC+8 timezone is -28800, so ts_new = ts_old - 28800000000LL: the stored
     *         value moves BACK 8 hours. Reading the rule as "+08" and advancing it lands
     *         16 hours out.
     *         Note @c timezone is standard time, fixed per zone, and the old code applied
     *         it unconditionally -- so one constant migrates every stored value, whatever
     *         date it carries and whether or not DST was in force when it was written.
     *       On a UTC host both encodings coincide and nothing has to be done.
     */
    zce_timestamp ZCE_API zce_to_timestamp(time_t t);

    /**
     * @brief Inverse of zce_to_timestamp(): a zce_timestamp as Unix time (UTC seconds).
     *
     * Sub-second digits are truncated toward zero, so timestamps before 2000-01-01 round
     * up to the next whole second. Strip the sub-second part with floor semantics first
     * if that matters.
     */
    time_t ZCE_API zce_to_timet(zce_timestamp ts);

    /**
     * @brief Sentinel returned by zce_timestamp_from_asc() when the input cannot be parsed.
     *
     * Deliberately not -1: -1 is a legal zce_timestamp (1us before 2000-01-01) and
     * zce_to_timet(-1) yields a plausible-looking time_t, which used to hide parse
     * failures behind a fixed "year 2000" reading.
     */
#define ZCE_TIMESTAMP_INVALID ((zce_timestamp)(-9223372036854775807LL - 1))

    /**
     * @brief Parse a textual timestamp into a zce_timestamp.
     *
     * Accepted grammar (the shapes PostgreSQL emits for timestamp/timestamptz):
     *   YYYY-MM-DD<sep>HH:MM:SS[.frac][offset]
     * where @c frac is 1..6+ digits (extra digits are truncated) and @c offset is
     * @c Z, @c z or @c (+|-)HH[[:]MM[[:]SS]]. Both are optional.
     *
     * @c sep is a run of blanks and/or 'T' that may be EMPTY: the underlying
     * zce_strptime() consumes "[[:space:]T]*" wherever its format holds one blank. So
     * "2026-08-20 12:34:56", "2026-08-20T12:34:56", "2026-08-20TTT12:34:56" and even
     * "2026-08-2012:34:56" all parse, to the same instant. A caller that needs the strict
     * single-separator ISO shape must check that itself -- passing @p out as NULL
     * validates against this looser grammar, not against ISO 8601.
     *
     * With an offset the wall clock is interpreted in that zone; without one it is
     * interpreted in the local zone (mktime semantics).
     *
     * Impossible calendar dates are a parse failure, not something to normalize:
     * "2026-02-31" is rejected rather than silently read as 2026-03-03.
     *
     * @param timestr Input string; leading/trailing blanks are tolerated.
     * @param out     Receives the parsed value, or ZCE_TIMESTAMP_INVALID on failure.
     *                May be NULL if only validation is wanted.
     * @return 0 on success, -1 on failure.
     */
    int ZCE_API zce_timestamp_from_asc_ex(const char* timestr, zce_timestamp* out);

    /**
     * @brief Same parsing as zce_timestamp_from_asc_ex(), returning the value directly.
     * @return The parsed timestamp, or ZCE_TIMESTAMP_INVALID if @p timestr is malformed.
     */
    zce_timestamp ZCE_API zce_timestamp_from_asc(const char* timestr);

    char* ZCE_API zce_strptime(const char* buf, const char* fmt, struct tm* tm, int* tz_offset);

    unsigned ZCE_API zce_thread_id();

    void ZCE_API zce_oom_adjust(int adjust);

    void ZCE_API zce_to_hex(char* out, size_t out_size, const char* in, size_t in_size, bool upper = false);

    int ZCE_API zce_from_hex(unsigned char* out, size_t out_size, const char* in, size_t in_size);

    int ZCE_API zce_base64_decode(unsigned char* result, const unsigned char* input, size_t inputlen);

    int ZCE_API zce_base64_encode(unsigned char* buffer, const unsigned char* input, size_t input_len);

    /*result == nul 可以预先计算所需大小*/
    int ZCE_API zce_base58_decode(unsigned char* result, size_t result_size, const unsigned char* input, size_t inputlen);

    int ZCE_API zce_base58_encode(unsigned char* buffer, size_t buffer_size, const unsigned char* input, size_t input_len);

    int ZCE_API zce_base32_encode(char* output, const zce_byte* data, size_t length);

    int ZCE_API zce_base32_decode(zce_byte* output, const char* encoded, size_t length);

    size_t ZCE_API zce_base64_length(const char* input);

    bool ZCE_API zce_interrupted();

    void ZCE_API zce_enable_coredump(bool enable);

    void ZCE_API zce_msleep(int msec);

    unsigned ZCE_API zce_getmem();

    unsigned ZCE_API zce_getcpu();

    enum ERV_ZCE_COMPRESS {
        ZCE_COMPRESS_AUTO = -1,
        ZCE_COMPRESS_NONE,
        ZCE_COMPRESS_BZIP2,
    };

    //out_size should be large enough
    int ZCE_API zce_compress(ERV_ZCE_COMPRESS cps, char* out_buf, size_t& out_size, const char* in_buf, size_t in_size);
    int ZCE_API zce_decompress(ERV_ZCE_COMPRESS cps, char* out_buf, size_t& out_size, const char* in_buf, size_t in_size);

    //inet api
	int ZCE_API zce_inet_isip(const char* ip);
    int ZCE_API zce_inet_isloopback(const zce_sockaddr_t& addr);

    int ZCE_API zce_inet_ntop4(const unsigned char* src, char* dst, size_t size);
    int ZCE_API zce_inet_ntop6(const unsigned char* src, char* dst, size_t size);
    const char* ZCE_API zce_inet_ntop(int af, const void* src, char* dst, size_t size);

    int ZCE_API zce_inet_pton4(const char* src, unsigned char* dst);
    int ZCE_API zce_inet_pton6(const char* src, unsigned char* dst);
    int ZCE_API zce_inet_pton(int af, const char* src, void* dst);

    int ZCE_API zce_inet_aton(zce_sockaddr_t*, const char* addr, int port);
    int ZCE_API zce_inet_ntoa(char* ip, unsigned len, const zce_sockaddr_t* addr, bool bport);

    int ZCE_API zce_inet_parse_host_port(const char*& host, int& hostlen, int& port, const char* hostport);

    int ZCE_API zce_create_pipe(SOCKET fds[2]);


#ifdef __cplusplus
}
#endif

// 解析嵌入式 Python home，返回存在的绝对路径；一个候选都不存在时返回空串。
// 候选优先级：python_home 实参 > ZCE_VIRTUAL_VENV 环境变量 > 平台默认值
// （Windows 为 ../WinPy<主><次>/python，Linux 为 /opt/venv）。
// 即 python_home 传 nullptr 时，ZCE_VIRTUAL_VENV 就是优先级最高的覆盖手段。
// 其中相对路径先按「可执行文件所在目录」解析，再按进程工作目录解析：服务与测试都可能
// 从任意目录启动，只按工作目录解析会在目录不对时静默失败，随后取 GIL 直接访问违例。
// zce_init_pyenv() 内部用的就是这个函数，python_home 传 nullptr 即表示只用默认候选。
// 仅在 ZCE_SUPPORT_PYVM 编译时提供实现。
std::string ZCE_API zce_resolve_pyenv_home(const char* python_home = nullptr);

int ZCE_API zce_symbol_read(const char* buf, int size, std::string& val, char split = ' ');

int ZCE_API zce_symbol_read_linefeed(const char* buf, int size, std::string& val);

int ZCE_API zce_list_ip(std::vector<std::string>& ip_list);

void ZCE_API zce_str2addrvec(std::vector<zce_addr_t>& tsock_vec, const std::string& addrs);

std::set<std::string> ZCE_API zce_get_mac(bool include_virtual = false,
                                          bool include_tunnel = false);

std::set<std::string> ZCE_API zce_get_hardrive();

std::string ZCE_API zce_get_cpuinfo();

zce_bytevec ZCE_API zce_from_hex(const std::string& in);

std::string ZCE_API zce_to_hex(const unsigned char* p, size_t size);

std::string ZCE_API zce_to_hex(const std::vector<zce_byte>& vec);

std::string ZCE_API zce_md5sum(const char* fname);

std::string ZCE_API zce_hash_md5(const char* input, size_t inputlen, unsigned char binout[16], bool upper = false);

std::string ZCE_API zce_hash_sha256(const zce_byte* input, size_t inputlen, unsigned char binout[32], bool upper = false);

std::string ZCE_API zce_string_format(unsigned length, const char* format, ...);

std::string ZCE_API zce_trim(const std::string& str, const std::string& whitespace = " \t\r\n");

std::string ZCE_API zce_localtime_str(bool msec);

/**
 * @brief Render a zce_timestamp as LOCAL wall clock: "YYYY-MM-DD HH:MM:SS.mmm".
 *
 * The text carries no UTC offset, so it is ambiguous on its own -- read in another time
 * zone the same characters denote a different instant. It is the mirror of
 * zce_timestamp_from_asc(), which reads offset-less input as local time. Good for logs
 * and UI.
 *
 * That mirroring is a round trip only within limits, so do not rely on it as a general
 * one:
 * - Only milliseconds are emitted. A zce_timestamp whose microsecond part is not a
 *   multiple of 1000 loses the remainder, and parsing back yields the value truncated to
 *   the millisecond.
 * - Offset-less local text is ambiguous across a DST fall-back, where the same wall clock
 *   occurs twice; parsing picks one of the two instants (mktime semantics).
 *
 * For PostgreSQL literals or anything crossing time zones use zce_timespec_utc_str(),
 * which emits UTC with an explicit "+00".
 */
std::string ZCE_API zce_timestamp_to_asc(zce_timestamp ts);

std::string ZCE_API zce_get_hostname();

std::string ZCE_API zce_format_scientific(double value);

std::string ZCE_API zce_error_msg(int errnum);

// Human-readable text for ERV_ZCE_ERROR / ZCE family codes (zce_inc.h). See also zce_error_msg() for OS errno.
std::string ZCE_API zce_error_desc(int errcode);

template<typename T>
inline T zce_min(T v1, T v2) 
{
    return (v1 < v2) ? v1 : v2;
}

template<typename T>
inline T zce_max(T v1, T v2)
{
    return (v1 > v2) ? v1 : v2;
}

template<typename T>
inline T zce_abs(const T& iValue)
{
    return iValue >= 0 ? iValue : -iValue;
}

template<typename T>
inline T zce_sum(const std::vector<T>& iVec)
{
    T v = 0;
    for (auto iter = iVec.begin(); iter != iVec.end(); ++iter) {
        v += *iter;
    }
    return v;
}

template<typename T>
inline void zce_sub(std::vector<T>& iVec, T v)
{
    for (auto iter = iVec.begin(); iter != iVec.end(); ++iter) {
        *iter -= v;
    }
}

template <typename T>
void zce_multi(std::vector<T>& rhs, double v)
{
    for (unsigned i = 0; i < rhs.size(); ++i) {
        if (rhs[i] == 0)
            continue;
        rhs[i] *= v;
    }
}

template <typename T>
zce_int64 zce_multi(const std::vector<T>& rhs)
{
    zce_int64 sum = 0;
    for (unsigned i = 0; i < rhs.size(); ++i) {
        if (rhs[i] == 0)
            return 0;
        sum *= rhs[i];
    }
    return sum;
}

template <typename T>
T zce_min(const std::vector<T>& rhs)
{
    if (rhs.size() == 0)
        return 0;
    T m = rhs[0];
    for (unsigned i = 0; i < rhs.size(); ++i) {
        if (rhs[i] < m)
            m = rhs[i];
    }
    return m;
}

template <typename T>
int zce_count(const std::vector<T>& rhs, T v)
{
    int count = 0;
    for (unsigned i = 0; i < rhs.size(); ++i) {
        if (rhs[i] == v)
            ++count;
    }
    return count;
}

template <typename T, typename P, typename Q>
int zce_multi(std::vector<T>& res, const std::vector<P>& lhs, const std::vector<Q>& rhs)
{
    if (res.size() != lhs.size() || lhs.size() != rhs.size())
        return -1;

    for (unsigned i = 0; i < rhs.size(); ++i) {
        res[i] = rhs[i] * lhs[i];
    }
    return 0;
}

template <typename T, typename P>
int zce_div(std::vector<double>& res, const std::vector<T>& lhs, P sum)
{
    if (res.size() != lhs.size())
        return -1;

    for (unsigned i = 0; i < lhs.size(); ++i) {
        res[i] = lhs[i] * 1.0 / sum;
    }
    return 0;
}

#ifdef _WIN32

HRESULT ZCE_API zce_createinstance(const wchar_t* dllname, REFCLSID rclsid, REFIID riid, LPVOID* ppv);

std::string ZCE_API zce_getharddrv();

std::string ZCE_API zce_getimei();

int ZCE_API zce_getmblock(zce::RefBlock& dblock);

void ZCE_API zce_setmblock(zce::RefBlock& dblock);

std::string ZCE_API zce_to_utf8(const wchar_t* src, unsigned len);

std::wstring ZCE_API zce_to_utf16(const char* src, unsigned len);

std::wstring ZCE_API zce_hresult_message(HRESULT hr);

#endif

namespace zce {

}

template<typename T>
static inline T zce_counter(zce_uint32 spanid, zce_uint32& lastspanid, T& count)
{
    if (spanid == lastspanid) {
        ++count;
        return 0;
    }
    else {
        T v = count;
        count = 0;
        lastspanid = spanid;
        return v;
    }
}

class ZCE_API zce_profile {
    const char* name_;
    unsigned begin_tick_;
    unsigned limit_;
  public:
    zce_profile(const char* name, unsigned limit)
        : name_(name), limit_(limit), begin_tick_((unsigned)zce_tick()){
    };

    ~zce_profile();

    inline void check_point(int line);
};

#endif // __zce_api_h__
