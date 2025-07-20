#pragma once
/* ***************************************************************
//  zce_data_block   version:  1.0  date: 2009/5/6
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of ZCE, which inherited from ubeda/utiny.
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// |---prespace---|-----length-----|---space---|
// |-------------------capacity----------------|
// base         rd_pos           wr_pos      size
// **************************************************************/
#include <zce/zce_types.h>
#include <zce/zce_object.h>
#include <zce/zce_dblock.h>

class zce_object_counter;

namespace zce {

class Allocator;

class ZCE_API DataBlock : public zce_object {
    zce_byte* buffer_;
    zce_object_counter* obj_;  // for alloc position statistics
    Allocator* alloc_;
    struct _size_t {
        zce_uint64 refbuf : 1;
        zce_uint64 len : 63;
    } size_;

    friend class RefBlock;

    friend class BlockPool;

  public:
    DataBlock(size_t len, zce_object_counter* obj = NULL, Allocator* alloc = NULL);

    DataBlock(size_t len, zce_byte* buf);

    ~DataBlock();

    inline size_t size() const noexcept { return size_.len; }

    bool manbuf() const { return !size_.refbuf; }
};

class ZCE_API RefBlock {
    zce_smartptr<DataBlock> dt_ptr_;
    size_t rd_pos_;
    size_t wr_pos_;

  public:
    RefBlock() : rd_pos_(0), wr_pos_(0) {}

    RefBlock(RefBlock&& rhs) noexcept
        : dt_ptr_(std::move(rhs.dt_ptr_)), rd_pos_(rhs.rd_pos_), wr_pos_(rhs.wr_pos_) {
        rhs.dt_ptr_ = 0;
        rhs.rd_pos_ = 0;
    };

    RefBlock(const RefBlock& rhs) noexcept
        : dt_ptr_(rhs.dt_ptr_), rd_pos_(rhs.rd_pos_), wr_pos_(rhs.wr_pos_) {};

    RefBlock& operator=(const RefBlock& rhs) {
        dt_ptr_ = rhs.dt_ptr_;
        rd_pos_ = rhs.rd_pos_;
        wr_pos_ = rhs.wr_pos_;
        return *this;
    };

    RefBlock& operator=(RefBlock&& rhs) noexcept {
        dt_ptr_ = std::move(rhs.dt_ptr_);
        rd_pos_ = rhs.rd_pos_;
        wr_pos_ = rhs.wr_pos_;
        rhs.dt_ptr_ = 0;
        rhs.rd_pos_ = 0;
        rhs.wr_pos_ = 0;
        return *this;
    };

    inline bool operator==(const RefBlock& rhs) const noexcept {
        return dt_ptr_ == rhs.dt_ptr_ && rd_pos_ == rhs.rd_pos_ && wr_pos_ == rhs.wr_pos_;
    }

    inline bool operator!=(const RefBlock& rhs) const noexcept { return !operator==(rhs); }

    inline DataBlock* _cow_dtblock() const noexcept;

    RefBlock(DataBlock* dtblock);

    RefBlock(size_t size, zce_byte* buf, int wr_pos,
               int rd_pos = 0);  // assign buffer, buffer managed outside

    virtual ~RefBlock();

  public:
    const zce_smartptr<DataBlock>& dt_ptr() const { return dt_ptr_; };

    const zce_byte* base_ptr() const;

    const zce_byte* rd_ptr() const;

    const zce_byte* wr_ptr() const;

    zce_byte* base_ptr_cow();

    zce_byte* rd_ptr_cow();

    zce_byte* wr_ptr_cow();

    void swap(RefBlock& rhs);

    void merge(RefBlock& rhs);

    void preserv(size_t len);

    void rd_ptr(int len);

    void wr_ptr(int len);

    bool is_fullown() const;

    void reset(size_t preserved);

    int copy(const zce_byte* data, size_t len);

    void crunch(size_t preserved = 0);

    inline bool empty() const noexcept { return length() == 0; };

    size_t prespace() const;

    size_t length() const;

    size_t space() const;

    size_t capacity() const;

    void resize(size_t newsize);  // won't crunch in case of protocol stack reserve
};

typedef RefBlock RefBlockPtr;

int ZCE_API fromFile(RefBlock& mb, const char* path, unsigned extra);

//////////////////////////////////////////////////////////////////////////

template <typename T, unsigned S>
class ZCE_API TempBlock {
  public:
    TempBlock(size_t len) {
        if (len * sizeof(T) > sizeof(_tmp)) {
            _size = len * sizeof(T);
            _buf = malloc(_size);
        } else {
            _size = S;
            _buf = _tmp;
        }
    }

    ~TempBlock() {
        if ((NULL != _buf) && (_buf != _tmp)) free(_buf);
    }

    void resize(size_t len) {
        if (len < _size) return;
        if ((NULL != _buf) && (_buf != _tmp)) free(_buf);
        _size = len;
        _buf = malloc(_size);
    }

    T* get() const { return (T*)_buf; }

    size_t size() const { return _size; }

  private:
    void* _buf;
    char _tmp[S];
    size_t _size;
};

}  // namespace zce
