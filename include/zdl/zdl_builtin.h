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
 * @file zdl_builtin.h
 * @brief
 *****************************************************************/

#ifndef __zdl_builtin_h__
#define __zdl_builtin_h__
#pragma once

#include <boost/shared_ptr.hpp>

#include "zdl/zdl_type.h"

class zdl_builtin : public zdl_type
{
public:
    zdl_builtin(int tpid);
    static std::string builtin_name(int tpid);
    static std::string signed_builtin_name(int tpid);
    virtual void visit(const zdl_visitor_ptr&) const ;
};
typedef boost::shared_ptr<zdl_builtin> zdl_builtin_ptr;

#endif /*__zdl_builtin_h__*/
