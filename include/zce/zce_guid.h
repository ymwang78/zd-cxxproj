// ***************************************************************
//  zce_guid   version:  1.0   -  date: 4/28/2012
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
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
    const std::vector<unsigned char>& get_byte_array()const;
    std::string to_string() const;
};

#endif // zce_guid_h__
