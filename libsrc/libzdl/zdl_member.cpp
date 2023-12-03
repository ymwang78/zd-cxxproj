// ***************************************************************
//  zdl_member   version:  1.0   date: 04/05/2007
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include <zce/zce_config.h>
#include "zdl/zdl_member.h"
#include "zdl/zdl_struct.h"
#include "zdl/zdl_parser.h"
#include "zdl/zdl_parser_context.h"
#include "zdl_grammar.tab.hpp"
#pragma warning (push, 3)
#include <boost/tokenizer.hpp>
#pragma warning (pop)

bool zdl_member::is_string() const
{
    return (type()->id() >= UIDL_ASTRING && type()->id() <= UIDL_WSTRING);
}


void zdl_member::add_template_arg(const std::string& arg)
{
    if(type_->type_e() != zdl_type::type_struct_e)
        return;

    boost::char_separator<char> sep(",");
    boost::tokenizer< boost::char_separator<char> > tokens(arg, sep);

    for (boost::tokenizer< boost::char_separator<char> >::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        std::string strPart = *it;
        zdl_template_arg_ptr arg_ptr(new zdl_template_arg(strPart));
        args_.push_back(arg_ptr);
    }

    zdl_struct* tp = dynamic_cast<zdl_struct*>(type_);
    if (tp->arg_size() != args_.size())
    {
        ZCE_ERROR((ZLOG_ERROR, "Member's template args not mach with the type's\n"));
        return;
    }
}