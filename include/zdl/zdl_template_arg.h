// ***************************************************************
//  zdl_template_arg   version:  1.0   date: 04/05/2007
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once
#ifndef __zdl_template_arg_h__
#define __zdl_template_arg_h__

#include <zce/zce_object.h>
#include <string>

class zdl_template_arg : public zce_object
{
public:
    zdl_template_arg(const std::string& name)
        :name_(name)
    {

    };
    const std::string& name() const { return name_;}
private:
    std::string name_;
};
typedef zce_smartptr<zdl_template_arg> zdl_template_arg_ptr;

#endif /*__zdl_template_arg_h__*/
