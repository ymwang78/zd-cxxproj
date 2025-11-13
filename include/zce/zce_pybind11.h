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

template <>
struct type_caster<zce::Any> {
  public:
    PYBIND11_TYPE_CASTER(zce::Any, _("zce::Any"));

    static zce::Any convert_py_to_any(pybind11::handle src) {
        using namespace pybind11;

        if (src.is_none()) {
            return zce::Any((const void*)(nullptr));
        }
        if (isinstance<pybind11::bool_>(src)) {
            return zce::Any(src.cast<bool>());
        }
        if (isinstance<pybind11::int_>(src)) {
            return zce::Any(src.cast<int64_t>());
        }
        if (isinstance<pybind11::float_>(src)) {
            return zce::Any(src.cast<double>());
        }
        if (isinstance<pybind11::str>(src)) {
            return zce::Any(src.cast<std::string>());
        }
        if (isinstance<pybind11::dict>(src)) {
            zce::Any dict = zce::Any::create_dict();
            for (auto item : pybind11::cast<pybind11::dict>(src)) {
                dict.dict().emplace(convert_py_to_any(item.first), convert_py_to_any(item.second));
            }
            return dict;
        }
        if (isinstance<pybind11::sequence>(src)) {
            pybind11::sequence seq = pybind11::cast<pybind11::sequence>(src);
            zce::Any vec = zce::Any::create_vector();
            vec.vector().reserve(seq.size());
            for (auto item : seq) vec.vector().push_back(convert_py_to_any(item));
            return vec;
        }
#ifdef PYBIND11_NUMPY_H
        // numpy 数组（可选）
        else if (py::isinstance<py::array_t<double>>(src)) {
            auto arr = src.cast<py::array_t<double>>();
            std::vector<double> vec(arr.size());
            std::memcpy(vec.data(), arr.data(), arr.size() * sizeof(double));
            value = zce::Any(vec);
        }
#endif
        // 默认fallback
        return zce::Any();
    }

    // 从 Python 到 C++
    bool load(handle src, bool) {
        value = convert_py_to_any(src);
        return true;
    }

    // 从 C++ 到 Python
    static handle cast(const zce::Any& src, return_value_policy, handle) {
        using namespace pybind11;
        if (src.is_none()) return pybind11::none().release();
        if (src.is_boolean()) return pybind11::bool_(src.boolean()).release();
        if (src.is_i64()) return pybind11::int_(src.i64()).release();
        if (src.is_double()) return pybind11::float_(src.dbl()).release();
        if (src.is_string()) return pybind11::str(src.str()).release();

        if (src.is_dict()) {
            const auto& dict = src.dict();
            pybind11::dict d;
            for (auto& kv : dict)
                d[cast(kv.first, return_value_policy::copy, handle())] =
                    cast(kv.second, return_value_policy::copy, handle());
            return d.release();
        }

        if (src.is_vector()) {
            const auto& vec = src.vector();
            pybind11::list lst;
            for (auto& v : vec) lst.append(cast(v, return_value_policy::copy, handle()));
            return lst.release();
        }

        return pybind11::none().release();
    }
};

}  // namespace detail
}  // namespace pybind11

namespace zce {
extern pybind11::object any_to_pyobject(const zce::Any& v);
}  // namespace zce