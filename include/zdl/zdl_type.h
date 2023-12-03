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
 * @file zdl_type.h
 * @brief
 *****************************************************************/

#ifndef __zdl_type_h__
#define __zdl_type_h__

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <algorithm>
#include <map>
class zdl_visitor;

struct match_pair_key
{
    std::string value_;
    match_pair_key(const std::string& first)
        :value_(first)
    {
    }

    bool operator()(const std::pair<std::string, std::string>& val)
    {
        return val.first == value_;
    }
};

class meta_base
{

public:
    virtual ~meta_base() {}

    const std::vector<std::pair<std::string, std::string> >& vec_metas() const
    {
        return vec_metas_;
    }

    const char* fetch_meta(const std::string& key, const char* def = NULL) const
    {
        std::vector<std::pair<std::string, std::string> >::const_iterator iter = std::find_if(vec_metas_.begin(), vec_metas_.end(), match_pair_key(key));
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
    typedef boost::shared_ptr<zdl_visitor> zdl_visitor_ptr;
public:

    enum zdl_type_e {
        type_unknow_e,
        type_builtin_e,
        type_builtinvec_e,
        type_enum_e,
        type_struct_e,
    };
    static zdl_type_e get_type_e (int tpid);
    
public:
    //tpid : UIDL_CHAR ,UIDL_UCHAR... @TODO CHECK CONV
    zdl_type(int tpid, const std::string& name)
        :type_id_(tpid), type_name_(name){
    };
    const std::string& name() const{
        return type_name_;
    };
    int id() const{
        return type_id_;
    };
    zdl_type_e type_e() const{
        return get_type_e(type_id_);
    };

    virtual void visit(const zdl_visitor_ptr&) const = 0;
    virtual void visit(const zdl_visitor_ptr&, const std::string&, bool include = true) const {};
private:
    int         type_id_;
    std::string type_name_;
};
typedef boost::shared_ptr<zdl_type> zdl_type_ptr;

class zdl_visitor;
class zdl_type_container
{
    typedef boost::shared_ptr<zdl_visitor> zdl_visitor_ptr;

public:
    zdl_type_container();
    int add_type(const zdl_type_ptr& type_ptr);
    zdl_type_ptr get_type(const std::string& name);
    zdl_type_ptr get_builtin_type(int tpid);

    void visit(const zdl_visitor_ptr&) const;
    void visit(const zdl_visitor_ptr&, const std::string&, bool include = true) const;
    void visit_type_meta(const zdl_visitor_ptr&, const std::string&, bool include = true) const;

    template <class tfunc>
    void sort_me(tfunc func) {
        sort(types_.begin(), types_.end(), func);
    }

    const std::vector<zdl_type_ptr>& types() const {
		return types_;
	}

	const std::map<std::string, std::map<std::string, zdl_type_ptr> > ns_types() const {
		return ns_types_;
	}

private:
    //std::map<std::string, zdl_type_ptr> types_;
    std::vector<zdl_type_ptr> types_;

public:
    zdl_type_ptr create_ns_type(const std::string& ns, const std::string& name, zdl_type::zdl_type_e e);

private:
    //std::map<std::string, zdl_type_ptr> ns_types_;
	std::map<std::string, std::map<std::string, zdl_type_ptr> > ns_types_;
};


#endif /*__zdl_type_h__*/
