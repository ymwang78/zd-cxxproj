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
 * @file zdl_lex.h
 * @brief
 *****************************************************************/

#ifndef __zdl_lex_h__
#define __zdl_lex_h__

#include <string>
#include <map>
#include <zce/zce_object.h>

class zdl_keyword : public zce_object
{
public:
    zdl_keyword(const char* name, int id);
    int id() const{
        return id_;
    };
    const std::string& name() const{
        return name_;
    };
    void payload(void* p){
        payload_ = p;
    };
    void* payload(){
        return payload_;
    };
private:
    std::string name_;
    int id_;
    void* payload_;
};
typedef zce_smartptr<zdl_keyword> zdl_keyword_ptr;

class zdl_keyword_map : public zce_object
{
public:
    zdl_keyword_map();
    ///return keyword id if is, else return ID_IDENTIFIER
    int get_identifier_by_name(const char* name);
    int get_identifier_by_name(const std::string& name);

private:
    void add_new_keyword(const char* name, int id);
private:
    std::map<std::string, zdl_keyword_ptr> name_keyword_map_;
    std::map<int, zdl_keyword_ptr>         id_keyword_map_;
};
typedef zce_smartptr<zdl_keyword_map> zdl_keyword_map_ptr;

extern zdl_keyword_map_ptr g_keyword_map_ptr;

#endif /*__zdl_lex_h__*/
