#pragma once
// ***************************************************************
//  zce_safemap   version:  1.0     date: 2003/07/31
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2003 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __zce_safemap_h__
#define __zce_safemap_h__

#include <map>
#include <algorithm>

template<typename key_t, 
typename val_t, 
typename lock_t = zce_mutex,
typename map_t = std::map<key_t, val_t> 
>
class zce_safemap
{
public:
    typedef  map_t mapt;
    typedef  typename map_t::iterator map_iter;
    typedef  typename map_t::const_iterator const_map_iter;
    
    int insert(const key_t& key, const val_t& val)
    {
          zce_guard<lock_t> g(lock_);
        map_iter iter = map_.find(key);
        if (iter == map_.end())
        {
            map_.insert(std::make_pair(key, val));
            return 0;
        }
        return -1;
    }
    //return 0 if ok; return >0 if exists; return <0 if insert failed or unknow failed
    int insert(const key_t& key, const val_t& val, val_t& exists_val, bool replace_if_exist = false)
    {
        zce_guard<lock_t> g(lock_);
        map_iter iter = map_.find(key);
        if (iter == map_.end())
        {
            map_.insert(std::make_pair(key, val));
            return 0;
        }
        else
        {
            exists_val = iter->second;
            if (replace_if_exist)
            {
                iter->second = val;
            }
            return 1;
        }
    };

    int query(const key_t& key, val_t& val) const
    {
        zce_guard<lock_t> g(lock_);
        const_map_iter iter = map_.find(key);
        if (iter != map_.end())
        {
            val = iter->second;
            return 0;
        }
        return -1;
    }

    int query_first(val_t& val) const
    {
        zce_guard<lock_t> g(lock_);
        if (!map_.empty())
        {
            val = map_.begin()->second;
            return 0;
        }
        return -1;
    }

    //return 0 if ok; < 0, query failed
    int query(const key_t& key, val_t& val, bool erase_if_exists)
    {
        zce_guard<lock_t> g(lock_);
        map_iter iter = map_.find(key);
        if (iter != map_.end())
        {
            val = iter->second;
            if (erase_if_exists)
                map_.erase(iter);
            return 0;
        }
        return -1;
    }

    int query_first(val_t& val, bool erase_if_exists)
    {
        zce_guard<lock_t> g(lock_);
        if (!map_.empty())
        {
            map_iter iter = map_.begin();
            val = iter->second;
            if (erase_if_exists)
                map_.erase(iter);
            return 0;
        }
        return -1;
    }

    template <typename tdoozer>
    int query_if(val_t& val, bool erase_if_exists, tdoozer doozer)
    {
        zce_guard<lock_t> g(lock_);
        map_iter iter = std::find_if(map_.begin(), map_.end(), doozer);
        if (iter != map_.end())
        {
            val = iter->second;
            if (erase_if_exists)
                map_.erase(iter);
            return 0;
        }
        return -1;
    }

    void erase(const key_t& key)
    {
        zce_guard<lock_t> g(lock_);
        map_.erase(key);
    };

    size_t size() const
    {
        zce_guard<lock_t> g(lock_);
        return map_.size();
    };

    void lock_copy(map_t&  mapt, bool eraseall = false)
    {
        zce_guard<lock_t> g(lock_);
        mapt =  map_;
        if (eraseall)
            map_.clear();
    }

    void clear()
    {
        zce_guard<lock_t> g(lock_);
        map_.clear();
    }

private:
    map_t map_;
    mutable lock_t lock_;
};

#endif // __zce_safemap_h__

