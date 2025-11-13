#pragma once

#ifdef _WIN32
#    ifdef LIBHOST_EXPORTS
#        define LIBHOST_API __declspec(dllexport)
#    else
#        define LIBHOST_API __declspec(dllimport)
#    endif
#else
#    define LIBHOST_API
#endif //_WIN32

struct hostvm_start_t {
    int my_size;
    const char* process_guid;
    const char* log_suffix;
    const char *vm_name;
    const char *vm_path;
    const char *vm_listen_addr;
    unsigned short vm_listen_port;
    int extra_argc;
    const char **extra_argv;
};

#ifdef _WIN32

typedef struct hostvm_entry_t *hostvm_entry_ptr_t;
typedef hostvm_entry_ptr_t(WINAPI *pfn_hostvm_entry_init)(void);
typedef int(WINAPI *pfn_hostvm_entry_start)(hostvm_entry_ptr_t, const hostvm_start_t* args);
typedef void(WINAPI *pfn_hostvm_entry_objprint)(void);

#else

extern "C" {
typedef struct hostvm_entry_t *hostvm_entry_ptr_t;
hostvm_entry_ptr_t LIBHOST_API hostvm_entry_init();
int LIBHOST_API hostvm_entry_start(hostvm_entry_ptr_t entry_ptr, const hostvm_start_t* args);
void LIBHOST_API hostvm_entry_objprint();
}

#endif