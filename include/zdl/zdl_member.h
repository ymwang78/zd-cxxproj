#pragma once
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

#include <string>
#include <zce/zce_object.h>
#include <zce/zce_object_counter.h>
#include "zdl/zdl_visitor.h"
#include "zdl/zdl_type.h"
#include "zdl/zdl_template_arg.h"
#include "zdl/zdl_member.h"

// class zdl_visitor;
class zdl_struct;
class zdl_type;

class zdl_member : public meta_base {

    ZCE_OBJECT_DECLARE;

  public:
    enum member_type_e {
        implement_e,
        array_e,
    };

    zdl_member(zdl_struct* parent, int var_idx, const std::string& var, const std::string& minsize,
               const std::string& maxsize, zdl_type* type = 0);

    bool is_array() const noexcept { return (member_type_e_ == array_e); }

    bool is_array_optional() const noexcept { return (calc_min_size() == 0 && is_array()); }

    bool is_fixsize() const noexcept { return min_size_ == max_size_ && min_size_ != "0"; }

    bool is_optional() const noexcept {
        if (is_array())
            return (calc_min_size() == 0);
        else {
            std::vector<std::pair<std::string, std::string> >::const_iterator iter;
            iter = std::find_if(
                vec_metas_.begin(), vec_metas_.end(),
                [](const std::pair<std::string, std::string>& val) { return val.first == "o"; });
            if (iter != vec_metas_.end()) return true;
            iter = std::find_if(
                vec_metas_.begin(), vec_metas_.end(),
                [](const std::pair<std::string, std::string>& val) { return val.first == "O"; });
            if (iter != vec_metas_.end()) return true;
            iter = std::find_if(vec_metas_.begin(), vec_metas_.end(),
                                [](const std::pair<std::string, std::string>& val) {
                                    return val.first == "_default_val";
                                });
            if (iter != vec_metas_.end()) return true;
        }
        return false;
    };

    bool is_string() const noexcept;

    zdl_struct* parent() const noexcept { return parent_; };

    zdl_type* type() const noexcept { return type_; };

    void type(zdl_type* t) noexcept { type_ = t; };

    int var_idx() const noexcept { return var_idx_; }

    const std::string& var_name() const { return var_name_; };

    void comment(const std::string& comm) { comment_ = comm; }

    const std::string& comment() const { return comment_; }

    virtual void vec_metas_added() {
        long max = calc_max_size();
        long min = calc_min_size();
        if (is_array() && min == 0) vec_metas_.push_back(std::make_pair("o", ""));
    }

    std::string max_size() const {
        if (max_size_[0] <= '9' && max_size_[0] >= '0') return max_size_;
        return std::string("_t.") + max_size_;
    };

    std::string min_size() const {
        if (min_size_[0] <= '9' && min_size_[0] >= '0') return min_size_;
        return std::string("_t.") + min_size_;
    };

    int calc_max_size() const {
        char* endp = 0;
        long val = strtol(max_size_.c_str(), &endp, 0);
        if (endp != 0 && *endp == 0) return val;
        return -1;
    }

    int calc_min_size() const {
        char* endp = 0;
        long val = strtol(min_size_.c_str(), &endp, 0);
        if (endp != 0 && *endp == 0) return val;
        return -1;
    }

    void add_template_arg(const std::string& arg);
    int arg_size() const { return (int)args_.size(); }

    template <typename visitor>
    void visit_args(visitor& v) const {
        std::for_each(args_.begin(), args_.end(), v);
    }

  private:
    zdl_struct* parent_;
    int var_idx_;
    std::string var_name_;
    std::string comment_;

    zdl_type* type_;
    member_type_e member_type_e_;
    std::string min_size_;
    std::string max_size_;

    std::vector<zdl_template_arg_ptr> args_;
};

typedef zce_smartptr<zdl_member> zdl_member_ptr;
