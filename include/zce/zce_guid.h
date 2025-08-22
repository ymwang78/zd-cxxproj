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
#include <vector>
#include <string>

namespace zce {

class ZCE_API Guid32 {
    std::vector<unsigned char> uuid_;

  public:
    Guid32(bool tmseq = true);
    Guid32(const std::vector<unsigned char>&);
    const std::vector<unsigned char>& get_byte_array() const;
    std::string toString() const;
    bool operator<(const Guid32& rhs) const;
};

/*guid12使用90个bit，生成15个字节(避免MSVC和libstdc++的std::string动态分配)长度的guidstr，
 * 字母表包含26个英文大小写字母10个数字和_-
 * 确保是时间顺序的，方便作为数据库索引
 */
class ZCE_API Guid15 {
    std::string uuid_;

  public:
    Guid15(int machine_id = 0);
    Guid15(const std::string& str) : uuid_(str) {};
    const std::string& toString() const { return uuid_; };
    bool operator<(const Guid15& rhs) const { return uuid_ < rhs.uuid_; };
};

}  // namespace zce
