/* ***************************************************************
 *  Copyright (C) 2006  Yongming Wang(wangym@gmail.com)
 *  All Rights Reserved
 *
 *  This file is part of Ubeda project (http://www.ubeda.cn).
 *
 *  This copy of file is licensed to you under Ubeda License.
 *  You should have received a copy of the Ubeda License
 *  along with this program, if not, get it from
 *      http://www.ubeda.cn
 * ***************************************************************/
#pragma once

#include <zce/zce_object.h>
#include <vector>

#include "zdl/zdl_type.h"

struct zdl_builtin_spec {
    int token_id;
    const char* zdl_name;
    const char* cpp_type;
    const char* signed_cpp_type;
    bool is_vector_like;
};

class zdl_builtin : public zdl_type {
  public:
    zdl_builtin(int tpid);
    static const zdl_builtin_spec* builtin_spec(int tpid) noexcept;
    static const std::vector<zdl_builtin_spec>& builtin_specs();
    static std::string builtin_name(int tpid);
    static std::string signed_builtin_name(int tpid);
    virtual void visit(const zdl_visitor_ptr&) const;
};
typedef zce::SmartPtr<zdl_builtin> zdl_builtin_ptr;
