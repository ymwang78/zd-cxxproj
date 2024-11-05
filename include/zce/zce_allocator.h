#pragma once
// ***************************************************************
//  zce_allocator   version:  1.0   -  date: 2002/04/29
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of ZCE, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// pool: |---chunk---|---chunk---|...|---chunk---|
// pool: |---chunk---|---chunk---|...|---chunk---|
// ***************************************************************
#ifndef __zce_allocator_h__
#define __zce_allocator_h__

#include <zce/zce_object.h>
#include <zce/zce_sync.h>
#include <zce/zce_list.h>
#include <vector>

struct ZCE_API zce_alloc_stat {

    zce_alloc_stat(size_t chunk_s) noexcept;

    zce_alloc_stat() = default;

    inline unsigned current_alloc() const noexcept {
        return chunk_alloc_times_ - chunk_free_times_;
    }

    inline void set_max_alloc() noexcept {
        unsigned cur_alloc = current_alloc();
        if (cur_alloc > chunk_max_used_)
            chunk_max_used_ = cur_alloc;
    }

    unsigned chunk_data_size_;      //the size of chunk
    unsigned chunk_availble_;       //how many chunks available for use
    unsigned chunk_alloc_times_;    //alloc times, for profile
    unsigned chunk_free_times_;     //free times, for profile
    unsigned chunk_max_used_;       //the max number of chunks used in history
};

class ZCE_API zce_allocator : public zce_object
{
public:
    enum ALLOCATOR_TYPE { CHUNK, V2 };

    static zce_smartptr<zce_allocator> create_chunk(size_t chunk_size, size_t n_chunks, bool lock);

    static zce_smartptr<zce_allocator> create_v2(size_t chunk_size, size_t n_chunks, bool lock);

    virtual size_t total_size() const noexcept = 0;

    virtual void get_stat(std::vector<zce_alloc_stat>& stat) const noexcept = 0;

    virtual size_t get_capacity(void* item) const noexcept = 0 ;

    virtual void* alloc(size_t nbytes, size_t* nreal) = 0;

    virtual void zfree(void* item) noexcept = 0; //fuck off the macro

    virtual void* realloc(void* ptr, size_t nbytes, size_t* nreal) noexcept = 0;

    virtual ALLOCATOR_TYPE get_type() const noexcept = 0;
};

#endif // __zce_allocator_h__
