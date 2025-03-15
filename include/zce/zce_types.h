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

// fix defines in different platform
#ifndef ZCE_TYPES_DEFINED
#    define ZCE_TYPES_DEFINED

#    ifdef _WIN32

#        if !defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED)
typedef intptr_t ssize_t;
#            define _SSIZE_T_
#            define _SSIZE_T_DEFINED
#        endif //!defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED)

typedef DWORD threadid_t;
typedef HANDLE handle_t;
typedef int socklen_t;
struct iovec {
    u_long iov_len;
    char* iov_base;
};
#    else //_WIN32
typedef pthread_t threadid_t;
typedef int handle_t;
#    endif  //_WIN32

#endif  // ZCE_TYPES_DEFINED

#ifdef _WIN32
#    define ZCE_INVALID_HANDLE INVALID_HANDLE_VALUE
typedef long long int zce_int64;
typedef unsigned long long int zce_uint64;
#else
#    define ZCE_INVALID_HANDLE -1
#    if defined(__GNUC__) && (__WORDSIZE == 64)
typedef long int zce_int64;
typedef unsigned long int zce_uint64;
#    else
typedef long long int zce_int64;
typedef unsigned long long int zce_uint64;
#    endif
#endif

typedef char zce_char;
typedef char zce_int8;
typedef short zce_int16;
typedef int zce_int32;
typedef unsigned char zce_byte;
typedef unsigned char zce_uint8;
typedef unsigned short zce_uint16;
typedef unsigned int zce_uint32;
typedef float zce_float;
typedef double zce_double;
typedef struct timespec zce_timespec_t;
typedef zce_int64 zce_timestamp;  // same as pgsql timestamp, microsecond from 2000-1-1 00:00:00

// to simplify type cast or type safety
typedef union _zce_sockaddr_t {
    struct sockaddr sa;
    struct sockaddr_in sa_in;
    struct sockaddr_in6 sa_in6;
    struct sockaddr_storage sa_stor;
} zce_sockaddr_t;

typedef struct _zce_addr_t {
    zce_byte translayer;
    zce_byte ipclass;
    zce_sockaddr_t addr;
} zce_addr_t;

#ifdef __cplusplus
#    include <string>
#    include <vector>

#    ifdef _UNICODE
typedef std::u16string zce_tstring;
typedef char16_t zce_tchar;
#    else   //_UNICODE
typedef std::string zce_tstring;
typedef char zce_tchar;
#    endif  //_UNICODE

typedef char16_t zce_char16;
typedef std::u16string zce_ustr;
typedef std::u16string_view zce_ustrview;
typedef std::string zce_string;
typedef std::string zce_astring;
typedef std::vector<zce_byte> zce_bytevec;
typedef std::vector<zce_ustr> zce_ustrvec;
typedef std::vector<zce_string> zce_strvec;
typedef std::vector<zce_astring> zce_astrvec;
typedef std::vector<zce_tstring> zce_tstrvec;

template <typename T>
class zce_matrix {
    int _x, _y;
    std::vector<T> _vec;

  public:
    zce_matrix() : _x(0), _y(0) {}
    zce_matrix(int x, int y) : _x(x), _y(y), _vec(x * y) {}
    zce_matrix(const zce_matrix& m) : _x(m._x), _y(m._y), _vec(m._vec) {}
    zce_matrix(zce_matrix&& m) : _x(m._x), _y(m._y), _vec(std::move(m._vec)) { m._x = m._y = 0; }
    ~zce_matrix() {}
    zce_matrix& operator=(const zce_matrix& m) {
        _x = m._x;
        _y = m._y;
        _vec = m._vec;
        return *this;
    }
    zce_matrix& operator=(zce_matrix&& m) {
        _x = m._x;
        _y = m._y;
        _vec = std::move(m._vec);
        m._x = m._y = 0;
        return *this;
    }
    inline bool operator==(const zce_matrix& m) const {
        return _x == m._x && _y == m._y && _vec == m._vec;
    }
    inline T& operator()(int i, int j) { return _vec[i * _y + j]; }
    inline T operator()(int i, int j) const { return _vec[i * _y + j]; }
    inline void resize(int x, int y) {
        _vec.resize(x * y);
        this->_x = x;
        this->_y = y;
    }
    inline int rows() const { return _x; }
    inline int cols() const { return _y; }
    inline size_t size() const { return _vec.size(); }
    inline const T* data() const { return _vec.data(); }
    inline T* data() { return _vec.data(); }
    inline const std::vector<T>& vec() const { return _vec; }
    inline bool empty() const { return _x == 0 || _y == 0 || _vec.empty(); }
    inline void clear() {
        _x = 0;
        _y = 0;
        _vec.clear();
    }
};

typedef zce_matrix<double> zce_dblmat;

#endif  //__cplusplus
