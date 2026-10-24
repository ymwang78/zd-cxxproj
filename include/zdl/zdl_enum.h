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

#include <vector>
#include <string>
#include <zce/zce_object.h>
#include <zdl/zdl_type.h>

class zdl_visitor;
class zdl_enum;

struct zdl_enumerator : public zce::Object {
    zdl_enumerator(zdl_enum* enum_t, const std::string& name, unsigned long val,
                   const std::string& comment)
        : enum_(enum_t), name_(name), val_(val), comment_(comment){};
    zdl_enum* enum_;
    std::string name_;
    unsigned val_;  // 0 .. 0xFFFFFFFF; generated as (int) in enum X : int, see ZDL_PROTOCOL.md
    std::string comment_;
};
typedef zce::SmartPtr<zdl_enumerator> zdl_enumerator_ptr;

class zdl_enum : public zdl_type {
    typedef zce::SmartPtr<zdl_visitor> zdl_visitor_ptr;

  public:
    zdl_enum(const zce::SmartPtr<zdl_module>& module_ptr, const std::string& name);
    const std::vector<zdl_enumerator_ptr>& enumerators() const { return enumerators_; };
    // Both overloads return
    //    0  the enumerator was added;
    //   -1  an enumerator with that name already exists;
    //   -2  the value does not fit in val_ (0 .. 0xFFFFFFFF): an explicit val above
    //       0xFFFFFFFF, or an implicit value after 0xFFFFFFFF. Nothing is added and
    //       the value is neither truncated nor wrapped to 0.
    // Where unsigned long is 32 bits (Windows) val cannot exceed 0xFFFFFFFF, so a
    // caller that parsed a wider literal must range-check it before narrowing.
    int add_enumerator(unsigned long val, const std::string& emname, const std::string& comment);
    // Uses the value after the last enumerator added, or 0 for the first one.
    int add_enumerator(const std::string& emname, const std::string& comment);

    zdl_enumerator_ptr find_enumerator(const std::string& emname);

  public:
    virtual void visit(const zdl_visitor_ptr&) const;

  private:
    std::vector<zdl_enumerator_ptr> enumerators_;
    // Wider than val_ so that the value after 0xFFFFFFFF is 0x100000000, not 0.
    unsigned long long current_enumerator_val_;
};
typedef zce::SmartPtr<zdl_enum> zdl_enum_ptr;

