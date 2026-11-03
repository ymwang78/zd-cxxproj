#pragma once
/* ***************************************************************
 *  keelhost_module  -  the C ABI between HostVM (KeelHost) and a module plugin
 * ***************************************************************
 * A module (KeelSim, fgw, ...) is a shared library in <host dir>/modules/<Name>/ next to its
 * module.xml (ymwang78/HostVM#43). The host reads module.xml to know which vmtypes the module
 * provides without loading it; a worker started for one of those vmtypes loads the library,
 * calls khost_module_entry() and checks the descriptor against module.xml and against the libzce
 * of the process before it calls init().
 *
 * The descriptor is plain C and constant: the host reads it without running any module code.
 * init() does what the module's xxx_init() always did (registering its virtual machine types),
 * and usually is that function:
 *
 *   #include <keelhost/keelhost_module.h>
 *   extern "C" int ksim_init();
 *   static const char* const kVmTypes[] = {"KeelSim", nullptr};
 *   static int moduleInit(const khost_host_info*, const void*) { return ksim_init(); }
 *   static const khost_module_desc kDesc = {
 *       KHOST_MODULE_ABI, sizeof(kDesc), "KeelSim", KSIM_VERSION_STR, ZCE_ABI_VERSION,
 *       ZCE_BUILD_ID, kVmTypes, nullptr, nullptr, moduleInit, nullptr};
 *   KHOST_MODULE_DEFINE(kDesc)
 *
 * The struct layouts only ever grow at the end: a host checks `size` before it reads a field
 * added later, and KHOST_MODULE_ABI changes when a layout cannot stay compatible.
 * *************************************************************** */
#include <stdint.h>

#define KHOST_MODULE_ABI 1

/* The name of the one symbol every module exports. */
#define KHOST_MODULE_ENTRY_NAME "khost_module_entry"

#if defined(_WIN32)
#    define KHOST_MODULE_EXPORT __declspec(dllexport)
#else
#    define KHOST_MODULE_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* What the host tells init() about where it runs. */
typedef struct khost_host_info {
    uint32_t size;          /* sizeof(khost_host_info) */
    uint32_t host_version;  /* HOSTVM_VERSION: compatibility.year.month.sequence, a byte each */
    const char* host_dir;   /* the directory of the host's executable, UTF-8 */
    const char* module_dir; /* this module's directory: its private libraries and resources */
    const char* vmtype;     /* the vmtype this worker starts */
} khost_host_info;

typedef struct khost_module_desc {
    uint32_t abi;            /* KHOST_MODULE_ABI */
    uint32_t size;           /* sizeof(khost_module_desc) */
    const char* name;        /* "KeelSim": the module's name, as in module.xml */
    const char* version;     /* "26.45.3" */
    uint32_t zce_abi;        /* ZCE_ABI_VERSION the module was compiled with */
    const char* build_id;    /* ZCE_BUILD_ID the module was compiled with */
    const char* const* vmtypes;   /* {"KeelSim", NULL}: the vmtypes it provides; may be empty */
    const char* const* requires_; /* {"py", NULL}: modules loaded and initialised before it.
                                     Named with an underscore: requires is a C++20 keyword. */
    const char* const* extends;   /* {"lua", NULL}: vmtypes whose workers also load this module,
                                     which then provides no vmtype of its own (coin for lua) */
    /* Called once in a worker, after the modules it requires. app_options points to the
       worker's zce::AppOptions. Returns < 0 on failure, and the worker stops. */
    int (*init)(const khost_host_info* host, const void* app_options);
    void (*fini)(void); /* may be NULL; not called when a worker ends with _exit */
} khost_module_desc;

typedef const khost_module_desc* (*khost_module_entry_fn)(void);

#ifdef __cplusplus
}
#    define KHOST_MODULE_DEFINE(desc)                                                     \
        extern "C" KHOST_MODULE_EXPORT const khost_module_desc* khost_module_entry(void) { \
            return &(desc);                                                                \
        }
#else
#    define KHOST_MODULE_DEFINE(desc)                                                     \
        KHOST_MODULE_EXPORT const khost_module_desc* khost_module_entry(void) { return &(desc); }
#endif
