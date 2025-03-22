#pragma once
// ***************************************************************
//  zce_sol3   version:  1.0   -  date:  2023/10/19
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project ZCE.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include <sol/sol.hpp>
#include <string>
#include <zce/zce_object.h>

namespace sol {

template <typename T>
struct unique_usertype_traits<zce_smartptr<T>> {
    typedef T type;
    typedef zce_smartptr<T> actual_type;
    static const bool value = true;

    static bool is_null(const actual_type& ptr) { return ptr == nullptr; }

    static type* get(const actual_type& ptr) { return (type*)ptr; }
};

}  // namespace sol