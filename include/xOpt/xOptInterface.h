#pragma once
// ***************************************************************
//  xOptInterface   version:  1.0   -  date:  2025/05/29
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xRto.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifdef _WIN32
#    ifndef XOPTIF_API
#        ifdef XOPTINTERFACE_EXPORTS
#            define XOPTIF_API __declspec(dllexport)
#        else
#            define XOPTIF_API __declspec(dllimport)
#        endif
#    endif
#else
#    define XOPTIF_API __attribute__((visibility("default")))
#endif

enum XOPTF_ERRCODE : int { 
    XOPTF_OK = 0,

    XOPTF_SUCCE_BASE = 5290000,

    XOPTF_ERROR_BASE = 85290000,
    XOPTF_ERROR_UNIMPLEMENT, // 未实现接口, 例如不支持固定变量
    XOPTF_ERROR_INVALID,     // 数据无效, 例如固定的变量列表不合法
};