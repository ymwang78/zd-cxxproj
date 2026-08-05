#pragma once
// ***************************************************************
//  Filesystem   version:  1.0   -  date: 2003/01/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#include <zce/zce_config.h>
#include <zce/zce_types.h>
#include <vector>
#include <sys/stat.h>
#include <stdarg.h>
#include <sstream>

#if defined(_WIN32)
#    include <fcntl.h>
#    define ZCE_OPEN_RDFLAG (0)
#    define ZCE_OPEN_WRFLAG (O_WRONLY)
#    define ZCE_OPEN_RDWRFLAG (O_RDWR)
#    define ZCE_OPEN_CRWFLAG (O_RDWR | O_CREAT)
#elif defined(__APPLE__)
#    define ZCE_OPEN_RDFLAG (O_RDONLY)
#    define ZCE_OPEN_WRFLAG (O_WRONLY)
#    define ZCE_OPEN_RDWRFLAG (O_RDWR)
#    define ZCE_OPEN_CRWFLAG (O_RDWR | O_CREAT)
typedef void* LPSECURITY_ATTRIBUTES;
#else
#    include <fcntl.h>
#    if defined(__linux__)
#        include <linux/fs.h>
#    endif
#    if !defined(O_NOATIME)
#        define O_NOATIME 0
#    endif
#    if !defined(O_LARGEFILE)
#        define O_LARGEFILE 0
#    endif
#    define ZCE_OPEN_RDFLAG \
        (O_NOATIME | O_RDONLY | O_LARGEFILE) /* only for mount, need not O_DIRECT */
#    define ZCE_OPEN_WRFLAG \
        (O_WRONLY | O_NOATIME | O_LARGEFILE) /* only for format, need not O_DIRECT */
#    define ZCE_OPEN_RDWRFLAG (O_RDWR | O_NOATIME | O_LARGEFILE)
#    define ZCE_OPEN_CRWFLAG (O_RDWR | O_CREAT | O_NOATIME | O_LARGEFILE)
typedef void* LPSECURITY_ATTRIBUTES;
#endif

handle_t ZCE_API zce_open(const char* filename, int mode, zce_uint16 perms,
                          LPSECURITY_ATTRIBUTES sa);

ssize_t ZCE_API zce_read(handle_t handle, void* buf, size_t len);

zce_int64 ZCE_API zce_llseek(handle_t handle, zce_int64 offset, int whence);

int ZCE_API zce_ftruncate(handle_t handle, zce_int64 newsize);

ssize_t ZCE_API zce_write(handle_t handle, const void* buf, size_t nbyte);

ssize_t ZCE_API zce_writev(handle_t handle, const iovec* iov, int iovcnt);

int ZCE_API zce_close(handle_t handle);

namespace zce {

int ZCE_API statFile(const char* filename_utf8, struct ::stat* stat_buf);

std::string ZCE_API getModulePath();

std::string ZCE_API matchModulePath(const char* filename);

void ZCE_API chdirToModulePath();

std::string ZCE_API getAbsolutePath(const char* filename);

int ZCE_API addToPath(const char* path);

// Set an environment variable of the current process. Subprocesses started
// afterwards through zce::SubProcessHost inherit it.
// name and value are UTF-8. An empty value sets the variable to an empty
// string instead of removing it.
int ZCE_API setEnvVar(const char* name, const char* value);

bool ZCE_API makeDir(const char* dir);

bool ZCE_API isFilePathExists(const char* path);

const char* ZCE_API getFileName(const char* str);

zce_int64 ZCE_API getFileSize(const char* path);

bool ZCE_API writeFile(const char* u8path, const void* buf, size_t len);

class ZCE_API File : public zce::Object {
    handle_t handle_;

    zce_byte block_size_;

    unsigned reserved_blocks_;

  public:
    File(unsigned block_size, unsigned reserved_blocks);

    ~File();

    bool valid() const { return handle_ != ZCE_INVALID_HANDLE; }

    handle_t open(const char* filename, int mode);

    ssize_t read(void* buf, size_t len);

    ssize_t write(const void* buf, size_t nbyte);

    ssize_t writev(const iovec* iov, int iovcnt);

    int lseek_block(zce_uint64 block_id);

    int lseek(zce_uint64 pos, int whence = SEEK_SET);

    int ftruncate(zce_int64 newsize);

    zce_int64 size();

    void close();
};

}  // namespace zce
