#pragma once
// ***************************************************************
//  zce_mbpool   version:  1.0   -  date: 2012/4/30
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2012 - All Rights Reserved
// ***************************************************************
// add_pool 非线程安全，必须在初始化时全部add完成，其他函数安全
// ***************************************************************
#ifndef __zce_mbpool_h__
#define __zce_mbpool_h__

#include <zce/zce_object_counter.h>
#include <zce/zce_sync.h>
#include <zce/zce_allocator.h>
#include <map>

class zce_dtblock;
class zce_dblock;
class zce_allocator;

class ZCE_API zce_mbpool
{
    std::map<unsigned, zce_smartptr<zce_allocator> >* allocators_;

    zce_smartptr<zce_allocator> dtblock_allocator_;

    zce_smartptr<zce_allocator> dbblock_allocator_;

    zce_alloc_stat outpool_alloc_;

public:

    zce_mbpool();

    virtual ~zce_mbpool();

    void add_pool(size_t size, size_t count);

    void add_pool_v2(size_t atomic_size, size_t count);

    zce_dtblock* acquire(size_t len, zce_object_counter& obj);

    zce_dblock acquire_dblock(size_t len, zce_object_counter& obj);

    void* zmalloc(size_t len, size_t* nreal);

    void zfree(void* ptr);

    void* realloc(void* ptr, size_t len, size_t* nreal);

    void get_stat(std::vector<zce_alloc_stat>& stat) const;
};

template <typename T>
class zce_objpool
{
    zce_smartptr<zce_allocator> vt_allocator_;

public:

    zce_objpool()
        :vt_allocator_(zce_allocator::create_chunk(sizeof(T), 4096, true)) {
    }

    virtual ~zce_objpool() {
    };

    template<typename V>
    T* aquire(const V& v) {
        T* ptr = new (vt_allocator_->alloc(sizeof(T), 0)) T(v);
        if (ptr)
            ptr->__set_allocator(vt_allocator_);
        return ptr;
    }
};

#include <zce/zce_singleton.h>
typedef zce_singleton<zce_mbpool> zce_mbpool_sigt;

#define ZCE_MBACQUIRE(RET, x) do{ \
    static zce_object_counter obj(__FUNCTION__); \
    RET = zce_mbpool_sigt::instance()->acquire_dblock(x, obj);\
}while(0)

#endif // __zce_mbpool_h__
