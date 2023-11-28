/* ***************************************************************
//  zce_data_block   version:  1.0  date: 23/23/2009
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// |---prespace---|-----length-----|---space---|
// |-------------------capacity----------------|
// base         rd_pos           wr_pos      size
// **************************************************************/
#pragma once
#ifndef __zce_data_block_h__
#define __zce_data_block_h__

#include <zce/zce_types.h>
#include <zce/zce_smartptr.h>
#include <zce/zce_dblock.h>

class zce_object_counter;
class zce_allocator;

class ZCE_API zce_dtblock : public zce_smartptr_mtbase
{
    zce_byte* buffer_;
    size_t size_;
    zce_object_counter* obj_;       //for alloc position statistics
    zce_allocator* alloc_;
    bool ownbuf_;           //will take response of free buffer

    friend class zce_dblock;

    friend class zce_mbpool;

public:
    zce_dtblock(size_t len, zce_object_counter* obj = NULL, zce_allocator* alloc = NULL);

    zce_dtblock(size_t len, zce_byte* buf);
    
    ~zce_dtblock();

    bool manbuf() const {
        return ownbuf_;
    }
};

class ZCE_API zce_dblock
{
    zce_smartptr<zce_dtblock> dt_ptr_;
    size_t rd_pos_;
    size_t wr_pos_;

public:

    zce_dblock()
        :rd_pos_(0), wr_pos_(0) {
    }

    zce_dblock(zce_dblock&& rhs) noexcept
        :dt_ptr_(std::move(rhs.dt_ptr_)), rd_pos_(rhs.rd_pos_), wr_pos_(rhs.wr_pos_) {
        rhs.dt_ptr_ = 0;
        rhs.rd_pos_ = 0;
    };

    zce_dblock(const zce_dblock& rhs) noexcept
        :dt_ptr_(rhs.dt_ptr_), rd_pos_(rhs.rd_pos_), wr_pos_(rhs.wr_pos_){
    };

    zce_dblock& operator = (const zce_dblock& rhs) {
        dt_ptr_ = rhs.dt_ptr_;
        rd_pos_ = rhs.rd_pos_;
        wr_pos_ = rhs.wr_pos_;
        return *this;
    };

    zce_dblock(zce_dtblock* dtblock);

    zce_dblock(size_t size, zce_byte* buf, int wr_pos, int rd_pos = 0 ); //assign buffer, buffer managed outside

    virtual ~zce_dblock();

public:

    zce_byte* base_ptr() const;

    zce_byte* rd_ptr() const;

    zce_byte* wr_ptr() const;

    void swap(zce_dblock& rhs);

    void preserv(size_t len);

    void rd_ptr(int len);

    void wr_ptr(int len);

    bool is_fullown() const;

    void reset(size_t preserved);

    int copy(const zce_byte* data, size_t len);

    void crunch(size_t preserved = 0);

    size_t prespace() const;

    size_t length() const;

    size_t space() const;

    size_t capacity() const;

    void resize(size_t newsize); //won't crunch in case of protocol stack reserve
};

typedef zce_dblock zce_data_block_ptr;

int ZCE_API zce_dblock_from_file(zce_dblock& mb, const char* path);

//////////////////////////////////////////////////////////////////////////

template<typename T, unsigned S>
class ZCE_API zce_tblock
{
public:
    zce_tblock(size_t len) {
        if (len*sizeof(T) > sizeof(_tmp)) {
            _size = len*sizeof(T);
            _buf = malloc(_size);
        }
        else {
            _size = S;
            _buf = _tmp;
        }
    }

    ~zce_tblock() {
        if ((NULL != _buf) && (_buf != _tmp))
            free(_buf);
    }

    void resize(size_t len) {
        if (len < _size)
            return;
        if ((NULL != _buf) && (_buf != _tmp))
            free(_buf);
        _size = len;
        _buf = malloc(_size);
    }

    T* get()const { return (T*)_buf; }

    size_t size() const { return _size;  }

private:
    void*    _buf;
    char    _tmp[S];
    size_t  _size;
};

#endif // __zce_data_block_h__
