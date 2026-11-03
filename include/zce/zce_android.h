#pragma once

#include <zce/zce_config.h>

#ifdef __cplusplus
extern "C" {
#endif

int ZCE_API zce_android_init(JavaVM* jvm, jobject jAssetManager);

ZCE_API JNIEnv* zce_android_getenv();

#ifdef __cplusplus
};
#endif
