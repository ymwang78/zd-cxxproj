#pragma once
// ***************************************************************
//  zce_object   version:  1.0     date: 2002/8/22
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2003 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

#include <zce/zce_inc.h>
#include <zce/zce_types.h>
#include <zce/zce_atomic.h>
#include <zce/zce_tss.h>
#include <zce/zce_sync.h>

class zce_allocator;
class zce_task_delegator;

class ZCE_API zce_object {
  protected:
    zce_object()
        : zce_alloc_(nullptr),
          release_delegator_(nullptr),
          ref_count_(0),
          obj_idx_(zce_tss::get_global()->next_oid()) {};

    zce_object(const zce_object& rhs)
        : zce_alloc_(nullptr),
          release_delegator_(rhs.release_delegator_),
          ref_count_(1),
          obj_idx_(zce_tss::get_global()->next_oid()) {};

    zce_object& operator=(const zce_object& rhs) {
        release_delegator_ = rhs.release_delegator_;
        return *this;
    };

    virtual ~zce_object() {};

    void __free_me() noexcept;

    void __delegate_release() noexcept;

  public:
    inline zce_int64 __get_oid() const noexcept { return obj_idx_; }

    inline bool __isvalid() noexcept { return true; }

    inline void __set_allocator(zce_allocator* alloc) noexcept { zce_alloc_ = alloc; }

    inline void __set_release_delegator(zce_task_delegator* v) { release_delegator_ = v; }

    inline void __addref() noexcept { ++this->ref_count_; }

    inline void __decref() noexcept {
        if (--this->ref_count_ != 0) return;
        if (release_delegator_) {
            __delegate_release();
        } else if (zce_alloc_) {
            __free_me();
        } else {
            delete this;
        }
    }

    inline long __get_ref_count() const noexcept { return this->ref_count_.value(); }

    std::shared_ptr<zce_object> shared_ptr() noexcept {
        __addref();
        auto deleter = [](zce_object* p) {
            if (p) {
                p->__decref();
            }
        };
        return std::shared_ptr<zce_object>(this, deleter);
    }

    template <typename T>
    std::shared_ptr<T> shared_from_this() noexcept {
        static_assert(std::is_base_of_v<zce_object, T>, "T must derive from zce_object");
        return std::dynamic_pointer_cast<T>(shared_ptr());
    }

  private:
    zce_allocator* zce_alloc_;
    zce_task_delegator* release_delegator_;
    zce_atomic_long ref_count_;
    const zce_int64 obj_idx_;
};

template <typename ZCE_LOCK>
class zce_smartptr_lock {
    static ZCE_LOCK mutex_;

  public:
    ZCE_LOCK& get() { return mutex_; }
};
template <typename ZCE_LOCK>
ZCE_LOCK zce_smartptr_lock<ZCE_LOCK>::mutex_;

template <typename IMPL_CLASS, typename ZCE_LOCK = zce_mutex_null>
class zce_smartptr {
    template <typename IMPL_OTHER, typename ZCE_LOCK_OTHER>
    friend class zce_smartptr;

  public:
    typedef IMPL_CLASS THE_CLASS;

  public:
    explicit zce_smartptr(IMPL_CLASS* handler = 0) {
        if (handler) {
            handler->__addref();
        }
        this->handler_ = handler;
    };

    zce_smartptr(const zce_smartptr& rhs) { this->handler_ = rhs.__lock_addref(); };

    zce_smartptr(zce_smartptr&& rhs) {
        this->handler_ = rhs.handler_;
        rhs.handler_ = 0;
    };

    template <typename OTHER_LOCK>
    zce_smartptr(const zce_smartptr<IMPL_CLASS, OTHER_LOCK>& rhs) {
        this->handler_ = rhs.__lock_addref();
    };

    zce_smartptr& operator=(const zce_smartptr& rhs) {
        if (this->handler_ != rhs.handler_) {
            IMPL_CLASS* r = rhs.__lock_addref();
            __lock_sign(r);
        }
        return (*this);
    }

    zce_smartptr& operator=(zce_smartptr&& rhs) {
        if (this->handler_ != rhs.handler_) {
            IMPL_CLASS* r = rhs.handler_;
            rhs.handler_ = 0;
            __lock_sign(r);
        }
        return (*this);
    }

    template <typename OTHER_LOCK>
    zce_smartptr& operator=(const zce_smartptr<IMPL_CLASS, OTHER_LOCK>& rhs) {
        if (this->handler_ != rhs.handler_) {
            IMPL_CLASS* r = rhs.__lock_addref();
            __lock_sign(r);
        }
        return (*this);
    }

    zce_smartptr& operator=(IMPL_CLASS* rhs) {
        if (this->handler_ != rhs) {
            if (rhs) {
                rhs->__addref();
            }

            __lock_sign(rhs);
        }
        return (*this);
    }

    ~zce_smartptr() {
        if (this->handler_) this->handler_->__decref();
    }

    inline void __assign(IMPL_CLASS* right) { this->handler_ = right; }

    template <class Y, class P>
    inline bool operator==(const zce_smartptr<Y, P>& right) const {
        return (this->handler_ == right.handler_);
    }
    template <class Y, class P>
    inline bool operator!=(const zce_smartptr<Y, P>& right) const {
        return (this->handler_ != right.handler_);
    }
    inline bool operator==(IMPL_CLASS* right) const { return (this->handler_ == right); }
    inline bool operator!=(IMPL_CLASS* right) const { return (this->handler_ != right); }
    inline IMPL_CLASS& operator*() const { return (*(this->handler_)); }
    inline IMPL_CLASS* operator->() const { return this->handler_; }
    inline operator IMPL_CLASS*() const { return this->handler_; }
    template <class Y, class P>
    inline bool operator<(const zce_smartptr<Y, P>& right) const {
        return (this->handler_ < right.handler_);
    }

    template <class Y, class P>
    static zce_smartptr __dynamic_cast(const zce_smartptr<Y, P>& rhs) {
        Y* handler = rhs.__lock_addref();
        IMPL_CLASS* p = dynamic_cast<IMPL_CLASS* const>(handler);
        if (p == 0 && handler != 0) {
            handler->__decref();
        }
        zce_smartptr<IMPL_CLASS, ZCE_LOCK> lhs;
        lhs.__assign(p);
        return lhs;
    }

    template <class Y>
    static zce_smartptr __dynamic_cast(Y* p) {
        return zce_smartptr(dynamic_cast<IMPL_CLASS*>(p));
    }

  private:
    IMPL_CLASS* handler_;

    IMPL_CLASS* __lock_addref() const {
        zce_guard<ZCE_LOCK> g(zce_smartptr_lock<ZCE_LOCK>().get());
        if (this->handler_) this->handler_->__addref();
        return this->handler_;
    }

    void __lock_sign(IMPL_CLASS* rhs) {
        IMPL_CLASS* ptr = NULL;
        {
            zce_guard<ZCE_LOCK> g(zce_smartptr_lock<ZCE_LOCK>().get());
            ptr = this->handler_;
            this->handler_ = rhs;
        }
        if (ptr) {
            ptr->__decref();
        }
    }
};

typedef zce_smartptr<zce_object> zce_object_ptr;

template <typename T>
class zce_object_wrapper : public zce_object {
  public:
    typedef zce_object_wrapper<T> wrapper_t;
    typedef zce_smartptr<wrapper_t> smart_ptr;

    zce_object_wrapper() {};

    zce_object_wrapper(const T& val) : val_(val) {};

    zce_object_wrapper(T&& val) : val_(val) {};

    const T& get() const { return val_; }

    //////////////////////////////////////////////////////////////////////////
    /// must be very careful if T is a complex type, because no thread-safe guarantee

    T& get_reference() { return val_; }

    void set(const T& val) { val_ = val; }

  private:
    T val_;
};

class zce_smartptr_decref {
    zce_object* const p_;

  public:
    zce_smartptr_decref(zce_object* p) : p_(p) {}
    ~zce_smartptr_decref() {
        if (p_) p_->__decref();
    }
};
