#pragma once
/* ***************************************************************
 *  Copyright (C) 2006 -  Yongming Wang(wangym@gmail.com)
 *  All Rights Reserved
 * ***************************************************************/
#include <zce/zce_object.h>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

class zdl_visitor;
class zdl_module;

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
enum yytokentype
{
	YYEMPTY = -2,
	YYEOF = 0,                     /* "end of file"  */
	YYerror = 256,                 /* error  */
	YYUNDEF = 257,                 /* "invalid token"  */
	ZDL_CHAR = 258,                /* ZDL_CHAR  */
	ZDL_UCHAR = 259,               /* ZDL_UCHAR  */
	ZDL_SHORT = 260,               /* ZDL_SHORT  */
	ZDL_USHORT = 261,              /* ZDL_USHORT  */
	ZDL_INT32 = 262,               /* ZDL_INT32  */
	ZDL_UINT32 = 263,              /* ZDL_UINT32  */
	ZDL_INT64 = 264,               /* ZDL_INT64  */
	ZDL_UINT64 = 265,              /* ZDL_UINT64  */
	ZDL_FLOAT = 266,               /* ZDL_FLOAT  */
	ZDL_DOUBLE = 267,              /* ZDL_DOUBLE  */
	ZDL_ASTRING = 268,             /* ZDL_ASTRING  */
	ZDL_TSTRING = 269,             /* ZDL_TSTRING  */
	ZDL_WSTRING = 270,             /* ZDL_WSTRING  */
	ZDL_SERIAL = 271,              /* ZDL_SERIAL  */
	ZDL_DATETIME = 272,            /* ZDL_DATETIME  */
	ZDL_ASTRVEC = 273,             /* ZDL_ASTRVEC  */
	ZDL_TSTRVEC = 274,             /* ZDL_TSTRVEC  */
	ZDL_WSTRVEC = 275,             /* ZDL_WSTRVEC  */
	ZDL_BYTEVEC = 276,             /* ZDL_BYTEVEC  */
	ZDL_ANY = 277,                 /* ZDL_ANY  */
	ZDL_ENUM = 278,                /* ZDL_ENUM  */
	ZDL_STRUCT = 279,              /* ZDL_STRUCT  */
	ZDL_NAMESPACE = 280,           /* ZDL_NAMESPACE  */
	ZDL_IDENTIFIER = 281,          /* ZDL_IDENTIFIER  */
	ZDL_INTEGER_LITERAL = 282,     /* ZDL_INTEGER_LITERAL  */
	ZDL_DOUBLE_LITERAL = 283,      /* ZDL_DOUBLE_LITERAL  */
	ZDL_STRING_LITERAL = 284,      /* ZDL_STRING_LITERAL  */
	ZDL_COMMENT = 285,             /* ZDL_COMMENT  */
	ZDL_USER_START = 286           /* ZDL_USER_START  */
};
typedef enum yytokentype yytoken_kind_t;
#endif

class meta_base : public zce_object
{

public:
    virtual ~meta_base() {}

    const std::vector<std::pair<std::string, std::string> >& vec_metas() const
    {
        return vec_metas_;
    }

    const char* fetch_meta(const std::string& key, const char* def = NULL) const
    {
        std::vector<std::pair<std::string, std::string> >::const_iterator iter = std::find_if(
            vec_metas_.begin(), vec_metas_.end(), [key](auto iter) {
                return iter.first == key;
            });
        if (iter == vec_metas_.end())
            return def;
        return iter->second.c_str();
    }

    void vec_metas(const std::vector<std::pair<std::string, std::string> >& val) 
    {
        vec_metas_ = val;
        vec_metas_added();
    }

    const std::vector<std::pair<std::string, std::string> >& vec_metas() { return vec_metas_; }

    virtual void vec_metas_added() {};

protected:
    std::vector<std::pair<std::string, std::string> > vec_metas_;
};

class zdl_type : public meta_base
{
protected:
    typedef zce_smartptr<zdl_visitor> zdl_visitor_ptr;
public:

    enum zdl_type_e {
        type_unknow_e,
        type_builtin_e,
        type_enum_e,
        type_struct_e,
    };
    static zdl_type_e get_type_e (int tpid) noexcept;
    
public:
    //tpid : ZDL_CHAR ,ZDL_UCHAR... @TODO CHECK CONV
    zdl_type(int tpid, const zce_smartptr<zdl_module>& module_ptr, const std::string& name);

    ~zdl_type();

    inline const std::string& name() const noexcept {
        return type_name_;
    };

    inline const std::string& full_name() const noexcept {
        return full_name_;
    }

    //type itself indicator is a vector
    inline bool is_vector() const noexcept {
        return (id() >= ZDL_ASTRVEC && id() <= ZDL_BYTEVEC);
    }

    inline const zce_smartptr<zdl_module>& module_ptr() const noexcept {
        return module_ptr_;
    }

    inline int id() const{
        return type_id_;
    };

    inline zdl_type_e type_e() const noexcept {
        return get_type_e(type_id_);
    };

    inline bool is_struct() const noexcept {
        return type_e() == type_struct_e;
    }

    virtual void visit(const zdl_visitor_ptr&) const = 0;
    virtual void visit(const zdl_visitor_ptr&, const std::string&, bool include = true) const {};
protected:
    int         type_id_;
    zce_smartptr<zdl_module> module_ptr_;
    std::string type_name_;
    std::string full_name_;
};
typedef zce_smartptr<zdl_type> zdl_type_ptr;
