#pragma once
// ***************************************************************
//  Object   version:  1.0     date: 2002/8/22
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

namespace zce {

class Allocator;
class TaskDelegator;

class ZCE_API Object {
  protected:
    Object()
        : zce_alloc_(nullptr),
          release_delegator_(nullptr),
          ref_count_(0),
          obj_idx_(Tss::get_global()->next_oid()) {};

    Object(const Object& rhs)
        : zce_alloc_(nullptr),
          release_delegator_(rhs.release_delegator_),
          ref_count_(1),
          obj_idx_(Tss::get_global()->next_oid()) {};

    Object& operator=(const Object& rhs) {
        release_delegator_ = rhs.release_delegator_;
        return *this;
    };

    virtual ~Object() {};

    void __free_me() noexcept;

    void __delegate_release() noexcept;

  public:
    inline zce_int64 __get_oid() const noexcept { return obj_idx_; }

    inline bool __isvalid() noexcept { return true; }

    inline void __set_allocator( Allocator* alloc) noexcept { zce_alloc_ = alloc; }

    inline void __set_release_delegator(TaskDelegator* v) { release_delegator_ = v; }

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

    std::shared_ptr<Object> shared_ptr() noexcept {
        __addref();
        auto deleter = [](Object* p) {
            if (p) {
                p->__decref();
            }
        };
        return std::shared_ptr<Object>(this, deleter);
    }

    template <typename T>
    std::shared_ptr<T> shared_from_this() noexcept {
        static_assert(std::is_base_of_v<Object, T>, "T must derive from Object");
        return std::dynamic_pointer_cast<T>(shared_ptr());
    }

  private:
    Allocator* zce_alloc_;
    TaskDelegator* release_delegator_;
    AtomicLong ref_count_;
    const zce_int64 obj_idx_;
};

template <typename ZCE_LOCK>
class SmartPtrLock {
    static ZCE_LOCK mutex_;

  public:
    ZCE_LOCK& get() { return mutex_; }
};
template <typename ZCE_LOCK>
ZCE_LOCK SmartPtrLock<ZCE_LOCK>::mutex_;

template <typename IMPL_CLASS, typename ZCE_LOCK = MutexNull>
class SmartPtr {
    template <typename IMPL_OTHER, typename ZCE_LOCK_OTHER>
    friend class SmartPtr;

  public:
    typedef IMPL_CLASS THE_CLASS;

  public:
    explicit SmartPtr(IMPL_CLASS* handler = 0) {
        if (handler) {
            handler->__addref();
        }
        this->handler_ = handler;
    };

    SmartPtr(const SmartPtr& rhs) { this->handler_ = rhs.__lock_addref(); };

    SmartPtr(SmartPtr&& rhs) {
        this->handler_ = rhs.handler_;
        rhs.handler_ = 0;
    };

    template <typename OTHER_LOCK>
    SmartPtr(const SmartPtr<IMPL_CLASS, OTHER_LOCK>& rhs) {
        this->handler_ = rhs.__lock_addref();
    };

    SmartPtr& operator=(const SmartPtr& rhs) {
        if (this->handler_ != rhs.handler_) {
            IMPL_CLASS* r = rhs.__lock_addref();
            __lock_sign(r);
        }
        return (*this);
    }

    SmartPtr& operator=(SmartPtr&& rhs) {
        if (this->handler_ != rhs.handler_) {
            IMPL_CLASS* r = rhs.handler_;
            rhs.handler_ = 0;
            __lock_sign(r);
        }
        return (*this);
    }

    template <typename OTHER_LOCK>
    SmartPtr& operator=(const SmartPtr<IMPL_CLASS, OTHER_LOCK>& rhs) {
        if (this->handler_ != rhs.handler_) {
            IMPL_CLASS* r = rhs.__lock_addref();
            __lock_sign(r);
        }
        return (*this);
    }

    SmartPtr& operator=(IMPL_CLASS* rhs) {
        if (this->handler_ != rhs) {
            if (rhs) {
                rhs->__addref();
            }

            __lock_sign(rhs);
        }
        return (*this);
    }

    ~SmartPtr() {
        if (this->handler_) this->handler_->__decref();
    }

    inline void __assign(IMPL_CLASS* right) { this->handler_ = right; }

    template <class Y, class P>
    inline bool operator==(const SmartPtr<Y, P>& right) const {
        return (this->handler_ == right.handler_);
    }
    template <class Y, class P>
    inline bool operator!=(const SmartPtr<Y, P>& right) const {
        return (this->handler_ != right.handler_);
    }
    inline bool operator==(IMPL_CLASS* right) const { return (this->handler_ == right); }
    inline bool operator!=(IMPL_CLASS* right) const { return (this->handler_ != right); }
    inline IMPL_CLASS& operator*() const { return (*(this->handler_)); }
    inline IMPL_CLASS* operator->() const { return this->handler_; }
    inline operator IMPL_CLASS*() const { return this->handler_; }
    template <class Y, class P>
    inline bool operator<(const SmartPtr<Y, P>& right) const {
        return (this->handler_ < right.handler_);
    }

    template <class Y, class P>
    static SmartPtr __dynamic_cast(const SmartPtr<Y, P>& rhs) {
        Y* handler = rhs.__lock_addref();
        IMPL_CLASS* p = dynamic_cast<IMPL_CLASS* const>(handler);
        if (p == 0 && handler != 0) {
            handler->__decref();
        }
        SmartPtr<IMPL_CLASS, ZCE_LOCK> lhs;
        lhs.__assign(p);
        return lhs;
    }

    template <class Y>
    static SmartPtr __dynamic_cast(Y* p) {
        return SmartPtr(dynamic_cast<IMPL_CLASS*>(p));
    }

    IMPL_CLASS* get() const { return this->handler_; }

  private:
    IMPL_CLASS* handler_;

    IMPL_CLASS* __lock_addref() const {
        Guard<ZCE_LOCK> g(SmartPtrLock<ZCE_LOCK>().get());
        if (this->handler_) this->handler_->__addref();
        return this->handler_;
    }

    void __lock_sign(IMPL_CLASS* rhs) {
        IMPL_CLASS* ptr = NULL;
        {
            Guard<ZCE_LOCK> g(SmartPtrLock<ZCE_LOCK>().get());
            ptr = this->handler_;
            this->handler_ = rhs;
        }
        if (ptr) {
            ptr->__decref();
        }
    }
};

typedef SmartPtr<Object> ObjectPtr;

template <typename T>
class ObjectWrapper : public Object {
  public:
    typedef ObjectWrapper<T> wrapper_t;
    typedef SmartPtr<wrapper_t> smart_ptr;

    ObjectWrapper() {};

    ObjectWrapper(const T& val) : val_(val) {};

    ObjectWrapper(T&& val) : val_(val) {};

    const T& get() const { return val_; }

    //////////////////////////////////////////////////////////////////////////
    /// must be very careful if T is a complex type, because no thread-safe guarantee

    T& get_reference() { return val_; }

    void set(const T& val) { val_ = val; }

  private:
    T val_;
};

class SmartPtrDecRef {
    Object* const p_;

  public:
    SmartPtrDecRef(Object* p) : p_(p) {}
    ~SmartPtrDecRef() {
        if (p_) p_->__decref();
    }
};

}  // namespace zce
