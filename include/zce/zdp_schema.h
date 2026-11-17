#pragma once
// ***************************************************************
//  zdp_schema   version:  1.0   -  date: 2015/11/14
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// The ZDP frame header: the 10 bytes (magic, attributes, message id, length, sequence) in
// front of every message that zdp_stream and Storm carry. Message bodies are ZDS
// (zds_schema.h); the old ZDP body encoding was removed.
// ***************************************************************
#include <zce/zce_config.h>
#include <zce/zce_types.h>
#include <zce/zce_bytes.h>
#include <zce/zce_object.h>
#include <zce/zce_dblock.h>
#include <zce/zce_mbpool.h>
#include <limits>
#include <new>
#include <stdexcept>
#include <typeinfo>
#include <utility>

namespace zce {

namespace zdp {

static const int ZDP_HEADLEN = 10;

static inline int zdp_headlen(int rev) {
    (void)rev;
    return ZDP_HEADLEN;
}

struct ZCE_API zdp_head {
    zce_byte magic /*'Z':ZDP, 'S':ZDS, 'B': BSON, 'J': JSON, 'P': PROTOBUF*/;
    zce_byte attrib;
    zce_uint16 msgmid;
    zce_uint32 msglen;
    zce_uint32 msgseq;

    zdp_head() : magic(0), attrib(0), msgmid(0), msglen(0), msgseq(0) {};

    inline zce_byte rev() const { return (attrib >> 4) & 0x3; };
    inline zce_byte zip() const { return attrib & 0x3; }
    inline zce_uint32 len() const { return ZDP_HEADLEN; }
    inline void set_rev(zce_byte v) { attrib = (attrib & 0xF) | (v << 4); }
    inline void set_zip(zce_byte v) { attrib = (attrib & 0xFC) | (v & 0x3); }
};

static inline unsigned zdp_msglen(const zce_byte* buf)  // should ensure >= 10
{
    return ((buf[4] & 0xf) << 24) | (buf[5] << 16) | (buf[6] << 8) | (buf[7]);
};

static inline unsigned short zdp_msgtype(const zce_byte* buf)  // should ensure >= 10
{
    return (buf[2] << 8) | (buf[3]);
};

int ZCE_API zdp_unpack(zdp_head&, const zce_byte* buf, zce_int32 size, zce_byte rev);

int ZCE_API zdp_pack(const zdp_head&, zce_byte* buf, zce_int32 size, zce_byte rev);

int ZCE_API zdp_reset_seq(const zdp_head&, zce_byte* buf, zce_int32 size, zce_byte rev);

int ZCE_API zdp_reset_zip(zce_byte v, zce_byte* buf, zce_int32 size, zce_byte rev);

#ifndef CHECKLEN_MOVEBUF_ADDRET_DECSIZE

#    define CHECKLEN_MOVEBUF_ADDRET_DECSIZE \
        do {                                \
            if (len < 0) return len;        \
            if (buf) buf += len;            \
            ret += len;                     \
            size -= len;                    \
        } while (0)

#    define CHECKCHAR_MOVEBUF_ADDDATA_DECSIZE(LTR)           \
        do {                                                 \
            if (size == 0) return ZCE_ERROR_MALLOC;          \
            if (buf && *buf != LTR) return ZCE_ERROR_SYNTAX; \
            ret = 1;                                         \
            if (buf) buf += ret;                             \
            len += ret;                                      \
            size -= ret;                                     \
        } while (0)

#    define CHECKCHAR_IGNORESPACE_DECSIZE                             \
        do {                                                          \
            while ((size > 0) && ((' ' == *buf) || ('\t' == *buf))) { \
                buf += 1;                                             \
                ret += 1;                                             \
                size -= 1;                                            \
            }                                                         \
        } while (0)

#    define CHECKCHAR_MOVEBUF_LINEFEED                      \
        do {                                                \
            if (size > 0) {                                 \
                if (size < 2) return 0;                     \
                if (('\r' != buf[0]) || ('\n' != buf[1])) { \
                    return ZCE_ERROR_SYNTAX;                \
                }                                           \
                buf += 2;                                   \
                ret += 2;                                   \
                size -= 2;                                  \
            }                                               \
        } while (0)

#    define CHECKCHAR_MOVEBUF_SPACE_DECSIZE                           \
        do {                                                          \
            int ret1 = 0;                                             \
            while ((size > 0) && ((' ' == *buf) || ('\t' == *buf))) { \
                ++ret1;                                               \
                buf += 1;                                             \
                ret += 1;                                             \
                size -= 1;                                            \
            }                                                         \
            if (ret1 < 1) return ZCE_ERROR_SYNTAX;                    \
        } while (0)

#endif

enum ERV_ZDP_ZIP {
    ZDP_ZIP_NONE,
    ZDP_ZIP_BZIP2,
    ZDP_ZIP_NOPE,
};
int ZCE_API zdp_unzip(zce::RefBlock& out_ptr, const zce::RefBlock& in_ptr, const zdp_head& head);

};  // namespace zdp

}  // namespace zce
