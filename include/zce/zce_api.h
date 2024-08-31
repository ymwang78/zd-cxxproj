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

    struct tm * ZCE_API zce_localtime_r(const time_t *t, struct tm *res);

    zce_uint64 ZCE_API zce_tick();

    zce_uint64 ZCE_API zce_nowms();

    zce_uint64 ZCE_API zce_atoi64(const char* str);

    int ZCE_API zce_localtime_str(char* buf, int size, bool msec);

    int ZCE_API zce_timespec_str(char* buf, int size, struct timespec*, bool msec);

    zce_timestamp ZCE_API zce_to_timestamp(time_t t);

    time_t ZCE_API zce_to_timet(zce_timestamp ts);

    zce_timestamp ZCE_API zce_timestamp_from_asc(const char* timestr);

    char* ZCE_API zce_strptime(const char* buf, const char* fmt, struct tm* tm, int* tz_offset);

    unsigned ZCE_API zce_thread_id();

    void ZCE_API zce_oom_adjust(int adjust);

    void ZCE_API zce_to_hex(char* out, size_t out_size, const char* in, size_t in_size, bool upper = false);

    int ZCE_API zce_from_hex(char* out, size_t out_size, const char* in, size_t in_size);

    int ZCE_API zce_base64_decode(unsigned char* result, const unsigned char* input, size_t inputlen);

    int ZCE_API zce_base64_encode(unsigned char* buffer, const unsigned char* input, size_t input_len);

    int ZCE_API zce_base32_encode(char* output, const zce_byte* data, size_t length);

    int ZCE_API zce_base32_decode(zce_byte* output, const char* encoded, size_t length);

    size_t ZCE_API zce_base64_length(const char* input);

    bool ZCE_API zce_interrupted();

    void ZCE_API zce_enable_coredump(bool enable);

    void ZCE_API zce_msleep(int msec);

    unsigned ZCE_API zce_getmem();

    unsigned ZCE_API zce_getcpu();

    enum ERV_ZCE_COMPRESS {
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

#ifdef __cplusplus
}
#endif

int ZCE_API zce_create_pipe(SOCKET fds[2]);

int ZCE_API zce_symbol_read(const char* buf, int size, std::string& val, char split = ' ');

int ZCE_API zce_symbol_read_linefeed(const char* buf, int size, std::string& val);

int ZCE_API zce_list_ip(std::vector<std::string>& ip_list);

void ZCE_API zce_str2addrvec(std::vector<zce_addr_t>& tsock_vec, const std::string& addrs);

std::set<std::string> ZCE_API zce_get_mac();

std::set<std::string> ZCE_API zce_get_hardrive();

std::string ZCE_API zce_get_cpuinfo();

zce_bytevec ZCE_API zce_from_hex(const std::string& in);

std::string ZCE_API zce_to_hex(const unsigned char* p, size_t size);

std::string ZCE_API zce_to_hex(const std::vector<zce_byte>& vec);

std::string ZCE_API zce_md5sum(const char* fname);

std::string ZCE_API zce_hash_md5(const char* input, size_t inputlen, unsigned char binout[16], bool upper = false);

std::string ZCE_API zce_string_format(unsigned length, const char* format, ...);

std::string ZCE_API zce_trim(const std::string& str, const std::string& whitespace = " \t\r\n");

std::string ZCE_API zce_localtime_str(bool msec);

std::string ZCE_API zce_timestamp_to_asc(zce_timestamp ts);

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

class zce_dblock;

int ZCE_API zce_getmblock(zce_dblock& dblock);

void ZCE_API zce_setmblock(zce_dblock& dblock);

std::string ZCE_API zce_to_utf8(const wchar_t* src, unsigned len);

std::wstring ZCE_API zce_to_utf16(const char* src, unsigned len);

#endif

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

#endif // __zce_api_h__
