#pragma once
// ***************************************************************
//  zce::Singleton   version:  1.0   -  date: 2002/03/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

#if defined(_WIN32)
#    include <windows.h>
#endif
#include <zce/zce_sync.h>

namespace zce {

template <typename T, typename L = zce::Mutex>
class ZCE_API Singleton {
  public:
    static inline void setInstance(T* v) { instance_ = v; }

    static inline T* instance() {
        if (instance_ == 0) {
            zce::Guard<L> g(lock_);
            if (instance_ == 0) {
                instance_ = new T;
                if constexpr (std::is_base_of<zce::Object, T>::value) {
                    instance_->__addref();
                }
            }
        }
        return instance_;
    }

    static inline void release() {
        zce::Guard<L> g(lock_);
        if (instance_ != 0) {
            if constexpr (std::is_base_of<zce::Object, T>::value) {
                instance_->__decref();
            } else {
                delete instance_;
            }
            instance_ = 0;
        }
    }

  protected:
    Singleton() {}

    virtual ~Singleton() {
        zce::Guard<L> g(lock_);
        if (instance_ != 0) {
            delete instance_;
            instance_ = 0;
        }
    }

  private:
    Singleton(const Singleton&) {}
    Singleton& operator=(const Singleton&) {}

  private:
    static T* instance_;
    static L lock_;
};

template <typename T, typename L>
T* Singleton<T, L>::instance_ = NULL;

template <typename T, typename L>
L Singleton<T, L>::lock_;

}  // namespace zce
