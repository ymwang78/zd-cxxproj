#pragma once
// ***************************************************************
//  ObjectCounter   version:  1.0     date: 07/31/2002
//  -------------------------------------------------------------
//  Yongming Wang(ymwang@iipc.zju.edu.cn)
//  The Institute of System Engineering, Zhejiang University
//  -------------------------------------------------------------
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 实现精细粒度对象统计
// ***************************************************************
#include <zce/zce_config.h>
#include <vector>
#include <zce/zce_object.h>
#include <zce/zce_singleton.h>
#include <zce/zce_atomic.h>

namespace zce {

class ZCE_API AtomicLong;

class ZCE_API ObjectMonitor {
    class Pimpl;
    Pimpl* pimpl_ptr_;

  public:
    struct object_stat {
        const char* name;
        unsigned alloc;
        unsigned free;
    };
    ObjectMonitor();
    virtual ~ObjectMonitor();
    void register_object(const char* name, AtomicLong*&, AtomicLong*&);
    /*name: the name to query, NULL if query all; ret:0 if not found, >0 the number, <0 error*/
    int get_objects(const char* name, std::vector<object_stat>& vec);
};

class ZCE_API ObjectMonitorSigt : public Singleton<ObjectMonitor, MutexNull> {
    ObjectMonitorSigt();
    ~ObjectMonitorSigt();
};

class ZCE_API ObjectCounter {
    ObjectCounter();

  public:
    ObjectCounter(const char* name);
    virtual ~ObjectCounter();
    inline void inc_ref() { ++(*alloc_ref_ptr_); };
    inline void dec_ref() { ++(*free_ref_ptr_); };

  protected:
    AtomicLong* alloc_ref_ptr_;
    AtomicLong* free_ref_ptr_;
};

class ZCE_API ObjectCounterProxy {
  public:
    ObjectCounterProxy(ObjectCounter& obj);
    ObjectCounterProxy(const ObjectCounterProxy& rhs);
    ~ObjectCounterProxy();
    const ObjectCounter& object() { return object_; }
    ObjectCounterProxy& operator=(const ObjectCounterProxy& rhs);

  private:
    ObjectCounter& object_;
};

}  // namespace zce
