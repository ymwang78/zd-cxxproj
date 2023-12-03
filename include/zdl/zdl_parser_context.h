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
 * @file zdl_parser_context.h
 * @brief
 *****************************************************************/

#ifndef __zdl_parser_context_h__
#define __zdl_parser_context_h__

#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>

class zdl_type_container;
class zdl_enum;
class zdl_struct;
class zdl_member;
class zdl_module;
class zdl_parser;
class zdl_type;

class zdl_parser_context
{
    typedef boost::shared_ptr<zdl_type_container> zdl_type_container_ptr;
    typedef boost::shared_ptr<zdl_enum>        zdl_enum_ptr;
    typedef boost::shared_ptr<zdl_struct>      zdl_struct_ptr;
    typedef boost::shared_ptr<zdl_member>      zdl_member_ptr;
    typedef boost::shared_ptr<zdl_module>      zdl_module_ptr;
    //typedef boost::shared_ptr<zdl_parser>      zdl_parser_ptr;
    typedef boost::shared_ptr<zdl_type>        zdl_type_ptr;

public:
    zdl_parser_context(const char* ns = "");

    void parser(zdl_parser* parser){
        parser_ = parser;
    };

    zdl_parser* parser(){
        return parser_;
    };

    ///implemention
    //void add_member_start(const std::string& varname);
    ///array [minsize,maxsize], if unknow(pointer) maxsize == 0
    void add_member_start(const std::string& varname, const std::string& minsize, const std::string& maxsize);
    void add_member_end(const std::string& varname);

    void set_current_member_type(int tpid);
    void set_current_member_type(int tpid, const std::string& tparg, const std::string& tpname, const std::string& ns = "");
    void add_current_member_metadata(const std::string& meta_key, const std::string& meta_val);

    void add_enumerator(unsigned long val, const std::string& emname, const std::string& comment);
    void add_enumerator(const std::string& emname, const std::string& comment);


    void add_type_start(int tpid, const std::string& tpname);
    void add_type_end(const std::string& tpname);
    void set_comment(const std::string& comment);

    void set_namespace(const std::string& ns);

    void add_type_template_arg(const std::string& argname);
    void add_member_template_arg(const std::string& arg);

    const zdl_module_ptr& module_ptr() { return module_ptr_; };
private:
    zdl_struct_ptr         current_struct_;
    zdl_enum_ptr           current_enum_;
    zdl_member_ptr         current_member_;
    zdl_type_ptr           current_member_type_;
    std::string            current_member_arg_;
    zdl_module_ptr         current_module_;
    std::vector<std::pair<std::string, std::string> > current_metas_;
    zdl_module_ptr         module_ptr_;
    zdl_parser*            parser_;
};

typedef boost::shared_ptr<zdl_parser_context> zdl_parser_context_ptr;

#endif /*__zdl_parser_context_h__*/
