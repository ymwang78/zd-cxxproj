#pragma once
// ***************************************************************
//  zce_filesystem   version:  1.0   -  date: 2003/01/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include <zce/zce_config.h>
#include <zce/zce_types.h>
#include <vector>
#include <stdarg.h>
#include <sstream>

std::string ZCE_API zce_get_path(const char* filename);

std::string ZCE_API zce_abs_path(const char* filename);

int ZCE_API zce_add_path(const char* path);

void ZCE_API zce_chdir();

bool ZCE_API zce_makedir(const char* dir);

const char* ZCE_API zce_filename(const char* str);

bool ZCE_API zce_filepath_exists(const char* path);