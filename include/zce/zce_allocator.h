#pragma once
// ***************************************************************
//  zce::Allocator   version:  1.0   -  date: 2002/04/29
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of ZCE, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// pool: |---chunk---|---chunk---|...|---chunk---|
// pool: |---chunk---|---chunk---|...|---chunk---|
// ***************************************************************

#include <zce/zce_object.h>
#include <zce/zce_sync.h>
#include <zce/zce_list.h>
#include <vector>

namespace zce {

struct ZCE_API AllocStat {
    AllocStat(size_t chunk_s) noexcept;

    AllocStat() = default;

    inline unsigned currentAlloc() const noexcept {
        return chunk_alloc_times_ - chunk_free_times_;
    }

    inline void setMaxAlloc() noexcept {
        unsigned cur_alloc = currentAlloc();
        if (cur_alloc > chunk_max_used_) chunk_max_used_ = cur_alloc;
    }

    unsigned chunk_data_size_;    // the size of chunk
    unsigned chunk_availble_;     // how many chunks available for use
    unsigned chunk_alloc_times_;  // alloc times, for profile
    unsigned chunk_free_times_;   // free times, for profile
    unsigned chunk_max_used_;     // the max number of chunks used in history
};

class ZCE_API Allocator : public zce_object {
  public:
    enum ALLOCATOR_TYPE { CHUNK, V2 };

    static zce_smartptr<Allocator> createChunk(size_t chunk_size, size_t n_chunks, bool lock);

    static zce_smartptr<Allocator> createDynamic(size_t chunk_size, size_t n_chunks, bool lock);

    virtual size_t getTotalSize() const noexcept = 0;

    virtual void getStat(std::vector<zce::AllocStat>& stat) const noexcept = 0;

    virtual size_t getCapacity(void* item) const noexcept = 0;

    virtual void* alloc(size_t nbytes, size_t* nreal) = 0;

    virtual void zfree(void* item) noexcept = 0;  // fuck off the macro

    virtual void* realloc(void* ptr, size_t nbytes, size_t* nreal) noexcept = 0;

    virtual ALLOCATOR_TYPE getAllocatorType() const noexcept = 0;
};

}  // namespace zce
