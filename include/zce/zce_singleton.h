#pragma once
// ***************************************************************
//  zce_singleton   version:  1.0   -  date: 2002/03/01
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

template <typename T, typename L = zce_mutex>
class ZCE_API zce_singleton {
  public:
    static inline T* instance() {
        if (instance_ == 0) {
            zce_guard<L> g(lock_);
            if (instance_ == 0) {
                instance_ = new T;
                if constexpr (std::is_base_of<zce_object, T>::value) {
                    instance_->__addref();
                }
            }
        }
        return instance_;
    }

    static inline void release() {
        zce_guard<L> g(lock_);
        if (instance_ != 0) {
            if constexpr (std::is_base_of<zce_object, T>::value) {
                instance_->__decref();
            } else {
                delete instance_;
            }
            instance_ = 0;
        }
    }

  protected:
    zce_singleton() {}
    virtual ~zce_singleton() {
        zce_guard<L> g(lock_);
        if (instance_ != 0) {
            delete instance_;
            instance_ = 0;
        }
    }

  private:
    zce_singleton(const zce_singleton&) {}
    zce_singleton& operator=(const zce_singleton&) {}

  private:
    static T* instance_;
    static L lock_;
};

template <typename T, typename L>
T* zce_singleton<T, L>::instance_ = NULL;

template <typename T, typename L>
L zce_singleton<T, L>::lock_;
