#pragma once
// ***************************************************************
//  uri   version:  1.0   -  date:  2016/04/11
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project ZCE.
//  Copyright (C) 2016 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include <string>
#include <string_view>

namespace zce {

class uri {
  public:
    explicit uri(const std::string& uri_str);

    ~uri();

    bool has_scheme() const noexcept;

    std::string_view scheme() const noexcept;

    bool has_host() const noexcept;

    std::string_view host() const noexcept;

    bool has_port() const noexcept;

    std::string_view port() const noexcept;

    bool has_user_info() const noexcept;

    std::string_view user_info() const noexcept;

    bool has_path() const noexcept;

    std::string_view path() const noexcept;

    bool uri::has_query() const noexcept;

    std::string_view uri::query() const noexcept;

    bool uri::has_fragment() const noexcept;

    std::string_view uri::fragment() const noexcept;

    std::vector<std::pair<std::string_view, std::string_view>> query_items() const noexcept;
  private:
    struct Impl;

    Impl* impl_;


};

}  // namespace zce