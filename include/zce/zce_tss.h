#pragma once
// ***************************************************************
//  zce_tss  version:  1.0   -  date: 2003/02/15
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include <zce/zce_inc.h>
#include <zce/zce_types.h>

class zce_object;
typedef struct _JavaVM JavaVM;
typedef struct _JNIEnv JNIEnv;
typedef struct AAssetManager AAssetManager;
class zce_semaphore;
class zce_task_delegator;

class ZCE_API zce_tss {
    struct pimpl;
    struct pimpl* pimpl_;

public:

    zce_tss();

    ~zce_tss();

    zce_object* ts_object(zce_object* ptr);

    zce_object* ts_object();

public:
    struct global_t {
        static JavaVM* jvm_;
        static AAssetManager* aasset_manager_;
        JNIEnv* env_;
        void* spec_;
        zce_int64 oid_;
        int last_errcode_;
        char* last_errdesc_;
        zce_task_delegator* current_delegator_;  // 当前线程的任务委托者环境
        std::vector<zce_semaphore*> sem_vec_;  // for wait delegate task
        static constexpr int last_errdesc_size_ = 4096;

        global_t();

        ~global_t();

        inline zce_int64 next_oid() { return ++oid_; }

        zce_semaphore* get_semaphore();

        void return_semaphore(zce_semaphore* sem);
    };

    static global_t* get_global(bool create_if_not_exists = true);

    struct zce_global_semaphore {
        zce_global_semaphore(const zce_global_semaphore&) = delete;
        zce_global_semaphore& operator=(const zce_global_semaphore&) = delete;

        zce_tss::global_t* tss;
        zce_semaphore* sem;
        zce_global_semaphore();
        ~zce_global_semaphore();
    };

    struct zce_env_task_delegator {
        zce_tss::global_t* tss;
        zce_env_task_delegator(zce_task_delegator* v);
        ~zce_env_task_delegator();
    };
};
