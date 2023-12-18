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
/*****************************************************************
 * @file zdl_enum.h
 * @brief
 *****************************************************************/

#ifndef __zdl_enum_h__
#define __zdl_enum_h__

#include <vector>
#include <string>
#include <zce/zce_object.h>
#pragma once

#include <zdl/zdl_type.h>

class zdl_visitor;
class zdl_enum;

struct zdl_enumerator : public zce_object
{    
    zdl_enumerator(zdl_enum* enum_t, const std::string& name, unsigned long val, const std::string& comment)
        :enum_(enum_t), name_(name), val_(val), comment_(comment){
    };
    zdl_enum*  enum_;
    std::string name_;
    unsigned val_;
    std::string comment_;
};
typedef zce_smartptr<zdl_enumerator> zdl_enumerator_ptr;


class zdl_enum : public zdl_type
{
    typedef zce_smartptr<zdl_visitor> zdl_visitor_ptr;
public:
    zdl_enum(const zce_smartptr<zdl_module>& module_ptr, const std::string& name);
    const std::vector<zdl_enumerator_ptr>& enumerators() const{
        return enumerators_;
    };
    int add_enumerator(unsigned long val, const std::string& emname, const std::string& comment);
    int add_enumerator(const std::string& emname, const std::string& comment);

    zdl_enumerator_ptr find_enumerator(const std::string& emname);

public:
    virtual void visit(const zdl_visitor_ptr&) const;
    
private:
    std::vector<zdl_enumerator_ptr> enumerators_;
    int current_enumerator_val_;
};
typedef zce_smartptr<zdl_enum> zdl_enum_ptr;


#endif /*__zdl_enum_h__*/
