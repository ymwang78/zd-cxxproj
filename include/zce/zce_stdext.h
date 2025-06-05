#pragma once
// ***************************************************************
//  zce_stdext   version:  1.0   -  date:  2025/06/05
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project ZCE.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#pragma once
#include <string>
#include <string_view>
#include <unordered_map>

namespace zce {

#if __cplusplus >= 202002L

struct transparent_string_hash {
    using is_transparent = void;

    size_t operator()(std::string_view txt) const noexcept {
        return std::hash<std::string_view>{}(txt);
    }

    size_t operator()(const std::string& txt) const noexcept {
        return std::hash<std::string_view>{}(txt);
    }

    size_t operator()(const char* txt) const noexcept { return std::hash<std::string_view>{}(txt); }
};

struct transparent_string_equal {
    using is_transparent = void;

    bool operator()(std::string_view lhs, std::string_view rhs) const noexcept {
        return lhs == rhs;
    }

    bool operator()(const std::string& lhs, std::string_view rhs) const noexcept {
        return lhs == rhs;
    }

    bool operator()(std::string_view lhs, const std::string& rhs) const noexcept {
        return lhs == rhs;
    }

    bool operator()(const char* lhs, std::string_view rhs) const noexcept {
        return std::string_view(lhs) == rhs;
    }

    bool operator()(std::string_view lhs, const char* rhs) const noexcept {
        return lhs == std::string_view(rhs);
    }
};

template <typename T>
using unordered_stringmap =
    std::unordered_map<std::string, T, transparent_string_hash, transparent_string_equal>;

#else

template <typename T>
using unordered_stringmap =
    std::unordered_map<std::string, T>;

#endif

}  // namespace zce
