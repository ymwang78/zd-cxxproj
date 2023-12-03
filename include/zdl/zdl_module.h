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
 * @file zdl_module.h
 * @brief
 *****************************************************************/

#ifndef __zdl_module_h__
#define __zdl_module_h__

#include <zce/zce_object.h>
#include <zce/zce_object.h>

class zdl_type_container;

class zdl_module : public zce_object
{
    typedef zce_smartptr<zdl_type_container> zdl_type_container_ptr;

    std::string namespace_name_;
    zdl_type_container_ptr type_container_;

public:
    zdl_module(const std::string& ns)
        :namespace_name_(ns), type_container_(new zdl_type_container())
    {
    };
    const zdl_type_container_ptr& type_container() const
    {
        return type_container_;
    }
    const std::string& name_space() 
    {
        return namespace_name_;
    }
};

typedef zce_smartptr<zdl_module> zdl_module_ptr;

#endif /*__zdl_module_h__*/
