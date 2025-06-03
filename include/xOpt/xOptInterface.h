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
