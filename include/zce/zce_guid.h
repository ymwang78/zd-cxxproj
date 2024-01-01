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
    zce_guid32();
	zce_guid32(const std::vector<unsigned char>&);
    const std::vector<unsigned char>& get_byte_array()const;
    std::string to_string() const;
    bool operator<(const zce_guid32& rhs) const;
};

#endif // zce_guid_h__
