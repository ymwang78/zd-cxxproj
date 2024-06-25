#pragma once
// ***************************************************************
//  zce_filesystem   version:  1.0   -  date: 2003/01/01
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
#include <stdarg.h>
#include <sstream>

#if defined(_WIN32)
#   include <fcntl.h>
#   define ZCE_OPEN_RDFLAG      (0)
#   define ZCE_OPEN_WRFLAG      (O_WRONLY)
#   define ZCE_OPEN_RDWRFLAG    (O_RDWR)
#   define ZCE_OPEN_CRWFLAG     (O_RDWR | O_CREAT)
#else
#   include <linux/fs.h>
#   define ZCE_OPEN_RDFLAG      (O_NOATIME | O_RDONLY | O_LARGEFILE)		/* only for mount, need not O_DIRECT */
#   define ZCE_OPEN_WRFLAG      (O_WRONLY | O_NOATIME | O_LARGEFILE)		/* only for format, need not O_DIRECT */
#   define ZCE_OPEN_RDWRFLAG    (O_RDWR | O_NOATIME | O_LARGEFILE)
#   define ZCE_OPEN_CRWFLAG     (O_RDWR | O_CREAT | O_NOATIME | O_LARGEFILE)
typedef void* LPSECURITY_ATTRIBUTES;
#endif

std::string ZCE_API zce_get_path(const char* filename);

std::string ZCE_API zce_abs_path(const char* filename);

int ZCE_API zce_add_path(const char* path);

void ZCE_API zce_chdir();

bool ZCE_API zce_makedir(const char* dir);

const char* ZCE_API zce_filename(const char* str);

bool ZCE_API zce_filepath_exists(const char* path);

int ZCE_API zce_temp_dir(char* buffer, size_t* size);

zce_int64 ZCE_API zce_filesize(const char* path);

handle_t ZCE_API zce_open(const char* filename, int mode, zce_uint16 perms, LPSECURITY_ATTRIBUTES sa);

ssize_t ZCE_API zce_read(handle_t handle, void* buf, size_t len);

zce_int64 ZCE_API zce_llseek(handle_t handle, zce_int64 offset, int whence);

int ZCE_API zce_ftruncate(handle_t handle, zce_int64 newsize);

ssize_t ZCE_API zce_write(handle_t handle, const void* buf, size_t nbyte);

ssize_t ZCE_API zce_writev(handle_t handle, const iovec* iov, int iovcnt);

int ZCE_API zce_close(handle_t handle);


class ZCE_API zce_file : public zce_object
{
    handle_t handle_;

    zce_byte block_size_;

    unsigned reserved_blocks_;

public:

    zce_file(unsigned block_size, unsigned reserved_blocks);

    ~zce_file();

    bool valid() const { return handle_ != ZCE_INVALID_HANDLE; }

    handle_t open(const char* filename, int mode);

    ssize_t read(void* buf, size_t len);

    ssize_t writev(const iovec* iov, int iovcnt);

    int lseek_block(zce_uint64 block_id);

    int lseek(zce_uint64 pos, int whence = SEEK_SET);

    int ftruncate(zce_int64 newsize);

    zce_int64 size();

    void close();
};
