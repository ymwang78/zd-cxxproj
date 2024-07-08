#pragma once
// ***************************************************************
//  zce_guid   version:  1.0   -  date: 2003/4/28
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of ZCE, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef zce_guid_h__
#define zce_guid_h__

#include <vector>
#include <string>

class ZCE_API zce_guid32
{
    std::vector<unsigned char> uuid_;
public:
    zce_guid32(bool tmseq = true);
	zce_guid32(const std::vector<unsigned char>&);
    const std::vector<unsigned char>& get_byte_array()const;
    std::string to_string() const;
    bool operator<(const zce_guid32& rhs) const;
};


/*guid12使用90个bit，生成15个字节(避免MSVC和libstdc++的std::string动态分配)长度的guidstr，
* 字母表包含26个英文大小写字母10个数字和_-
* 确保是时间顺序的，方便作为数据库索引
*/
class ZCE_API zce_guid15
{
    std::string uuid_;
public:
    zce_guid15(int machine_id = 0);
    zce_guid15(const std::string& str) : uuid_(str) {};
    const std::string& to_string() const { return uuid_; };
    bool operator<(const zce_guid15& rhs) const { return uuid_ < rhs.uuid_; };
};

#endif // zce_guid_h__
