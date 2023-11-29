#pragma once

#include <zce/zce_inc.h>
#include <zce/zce_types.h>

class zce_object;
typedef struct _JavaVM JavaVM;
typedef struct _JNIEnv JNIEnv;
typedef struct AAssetManager AAssetManager;
class zce_semaphore;

class ZCE_API zce_tss
{
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
        zce_semaphore* sem_; //for wait delegate task
        JNIEnv* env_;
        void* spec_;
        zce_int64 oid_;
        int last_err_;

        global_t();

        inline zce_int64 next_oid() { return ++oid_; }
    };
    static global_t* get_global();
};
