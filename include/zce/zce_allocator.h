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

    zce_alloc_stat(size_t chunk_s);

    zce_alloc_stat() = default;

    inline unsigned current_alloc() const noexcept {
        return chunk_alloc_times_ - chunk_free_times_;
    }

    inline void set_max_alloc() noexcept {
        unsigned cur_alloc = current_alloc();
        if (cur_alloc > chunk_max_used_)
            chunk_max_used_ = cur_alloc;
    }

    unsigned chunk_data_size_;           //the size of chunk
    unsigned chunk_availble_;       //how many chunks available for use
    unsigned chunk_alloc_times_;    //alloc times, for profile
    unsigned chunk_free_times_;     //free times, for profile
    unsigned chunk_max_used_;      //the max number of chunks used in history
};

class ZCE_API zce_allocator : public zce_object
{
public:
    virtual void* alloc(size_t nbytes, size_t* nreal) = 0;

    virtual void zfree(void* item) = 0; //fuck off the macro
};

class ZCE_API zce_allocator_chunk : public zce_allocator
{

private:
    bool lock_;                     //need cross thread usage
    
    zce_mutex pool_lock_;           //protected alloc and free if lock is enabled
    
    unsigned long work_threadid_;   //debug purpose only

    unsigned chunk_count_;          //how many chunks in one pool

    zce_alloc_stat alloc_stat_;

    std::vector<char*> pool_vec_;       //pools

    struct zce_list_node free_list_;    //available chunks

    void add_new_pool();

    void* do_malloc(size_t& nbytes);

    void do_free(void* item);

public:

    inline unsigned chunk_size() const noexcept { return alloc_stat_.chunk_data_size_; }

    inline zce_alloc_stat stat() const noexcept { return alloc_stat_; }

public:

    zce_allocator_chunk(size_t chunk_size, size_t n_chunks, bool lock);

    virtual ~zce_allocator_chunk();

    void *alloc(size_t nbytes, size_t* nreal) override;

    void zfree(void* item) override; //fuck off the macro
};

#endif // __zce_allocator_h__
