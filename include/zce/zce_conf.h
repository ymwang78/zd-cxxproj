#pragma once
// ***************************************************************
//  zce_conf   version:  1.0   -  date: 2002/4/27/
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of ZCE, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __zce_conf_h__
#define __zce_conf_h__

#include <zce/zce_object.h>

class zce_dblock;

class ZCE_API zce_filemon : public zce_object
{
    time_t st_mtime_;

    std::string get_fullpath(const char* filename);

    int load_file(zce_dblock& dblock, const char* filename);

public:

    zce_filemon();

    virtual ~zce_filemon();

    bool is_changed(const char* filename);

    int load(const char* filename);

    virtual int process_content(unsigned char* data, unsigned len) = 0;
};

template<typename T>
class zce_conf : public zce_filemon
{
protected:

    T conf_;

public:

    T& get()
    { 
        return conf_; 
    };

    const T& get_const() const
    {
        return conf_;
    };
};

#endif // __zce_conf_h__
