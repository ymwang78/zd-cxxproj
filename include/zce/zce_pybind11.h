#pragma once
// ***************************************************************
//  zce_pybind11   version:  1.0   -  date:  2023/10/19
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project ZCE.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#include <pybind11/pybind11.h>
#include <zce/zce_object.h>

namespace pybind11 {
namespace detail {

template <typename T>
struct type_caster<zce_smartptr<T>> {
    using SmartPtr = zce_smartptr<T>;
    using type = SmartPtr;
    PYBIND11_TYPE_CASTER(SmartPtr, _("zce_smartptr"));

    bool load(handle src, bool) {
        // 不支持从 Python 转为 zce_smartptr<T>，可选实现
        return false;
    }

    static handle cast(const SmartPtr& src, return_value_policy, handle) {
        if (!src) return pybind11::none().release();
        return pybind11::cast(src.operator->(), return_value_policy::take_ownership).release();
    }
};

template <typename T>
struct holder_helper<zce_smartptr<T>> {
    static const T* get(const zce_smartptr<T>& p) { return p.operator->(); }
};

}  // namespace detail
}  // namespace pybind11
