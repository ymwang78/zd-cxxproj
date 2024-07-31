#pragma once
// ***************************************************************
//  zce_convertor   version:  1.0  date: 2007-12-21
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// return Characters(not Bytes) of Target
// ***************************************************************

#include <zce/zce_config.h>
#include <memory>

class ZCE_API zce_convertor
{
    bool swap_;
public:
    enum Result 
    {
        CONVERSION_OK = 0,
        SOURCE_EXHAUSTED = ZCE_ERROR_CONVERTOR,
        TARGET_EXHAUSTED,
        SOURCE_ILLEGAL
    };

    zce_convertor(bool swap = false) : swap_(swap) {};
    static int gb2312_to_utf8(const char* source, size_t source_size, unsigned char* target, size_t target_size, bool strict = true);
    static int utf8_to_gb2312(const unsigned char* source, size_t source_size, char* target, size_t target_size, bool strict = true);
    static int wchart_to_gb2312(const wchar_t* source, size_t source_size, char* target, size_t target_size);
    static int gb2312_to_wchart(const char* source, size_t source_size, wchar_t* target, size_t target_size);
    static int utf16_to_utf8(const zce_char16* source, size_t source_size, char* target, size_t target_size);
    static int utf8_to_utf16(const char* source, size_t source_size, zce_char16* target, size_t target_size);
    static std::string utf16_to_utf8(const zce_ustring& ustr);
    static int utf8_to_wchart(const char* source, size_t source_size, wchar_t* target, size_t target_size);
    static int wchart_to_utf8(const wchar_t* source, size_t source_size, char* target, size_t target_size);
};

class ZCE_API zce_convertor_to_utf8
{
public:
    enum err_code
    {
        E_OK,
        E_ERR = ~0
    };
    zce_convertor_to_utf8(const char* gb2312_str, int len = 0);    //gb2312
    ~zce_convertor_to_utf8();

    int utf8_len() const { return utf8_len_; }
    operator const char*() const    { return utf8_str_; }
    bool result() const         { return E_OK == error_; };
private:
    int             utf8_len_;
    char*            utf8_str_;
    char            tmp_str_[2048];
    enum err_code    error_;
};

class ZCE_API zce_convertor_from_utf8
{
public:
    enum err_code
    {
        E_OK,
        E_ERR = ~0
    };
    zce_convertor_from_utf8(const char* utf8_str, int len = 0);
    ~zce_convertor_from_utf8();
    operator const char*() const { return str_; }
    int str_len() const { return str_len_; }
    bool result() const { return E_OK == error_; }
private:
    int         str_len_;
    char*        str_;
    char           tmpstr_[2048];
    enum err_code    error_;
};

class ZCE_API zce_convertor_to_wchart
{
public:
    enum err_code
    {
        E_OK,
        E_ERR = ~0
    };
    zce_convertor_to_wchart(const char* gbk_str, int len = 0);
    ~zce_convertor_to_wchart();
    operator const wchar_t*() const { return str_; }
    int str_len() const { return str_len_; }
    bool result() const { return E_OK == error_; }
private:
    int             str_len_;
    wchar_t*        str_;
    wchar_t           tmpstr_[2048];
    enum err_code    error_;
};
