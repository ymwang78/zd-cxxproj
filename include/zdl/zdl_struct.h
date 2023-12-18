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
 * @file zdl_struct.h
 * @brief
 *****************************************************************/

#ifndef __zdl_struct_h__
#define __zdl_struct_h__

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "zdl/zdl_type.h"
#include "zdl/zdl_template_arg.h"
#include "zdl/zdl_module.h"

class zdl_member;
class zdl_visitor;


class zdl_struct : public zdl_type
{

    typedef zce_smartptr<zdl_member>  zdl_member_ptr;
    typedef zce_smartptr<zdl_visitor> zdl_visitor_ptr;
public:
    zdl_struct(const zdl_module_ptr& module_ptr, const std::string& name);
    ~zdl_struct();

    const zdl_module_ptr& module_ptr() const { return module_ptr_; }

    void add_template_arg(const std::string& arg);
    int add_member(const zdl_member_ptr& member);
    unsigned refcount() { return refcount_; }
    void addrefcount() { ++ refcount_; };
    virtual void visit(const zdl_visitor_ptr&) const;
    virtual void visit(const zdl_visitor_ptr&, const std::string& meta, bool include) const;
    template <typename predicate>
    void visit(const zdl_visitor_ptr&, predicate p) const;

    int get_member_count() const;
    int get_member_count(const std::string& meta) const;
    bool empty() const { return members_.empty();}
    void comment(const std::string& cmt) 
    { 
        int state = 0;
        for (unsigned i=0; i<cmt.size(); ++i)
        {
            switch (state)
            {
            case 0:
                if (cmt[i] == '[')
                    state = 1;
                break;
            case 1:
                if (cmt[i] == ']')
                {
                    state = 2;
                    break;
                }
                comment_mask_ += cmt[i];
                break;
            default:
                break;
            }
            if (state >= 2)
                break;
        }
        comment_ = cmt; 
    }
    const std::string& comment() const { return comment_; }
    const std::string& comment_mask() const { return comment_mask_; }
    int arg_size() const { return (int) args_.size(); }
    int nonarray_template_member_size() const { return (int) template_nonarray_members_.size(); }

    template <typename visitor>
    void visit_args(visitor& v) const
    {
        std::for_each(args_.begin(), args_.end(), v);
    }

    template <typename visitor>
    void visit_nonarray_template_member(visitor& v) const
    {
        std::for_each(template_nonarray_members_.begin(), template_nonarray_members_.end(), v);
    }
    const std::vector<zdl_member_ptr>& meta_member_vec(const std::string& meta_key) const
    {
        static std::vector<zdl_member_ptr> empty_vec;
        std::map<std::string, std::vector<zdl_member_ptr> >::const_iterator iter = meta_member_map_.find(meta_key);
        if  (iter !=  meta_member_map_.end())
            return iter->second;
        return empty_vec;
    }
    const std::vector<zdl_member_ptr>& member_vec() const { return members_; }

    virtual void vec_metas_added()
    {
        if (name().substr(0, 3) == "MSG")
        {
            vec_metas_.push_back(std::make_pair("msg", "1"));
        }
    }
private:
    std::vector<zdl_template_arg_ptr> args_;
    std::vector<zdl_member_ptr> members_;
    std::map<std::string, std::vector<zdl_member_ptr> > meta_member_map_;
    std::string comment_;
    std::string comment_mask_;
    std::vector<zdl_member_ptr> template_nonarray_members_;
    unsigned refcount_;
};

typedef zce_smartptr<zdl_struct>  zdl_struct_ptr;

template <typename predicate>
void zdl_struct::visit(const zdl_visitor_ptr& visitor, predicate p) const
{
    visitor->visit_struct_start(this);
    int count = 0;
    std::vector<zdl_member_ptr>::const_iterator iter;
    for (iter = members_.begin(); iter != members_.end(); ++iter)
    {
        if (p(*iter))
        {
            ++count;
        }
    }
    for (iter = members_.begin(); iter != members_.end(); ++iter)
    {
        if (p(*iter))
        {
            --count;
            visitor->visit_member((*iter).get(), !count);
        }
    }
    visitor->visit_struct_end(this);
}

#endif /*__zdl_struct_h__*/
