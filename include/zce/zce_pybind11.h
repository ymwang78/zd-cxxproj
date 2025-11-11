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
#include <zce/zce_any.h>

namespace pybind11 {
namespace detail {

template <typename T>
struct type_caster<zce::SmartPtr<T>> {
    using type = zce::SmartPtr<T>;
    PYBIND11_TYPE_CASTER(type, _("SmartPtr"));

    bool load(handle src, bool) {
        // 不支持从 Python 转为 zce_smartptr<T>，可选实现
        return false;
    }

    static handle cast(const type& src, return_value_policy, handle) {
        if (!src) return pybind11::none().release();
        return pybind11::cast(src.operator->(), return_value_policy::take_ownership).release();
    }
};

template <typename T>
struct holder_helper<zce::SmartPtr<T>> {
    static const T* get(const zce::SmartPtr<T>& p) { return p.operator->(); }
};

}  // namespace detail
}  // namespace pybind11

namespace zce {
extern pybind11::object any_to_pyobject(const zce::Any& v);
}  // namespace zce