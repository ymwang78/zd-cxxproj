// ***************************************************************
//  zce_allocator   version:  1.0   -  date: 4/29/2012
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// pool: |---chunk---|---chunk---|...|---chunk---|
// pool: |---chunk---|---chunk---|...|---chunk---|
// ***************************************************************
#pragma once
#ifndef __zce_allocator_h__
#define __zce_allocator_h__

#include <zce/zce_object.h>
#include <zce/zce_sync.h>
#include <zce/zce_list.h>
#include <vector>

struct ZCE_API zce_alloc_stat {

    zce_alloc_stat(size_t chunk_s, size_t n_chunks);

    unsigned chunk_count_;          //how many chunks in one pool
    unsigned chunk_size_;           //the size of chunk
    unsigned chunk_total_;          //total chunks in all of the pool
    unsigned chunk_availble_;       //how many chunks available for use
    unsigned chunk_alloc_times_;    //alloc times, for profile
    unsigned chunk_free_times_;     //free times, for profile
    unsigned chunk_max_alloc_;      //the max number of chunks used in history
};

class ZCE_API zce_allocator : public zce_object
{

private:
    bool lock_;                     //need cross thread usage
    
    zce_mutex pool_lock_;           //protected alloc and free if lock is enabled
    
    unsigned long work_threadid_;   //debug purpose only

    zce_alloc_stat alloc_stat_;

    std::vector<char*> pool_vec_;       //pools

    struct zce_list_node free_list_;    //available chunks

    void add_new_pool();

    void* do_malloc(size_t& nbytes);

    void do_free(void* item);

public:

    inline unsigned chunk_size() const { return alloc_stat_.chunk_size_; }

    inline zce_alloc_stat stat() const { return alloc_stat_; }

public:

    zce_allocator(size_t chunk_size, size_t n_chunks, bool lock);

    virtual ~zce_allocator();

    void *alloc(size_t nbytes, size_t* nreal);

    void zfree(void* item); //fuck off the macro
};

#endif // __zce_allocator_h__
