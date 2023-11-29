
// ***************************************************************
//  zdp_schema   version:  1.0   -  date: 11/14/2015
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once
#ifndef __zdp_schema_h__
#define __zdp_schema_h__

#include <zce/zce_config.h>
#include <zce/zce_types.h>
#include <zce/zce_bytes.h>
#include <zce/zce_object.h>
#include <zce/zce_dblock.h>
#include <zce/zce_mbpool.h>
#include <typeinfo>
#include <utility>

namespace zdp
{
    static const int ZDP_HEADLEN = 10;

    static const int   GEP_HEADLEN = 76;
    static const char* GEP_ZERO_DOMAIN = "000000";
    static const char* GEP_ZERO_ID = "000000000000000000";

    static const int   NSSD_HEADLEN = 20;

    static inline int zdp_dynarr_length(size_t x) {
        return (((x) < 255) ? (int)((x)+1) : (int)((x)+5));
    };

    static inline int zdp_dynarr_headlen(size_t x) {
        return (((x) < 255) ? 1 : 5);
    }

    static inline int zdp_headlen(int rev) {
        return rev == 3 ? GEP_HEADLEN : ZDP_HEADLEN;
    }

    struct ZCE_API zdp_head
    {
        zce_byte    magic /*'Z':ZDP, 'B': BSON, 'J': JSON, 'P': PROTOBUF, 'G': GEP, '\8':NSSD*/;
        zce_byte    attrib;
        zce_uint16  msgmid;
        zce_uint32  msglen;
        zce_uint32  msgseq;

#if 1 //def ZDP_GEP
        struct gepex_t {
            zce_astring src_dmn;
            zce_astring src_id;
            zce_astring dst_dmn;
            zce_astring dst_id;

            gepex_t()
                : src_dmn(GEP_ZERO_DOMAIN), src_id(GEP_ZERO_ID),
                dst_dmn(GEP_ZERO_DOMAIN), dst_id(GEP_ZERO_ID) {
            };

        } gepex;

        struct nssdex_t {
            zce_uint32 msgtype;
            zce_uint32 msgfullid;
            zce_uint32 connid;
            nssdex_t() : msgtype(0), msgfullid(0) {
            };
        } nssdex;
#endif 

        zdp_head() : magic(0), attrib(0), msgmid(0), msglen(0), msgseq(0) { };

        inline zce_byte   rev() const { return (attrib >> 4) & 0x3; };
        inline zce_byte   zip() const { return attrib & 0x3; }
        inline zce_uint32 len() const { return (rev() == 3) ? ((magic == 0x08) ? NSSD_HEADLEN : GEP_HEADLEN) : ZDP_HEADLEN; }
		inline void set_rev(zce_byte v) { attrib = (attrib & 0xF) | (v << 4); }
		inline void set_zip(zce_byte v) { attrib = (attrib & 0xFC) | (v & 0x3); }
    };

    static inline unsigned zdp_msglen(const zce_byte* buf) //should ensure >= 10
    {
        if (buf[0] != 'G')
            return (buf[5] << 16) | (buf[6] << 8) | (buf[7]);
        else
            return (buf[69] << 16) | (buf[70] << 8) | (buf[71]);
    };

    static inline unsigned short zdp_msgtype(const zce_byte* buf) //should ensure >= 10
    {
        if (buf[0] != 'G')
            return (buf[2] << 8) | (buf[3]);
        else
            return (buf[7] << 8) | (buf[8]);
    };

    int ZCE_API zdp_unpack(zdp_head&, const zce_byte* buf, zce_int32 size, zce_byte rev);

    int ZCE_API zdp_pack(const zdp_head&, zce_byte* buf, zce_int32 size, zce_byte rev);

    int ZCE_API zdp_reset_seq(const zdp_head&, zce_byte* buf, zce_int32 size, zce_byte rev);

    int ZCE_API zdp_reset_zip(zce_byte v, zce_byte* buf, zce_int32 size, zce_byte rev);

    static inline int zdp_ie_head_len(int bodylen, zce_byte rev) { return 6; }

#define MOVE(x)    std::move(x)

#ifndef CHECKLEN_MOVEBUF_ADDRET_DECSIZE

#define CHECKLEN_MOVEBUF_ADDRET_DECSIZE do{\
            if (len < 0) \
                return len;\
            buf += len;\
            ret += len;\
            size -= len;\
    }while(0)

#define CHECKCHAR_MOVEBUF_ADDDATA_DECSIZE(LTR) \
    do { \
        if (size == 0) \
            return ZCE_ERROR_MALLOC; \
        if (*buf != LTR) \
            return ZCE_ERROR_SYNTAX; \
        ret = 1; \
        buf += ret; \
        len += ret; \
        size -= ret; \
    }while(0)

#define CHECKCHAR_IGNORESPACE_DECSIZE \
    do { \
        while ((size > 0) && ((' ' == *buf) || ('\t' == *buf)))    { \
            buf += 1; \
            ret += 1; \
            size -= 1; \
        } \
    }while(0)

#define CHECKCHAR_MOVEBUF_LINEFEED \
    do { \
        if (size > 0) {\
            if (size < 2) return 0;\
            if (('\r' != buf[0]) || ('\n' != buf[1])) { \
                return ZCE_ERROR_SYNTAX; \
            } \
            buf += 2; \
            ret += 2; \
            size -= 2; \
        }\
    }while(0)

    #define CHECKCHAR_MOVEBUF_SPACE_DECSIZE \
    do { \
        int ret1 = 0; \
        while ((size > 0) && ((' ' == *buf) || ('\t' == *buf)))    { \
            ++ret1; \
            buf += 1; \
            ret += 1; \
            size -= 1; \
        } \
        if (ret1 < 1) \
            return ZCE_ERROR_SYNTAX; \
    }while(0)

#endif

    ZCE_API int set_array_len(zce_byte* buf, zce_int32 size, zce_int32 alen);

    ZCE_API int get_array_len(zce_int32* alen, const zce_byte* buf, zce_int32 size);

    template<typename T>
    static inline int pack_builtin(zce_byte* buf, int size, T val)
    {
        if (size < (int)sizeof(T))
            return ZCE_ERROR_SHRTLEN;

        if (sizeof(T) == 1) {
            ZBYTE_HN_SCVAL(buf, 0, *(zce_char*)&val);
        }
        else if (sizeof(T) == 2) {
            ZBYTE_HN_SSVAL(buf, 0, *(zce_int16*)&val);
        }
        else if (sizeof(T) == 4) {
            ZBYTE_HN_SIVAL(buf, 0, *(zce_int32*)&val);
        }
        else if (sizeof(T) == 8) {
            ZBYTE_HN_SLVAL(buf, 0, *(zce_int64*)&val);
        }
        else {
            return ZCE_ERROR_SYNTAX;
        }
        return sizeof(T);
    }

    template<typename T>
    static inline int unpack_builtin(T *val, const zce_byte* buf, int size)
    {
        if (size < (int)sizeof(T))
            return ZCE_ERROR_SHRTLEN;

        if (sizeof(T) == 1) {
            *val = (T)ZBYTE_HN_CVAL(buf, 0);
        }
        else if (sizeof(T) == 2) {
            *val = (T)ZBYTE_HN_SVAL(buf, 0);
        }
        else if (sizeof(T) == 4) {
            *val = (T)ZBYTE_HN_IVAL(buf, 0);
        }
        else if (sizeof(T) == 8) {
            zce_byte* v = (zce_byte*)val;
            v[0] = buf[7];
            v[1] = buf[6];
            v[2] = buf[5];
            v[3] = buf[4];
            v[4] = buf[3];
            v[5] = buf[2];
            v[6] = buf[1];
            v[7] = buf[0];
        }
        else {
            return ZCE_ERROR_SYNTAX;
        }

        return sizeof(T);
    }

    static inline int pack_builtin_iehead(zce_byte* buf, int size,
        unsigned short ie_val, zce_byte ie_format, zce_int32 ie_len, zce_byte /*version*/)
    {
        if (size < 6 || ie_len >= 1024 * 1024 * 1024)
            return ZCE_ERROR_SHRTLEN;
        buf[0] = (zce_byte)(ie_val >> 8);
        buf[1] = (zce_byte)ie_val;
        buf[2] = ie_format;
        buf[3] = (zce_byte)(ie_len >> 16);
        buf[4] = (zce_byte)(ie_len >> 8);
        buf[5] = (zce_byte)ie_len;
        return 6;
    }

    static inline int unpack_builtin_iehead(
        zce_uint16* ie_val, zce_byte* ie_format, zce_int32* ie_len,
        const zce_byte* buf, int size, zce_byte /*version*/)
    {
        if (size < 6)
            return ZCE_ERROR_SHRTLEN;
        *ie_val = ((zce_uint16)buf[0] << 8) | buf[1];
        *ie_format = buf[2];
        *ie_len = ((zce_uint32)buf[3] << 16) | ((zce_uint32)buf[4] << 8) | buf[5];
        return 6;
    }

    template<typename T>
    static inline int pack_builtin_array(zce_byte* buf, zce_int32 size, const std::vector<T>& val, zce_int32 fixed_len)
    {
        int len = 0, ret = 0, i = 0;
        zce_int32 alen = (zce_int32)val.size();

        if (fixed_len > 0 && fixed_len != alen) {
            ZCE_DEBUG((ZLOG_DEBUG, "pack_builtin_array array=%s, len=%u, fixed_len=%u", typeid(T).name(), alen, fixed_len));
            return ZCE_ERROR_SYNTAX;
        }

        if (fixed_len == 0) {
            len = set_array_len(buf, size, alen);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
        }

        len = (alen * sizeof(T));
        if (size < len)
            return ZCE_ERROR_SHRTLEN;
        if (len == 0)
            return ret;

        if (sizeof(T) == 1) {
            memcpy(buf, &val[0], len);
        }
        else if (sizeof(T) == 2) {
            for (i = 0; i < alen; ++i) {
                ZBYTE_HN_SSVAL(buf, 0, (zce_int16)val[i]);
                buf += 2;
            }
        }
        else if (sizeof(T) == 4) {
            for (i = 0; i < alen; ++i) {
                ZBYTE_HN_SIVAL(buf, 0, (zce_int32)val[i]);
                buf += 4;
            }
        }
        else if (sizeof(T) == 8) {
            for (i = 0; i < alen; ++i) {
                ZBYTE_HN_SLVAL(buf, 0, (zce_int64)val[i]);
                buf += 8;
            }
        }

        ret += len;
        return ret;
    }

    template<typename T>
    static inline int unpack_builtin_array(std::vector<T>& val,
        const zce_byte* buf, zce_int32 size, zce_int32 fixed_len)
    {
        int len = 0, ret = 0, i = 0;
        zce_int32 alen = 0;

        if (fixed_len == 0) {
            len = get_array_len(&alen, buf, size);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
            if (alen != fixed_len && fixed_len != 0) {
                ZCE_DEBUG((ZLOG_DEBUG, 
                    "unpack_builtin_array array=%s, len=%u, fixed_len=%u\n",
                    typeid(T).name(), alen, fixed_len));
                return ZCE_ERROR_SYNTAX;
            }
        }
        else {
            alen = fixed_len;
        }

        if (alen == 0)
            return ret;

        val.resize(alen);

        len = (alen * sizeof(T));
        if (size < len)
            return ZCE_ERROR_SHRTLEN;

        if (sizeof(T) == 1) {
            memcpy(&val[0], buf, len);
        }
        else if (sizeof(T) == 2) {
            for (i = 0; i < alen; ++i) {
                val[i] = (T)ZBYTE_HN_SVAL(buf, 0);
                buf += 2;
            }
        }
        else if (sizeof(T) == 4) {
            for (i = 0; i < alen; ++i) {
                val[i] = (T)ZBYTE_HN_IVAL(buf, 0);
                buf += 4;
            }
        }
        else if (sizeof(T) == 8) {
            for (i = 0; i < alen; ++i) {
                val[i] = (T)ZBYTE_HN_LVAL(buf, 0);
                buf += 8;
            }
        }
        ret += len;
        return ret;
    }

    template<typename T, typename P>
    static inline int unpack_builtin_fix_array(P& val,
        const zce_byte* buf, zce_int32 size, zce_int32 fixed_len,
        bool alwasy_has_array_len)
    {
        int len = 0, ret = 0, i = 0;
        zce_int32 alen = 0;

        if (alwasy_has_array_len || fixed_len == 0) {
            len = get_array_len(&alen, buf, size);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
            if (alen != fixed_len && fixed_len != 0) {
                ZCE_DEBUG((ZLOG_DEBUG, "unpack_builtin_fix_array array=%s, len=%u, fixed_len=%u\n", typeid(T).name(), alen, fixed_len));
                return ZCE_ERROR_SYNTAX;
            }
        }
        else {
            alen = fixed_len;
        }

        if (alen == 0)
            return ret;

        len = (alen * sizeof(T));
        if (size < len)
            return ZCE_ERROR_SHRTLEN;

        if (sizeof(T) == 1) {
            memcpy(&val[0], buf, len);
        }
        else if (sizeof(T) == 2) {
            for (i = 0; i < alen; ++i) {
                val[i] = (T)ZBYTE_HN_SVAL(buf, 0);
                buf += 2;
            }
        }
        else if (sizeof(T) == 4) {
            for (i = 0; i < alen; ++i) {
                val[i] = (T)ZBYTE_HN_IVAL(buf, 0);
                buf += 4;
            }
        }
        else if (sizeof(T) == 8) {
            for (i = 0; i < alen; ++i) {
                val[i] = (T)ZBYTE_HN_LVAL(buf, 0);
                buf += 8;
            }
        }
        ret += len;
        return ret;
    }

    template<typename T>
    static inline int pack_rawptr_pair(zce_byte* buf, zce_int32 size,
        const std::pair<T*, T*>& val, zce_int32 fixed_len)
    {
        int len = 0, ret = 0;
        zce_int32 alen = (zce_int32)(val.second - val.first);

        if (fixed_len > 0 && fixed_len != alen) {
            ZCE_DEBUG((ZLOG_DEBUG, 
                "[proto_pack] array=%s, len=%u, fixed_len=%u\n",
                typeid(T).name(), alen, fixed_len));
            return ZCE_ERROR_SYNTAX;
        }

        if (fixed_len == 0) {
            len = set_array_len(buf, size, alen);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
        }

        len = (alen * sizeof(T));
        if (size < len)
            return ZCE_ERROR_SHRTLEN;
        if (len == 0)
            return ret;

        memcpy(buf, val.first, len);

        ret += len;
        return ret;
    }

    template<typename T>
    static inline int unpack_rawptr_pair(std::pair<T*, T*>& val,
        const zce_byte* buf, zce_int32 size, zce_int32 fixed_len)
    {
        int len = 0, ret = 0;
        zce_int32 alen = 0;

        if (fixed_len == 0) {
            len = get_array_len(&alen, buf, size);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
        }
        else {
            alen = fixed_len;
        }

        if (alen == 0) {
            val.first = val.second = NULL;
            return ret;
        }

        val.first = (T*)buf;
        val.second = val.first + alen;
        len = (alen * sizeof(T));
        if (size < len)
            return ZCE_ERROR_SHRTLEN;
        ret += len;
        return ret;
    }

    int ZCE_API calc_pack_string_len(const std::string& val, zce_byte version);
	int ZCE_API pack_builtin_string(zce_byte* buf, zce_int32 size, const char* data, zce_int32 len, zce_int32 fixed_len, zce_byte version);
    int ZCE_API pack_builtin_string(zce_byte* buf, zce_int32 size, const std::string& val, zce_int32 fixed_len, zce_byte version);
    int ZCE_API unpack_builtin_string(std::string& val, const zce_byte* buf, zce_int32 size, zce_int32 fixed_len, zce_byte version);
    int ZCE_API zdp_pack(const std::string& val, zce_byte* buf, zce_int32 size, zce_byte version);
    int ZCE_API zdp_unpack(std::string& val, const zce_byte* buf, zce_int32 size, zce_byte version);
    int ZCE_API zdp_total_length(const std::string& val, zce_byte version);

#ifdef _WIN32
    int ZCE_API calc_pack_string_len(const std::wstring& val, zce_byte version);
    int ZCE_API pack_builtin_string(zce_byte* buf, zce_int32 size, const std::wstring& val, zce_int32 fixed_len, zce_byte version);
    int ZCE_API unpack_builtin_string(std::wstring& val, const zce_byte* buf, zce_int32 size, zce_int32 fixed_len, zce_byte version);
#endif

    template<typename T>
    int pack_array(zce_byte* buf, zce_int32 size, const std::vector<T>& val, int fixed_len, unsigned char version) {
        int len = 0, ret = 0;

        if (fixed_len > 0 && fixed_len != val.size()) {
            ZCE_DEBUG((ZLOG_DEBUG, "pack_array fixed_len != val.size()\n"));
            return ZCE_ERROR_SYNTAX;
        }

        len = set_array_len(buf, size, (zce_int32)val.size());
        CHECKLEN_MOVEBUF_ADDRET_DECSIZE;

        for (auto iter = val.begin(); iter != val.end(); ++iter) {
            len = zdp_pack(*iter, buf, size, version);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
        };
        return ret;
    }

    template<typename T>
    int unpack_array(std::vector<T>& val, const zce_byte* buf, zce_int32 size, int fixed_len, unsigned char version) {
        int len = 0, ret = 0, alen = 0;
        len = get_array_len(&alen, buf, size);
        CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
        if (fixed_len && (fixed_len != alen)) {
            ZCE_DEBUG((ZLOG_DEBUG, "unpack_array fixed_len != alen\n"));
            return ZCE_ERROR_SYNTAX;
        }
        val.resize(alen);

        typename std::vector<T>::iterator iter;
        for (iter = val.begin(); iter != val.end(); ++iter) {
            len = zdp_unpack(*iter, buf, size, version);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
        };
        return ret;
    }

    template<typename T>
    int pack_ie_array(zce_byte* buf, zce_int32 size, const std::vector<T>& val, int /*fixed_len*/, unsigned char version) {
        int len = 0, ret = 0;
        for (auto iter = val.begin(); iter != val.end(); ++iter) {
            len = zdp_pack(*iter, buf, size, version);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
        };
        return ret;
    }

    template<typename T>
    int unpack_ie_array(std::vector<T>& val, const zce_byte* buf, zce_int32 size, int /*fixed_len*/, unsigned char version) {
        int len = 0, ret = 0;
        do {
            T tempval;
            len = zdp_unpack(tempval, buf, size, version);
            if (len == 0)
                break;
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
            val.push_back(tempval);
        } while (true);
        return ret;
    }

    template<typename T>
    zce_int32 array_total_length(const std::vector<T>& val, unsigned char version) {
        zce_int32 len = 0;
        for (auto iter = val.begin(); iter != val.end(); ++iter)
            len += zdp_total_length(*iter, version);
        len += (val.size() < 255) ? 1 : 5;
        return len;
    }

    template<typename T>
    zce_int32 rawptr_pair_total_length(const std::pair<T*, T*>& val, unsigned char version) {
        if (val.second - val.first >= 255)
            return (val.second - val.first) + 5;
        return (val.second - val.first) + 1;
    }

    template<typename T>
    zce_int32 ie_array_total_length(const std::vector<T>& val, unsigned char version) {
        zce_int32 len = 0;
        typename std::vector<T>::const_iterator iter;
        for (iter = val.begin(); iter != val.end(); ++iter)
            len += zdp_total_length(*iter, version);
        return len;
    }

    enum ERV_ZDP_ZIP {
        ZDP_ZIP_NONE,
        ZDP_ZIP_BZIP2,
        ZDP_ZIP_NOPE,
    };
    int ZCE_API zdp_unzip(zce_dblock& out_ptr, const zce_dblock& in_ptr, const zdp_head& head);
};

#define ZDP_PROC_MSG(Y, X, C) case Y::X::MSG_TYPE : {\
    zce_smartptr<Y::X> msgptr(new Y::X());\
    len = zdp::zdp_unpack(*msgptr, buf, size, head.rev());\
    if (len < 0) {\
        ZCE_ERROR((ZLOG_ERROR, "ASSERT! %s::%s ret:%d", #Y, #X, len)); \
        return len;\
    }\
    proc_##X(head, msgptr, C);\
} break;

#define ZDP_PROC_RES(Y, X) case Y::X::MSG_TYPE : {\
    if (head.msglen() != -1) { \
        zce_smartptr<Y::X> msgptr(new Y::X());\
        len = zdp::zdp_unpack(*msgptr, buf, size, head.rev());\
        if (len < 0) {\
            ZCE_ERROR((ZLOG_ERROR, "ASSERT! %s::%s ret:%d", #Y, #X, len)); \
            return len;\
        }\
        proc_##X(head, msgptr, ctx);\
    }\
    else {\
        proc_##X(head, zce_smartptr<Y::X>(0), ctx);\
    }\
} break;

#endif // __zdp_schema_h__
