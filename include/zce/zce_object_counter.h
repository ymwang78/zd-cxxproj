// ***************************************************************
//  zce_object_counter   version:  1.0     date: 07/31/2002
//  -------------------------------------------------------------
//  Yongming Wang(ymwang@iipc.zju.edu.cn)
//  The Institute of System Engineering, Zhejiang University
//  -------------------------------------------------------------
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 实现精细粒度对象统计
// ***************************************************************
#ifndef __zce_object_counter_h__
#define __zce_object_counter_h__

#include <zce/zce_config.h>
#include <vector>
#include <zce/zce_smartptr.h>
#include <zce/zce_singleton.h>
#include <zce/zce_atomic.h>

class zce_atomic_long;

class ZCE_API zce_monitor
{
    class Pimpl;
    Pimpl* pimpl_ptr_;
public:
    struct object_stat
    {
        const char*    name;
        unsigned    alloc;
        unsigned    free;
    };
    zce_monitor();
    virtual ~zce_monitor();
    void register_object(const char* name, zce_atomic_long*&, zce_atomic_long*&);
    /*name: the name to query, NULL if query all; ret:0 if not found, >0 the number, <0 error*/
    int get_objects(const char* name, std::vector<object_stat>& vec);
};

class ZCE_API zce_monitor_sigt : public zce_singleton<zce_monitor, zce_mutex_null>
{
    zce_monitor_sigt();
    ~zce_monitor_sigt();
};

class ZCE_API zce_object_counter
{
    zce_object_counter();
public:
    zce_object_counter(const char* name);
    virtual ~zce_object_counter();
    inline void inc_ref() { ++(*alloc_ref_ptr_); };
    inline void dec_ref() { ++(*free_ref_ptr_); };
protected:
    zce_atomic_long* alloc_ref_ptr_;
    zce_atomic_long* free_ref_ptr_;
};

class ZCE_API zce_object_counter_proxy
{
public:
    zce_object_counter_proxy(zce_object_counter& obj);
    zce_object_counter_proxy(const zce_object_counter_proxy& rhs);
    ~zce_object_counter_proxy();
    const zce_object_counter& object() { return object_; }
    zce_object_counter_proxy& operator=(const zce_object_counter_proxy& rhs);
private:
    zce_object_counter& object_;
};

#endif /*__zce_object_counter_h__*/
