#pragma once
// ***************************************************************
//  zce::BlockPool   version:  1.0   -  date: 2012/4/30
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2012 - All Rights Reserved
// ***************************************************************
// add_pool 非线程安全，必须在初始化时全部add完成，其他函数安全
// ***************************************************************
#include <zce/zce_object_counter.h>
#include <zce/zce_sync.h>
#include <zce/zce_allocator.h>
#include <map>
#include <zce/zce_singleton.h>

namespace zce {

class Allocator;
class DataBlock;
class RefBlock;

class ZCE_API BlockPool
{
    std::map<unsigned, zce::SmartPtr<zce::Allocator> >* allocators_;

    zce::SmartPtr<zce::Allocator> dtblock_allocator_;

    zce::SmartPtr<zce::Allocator> dbblock_allocator_;

    zce::AllocStat outpool_alloc_;

public:

    BlockPool();

    virtual ~BlockPool();

    void add_pool(size_t size, size_t count);

    void add_pool_v2(size_t atomic_size, size_t count);

    zce::SmartPtr<zce::Allocator> get_v2() const;

    zce::DataBlock* acquire(size_t len, zce::ObjectCounter& obj);

    zce::RefBlock acquire_dblock(size_t len, zce::ObjectCounter& obj);

    void* zmalloc(size_t len, size_t* nreal);

    void zfree(void* ptr);

    void* realloc(void* ptr, size_t len, size_t* nreal);

    void getStat(std::vector<zce::AllocStat>& stat) const;
};

template <typename T>
class ObjectPool
{
    zce::SmartPtr<zce::Allocator> vt_allocator_;

public:

    ObjectPool()
        :vt_allocator_( zce::Allocator::createChunk(sizeof(T), 4096, true)) {
    }

    virtual ~ObjectPool() {
    };

    template<typename V>
    T* aquire(const V& v) {
        T* ptr = new (vt_allocator_->alloc(sizeof(T), 0)) T(v);
        if (ptr)
            ptr->__set_allocator(vt_allocator_);
        return ptr;
    }
};

typedef zce::Singleton<zce::BlockPool> BlockPoolSigt;

}  // namespace zce

#define ZCE_MBACQUIRE(RET, x) do{ \
    static zce::ObjectCounter obj(__FUNCTION__); \
    RET = zce::BlockPoolSigt::instance()->acquire_dblock(x, obj);\
}while(0)
