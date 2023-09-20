// ***************************************************************
//  zxml_convertor   version:  1.0     date: 07/31/2002
//  -------------------------------------------------------------
//  Yongming Wang(ymwang@iipc.zju.edu.cn)
//  The Institute of System Engineering, Zhejiang University
//  -------------------------------------------------------------
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// char* assume utf-8 characters
// wchar* assume utf-16 characters
// std::string use native encode or unicode depends the mircro ACE_USES_WCHAR
// ***************************************************************
#pragma once
#ifndef __zxml_convertor_h__
#define __zxml_convertor_h__

#include <zce/zce_config.h>
#include <zce/zce_types.h>
#include <string>

int             ZCE_API   zxml_char_to_attr(char* buffer, int size, const char* name, char val);
char            ZCE_API   zxml_char_from_attr(const char* val);
int             ZCE_API   zxml_uchar_to_attr(char* buffer, int size, const char* name, unsigned char val);
unsigned char   ZCE_API   zxml_uchar_from_attr(const char* val);
int             ZCE_API   zxml_short_to_attr(char* buffer, int size, const char* name, short val);
short           ZCE_API   zxml_short_from_attr(const char* val);
int             ZCE_API   zxml_ushort_to_attr(char* buffer, int size, const char* name, unsigned short val);
unsigned short  ZCE_API   zxml_ushort_from_attr(const char* val);
int             ZCE_API   zxml_long_to_attr(char* buffer, int size, const char* name, int val);
int             ZCE_API   zxml_long_from_attr(const char* val);
int             ZCE_API   zxml_ulong_to_attr(char* buffer, int size, const char* name, unsigned val);
unsigned        ZCE_API   zxml_ulong_from_attr(const char* val);
int             ZCE_API   zxml_longlong_to_attr(char* buffer, int size, const char* name, long long val);
long long       ZCE_API   zxml_longlong_from_attr(const char* val);
int             ZCE_API   zxml_ulonglong_to_attr(char* buffer, int size, const char* name, unsigned long long val);
unsigned long long    ZCE_API   zxml_ulonglong_from_attr(const char* val);
int             ZCE_API   zxml_ip_to_attr(char* buffer, int size, const char* name, unsigned val);
unsigned        ZCE_API   zxml_ip_from_attr(const char* val);
int             ZCE_API   zxml_time_to_attr(char* buffer, int size, const char* name, unsigned val);
unsigned        ZCE_API   zxml_time_from_attr(const char* val);
int             ZCE_API   zxml_bool_to_attr(char* buffer, int size, const char* name, unsigned char val);
unsigned char   ZCE_API   zxml_bool_from_attr(const char* val);
int             ZCE_API   zxml_float_to_attr(char* buffer, int size, const char* name, float val, const char* fmt);
float           ZCE_API   zxml_float_from_attr(const char* val);
int             ZCE_API   zxml_double_to_attr(char* buffer, int size, const char* name, double val, const char* fmt);
double          ZCE_API   zxml_double_from_attr(const char* val);
int             ZCE_API   zxml_string_to_attr(char* buffer, int size, const char* name, const std::string& val);
std::string     ZCE_API   zxml_string_from_attr(const char* val);
int             ZCE_API   zxml_char_to_elem(char* buffer, int size, const char* name, char val);
int             ZCE_API   zxml_uchar_to_elem(char* buffer, int size, const char* name, unsigned char val);
int             ZCE_API   zxml_short_to_elem(char* buffer, int size, const char* name, short val);
int             ZCE_API   zxml_ushort_to_elem(char* buffer, int size, const char* name, unsigned short val);
int             ZCE_API   zxml_long_to_elem(char* buffer, int size, const char* name, int val);
int             ZCE_API   zxml_ulong_to_elem(char* buffer, int size, const char* name, unsigned val);
int             ZCE_API   zxml_ip_to_elem(char* buffer, int size, const char* name, unsigned val);
int             ZCE_API   zxml_float_to_elem(char* buffer, int size, const char* name, float val, const char* fmt);
int             ZCE_API   zxml_double_to_elem(char* buffer, int size, const char* name, double val, const char* fmt);
int             ZCE_API   zxml_string_to_elem(char* buffer, int size, const char* name, const std::string& val);
int             ZCE_API   zxml_time_to_elem(char* buffer, int size, const char* name, unsigned  val);
int             ZCE_API   zxml_bool_to_elem(char* buffer, int size, const char* name, unsigned char val);

int             ZCE_API   zxml_builtin_array_to_attr(char* buffer, int size, const char* name, const std::vector<zce_char>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_attr(char* buffer, int size, const char* name, const std::vector<zce_byte>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_attr(char* buffer, int size, const char* name, const std::vector<zce_int16>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_attr(char* buffer, int size, const char* name, const std::vector<zce_uint16>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_attr(char* buffer, int size, const char* name, const std::vector<zce_int32>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_attr(char* buffer, int size, const char* name, const std::vector<zce_uint32>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_attr(char* buffer, int size, const char* name, const std::vector<zce_int64>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_attr(char* buffer, int size, const char* name, const std::string& val, const char* fmt);

int             ZCE_API   zxml_builtin_array_to_elem(char* buffer, int size, const char* name, const std::vector<zce_char>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_elem(char* buffer, int size, const char* name, const std::vector<zce_byte>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_elem(char* buffer, int size, const char* name, const std::vector<zce_int16>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_elem(char* buffer, int size, const char* name, const std::vector<zce_uint16>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_elem(char* buffer, int size, const char* name, const std::vector<zce_int32>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_elem(char* buffer, int size, const char* name, const std::vector<zce_uint32>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_elem(char* buffer, int size, const char* name, const std::vector<zce_int64>& vec, const char* fmt);
int             ZCE_API   zxml_builtin_array_to_elem(char* buffer, int size, const char* name, const std::string& val, const char* fmt);

int                ZCE_API    zxml_builtin_array_from_attr(std::vector<zce_char>& vec, const char* val, size_t length);
int                ZCE_API    zxml_builtin_array_from_attr(std::vector<zce_byte>& vec, const char* val, size_t length);
int                ZCE_API    zxml_builtin_array_from_attr(std::vector<zce_int16>& vec, const char* val, size_t length);
int                ZCE_API    zxml_builtin_array_from_attr(std::vector<zce_uint16>& vec, const char* val, size_t length);
int                ZCE_API    zxml_builtin_array_from_attr(std::vector<zce_int32>& vec, const char* val, size_t length);
int                ZCE_API    zxml_builtin_array_from_attr(std::vector<zce_uint32>& vec, const char* val, size_t length);
int                ZCE_API    zxml_builtin_array_from_attr(std::vector<zce_int64>& vec, const char* val, size_t length);
int                ZCE_API    zxml_builtin_array_from_attr(std::string& str, const char* val, size_t length);

#endif /*__zxml_convertor_h__*/
