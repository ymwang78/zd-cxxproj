#pragma once
// ***************************************************************
//  zce_conf.h   version:  1.0   -  date: 2002/4/27/
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of ZCE, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include <zce/zce_object.h>

class zce_dblock;

namespace zce
{

class ZCE_API FileMonitor : public zce_object {
    time_t st_mtime_;

    std::string getFullpath(const char* filename);

    int loadFile(zce_dblock& dblock, const char* filename);

  public:
    FileMonitor();

    virtual ~FileMonitor();

    bool isChanged(const char* filename);

    int load(const char* filename);

    virtual int processContent(const unsigned char* data, unsigned len) = 0;
};

template <typename T>
class ConfigTemplate : public FileMonitor {
  protected:
    T conf_;

  public:
    T& get() { return conf_; };

    const T& get_const() const { return conf_; };
};

}

