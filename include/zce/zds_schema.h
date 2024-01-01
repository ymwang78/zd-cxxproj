#pragma once
// ***************************************************************
//  zds_schema   version:  1.0   -  date: 2023/07/07
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include <zce/zce_string.h>
#ifndef CHECKLEN_MOVEBUF_ADDRET_DECSIZE
    #define CHECKLEN_MOVEBUF_ADDRET_DECSIZE do{\
            if (len < 0) \
                return len;\
            if (buf) buf += len;\
            ret += len;\
            size -= len;\
    }while(0)
#endif

class zdl_member;
typedef struct _object PyObject;

namespace zdp
{
    struct zds_context_t
    {
        zce_byte version : 7;
    };

    template<typename T>
    struct is_builtin_basic : std::integral_constant <bool, 
        std::is_arithmetic<T>::value 
        || std::is_same<T, std::string>::value 
        || std::is_same<T, zce_dblock>::value
    > {};

    template<typename T>
    struct is_builtin_vector : std::false_type {};

    template<typename T>
    struct is_builtin_vector<std::vector<T>> : std::integral_constant <bool,
        is_builtin_basic<T>::value > {};

    //template<typename T>
    //struct is_builtin_or_vector_or_string : std::integral_constant<bool,
    //    is_builtin_basic<T>::value ||
    //    is_builtin_string<T>::value ||
    //    is_string<T>::value> {};

    template<typename T>
    constexpr bool is_builtin_type() {
        using decayed_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
        return is_builtin_basic<T>::value 
            || is_builtin_vector<T>::value 
            || (std::is_pointer<decayed_type>::value && is_builtin_basic<typename std::remove_pointer<decayed_type>::type>::value)
            || (std::is_array<decayed_type>::value && is_builtin_basic<typename std::remove_extent<decayed_type>::type>::value);
    }

    int ZCE_API write_varuint_raw(zce_byte* buf, int size, zce_uint64 val, zds_context_t* ctx);

    int ZCE_API read_varuint_raw(zce_uint64& val, const zce_byte* buf, int size, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, bool val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, zce_int64 val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, zce_uint64 val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, zce_float val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, zce_double val, zds_context_t* ctx);

    template<typename T>
    inline int zds_pack_builtin(zce_byte* buf, zce_int32 size, T val, zds_context_t* ctx) {
        static_assert (std::is_integral<T>::value, "must be integral type");
        if (std::is_signed<T>::value) {
            zce_int64 tmp = (zce_int64)val;
            return zds_pack_builtin(buf, size, tmp, ctx);
        }
        else {
            zce_uint64 tmp = (zce_uint64)val;
            return zds_pack_builtin(buf, size, tmp, ctx);
        }
    }

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, const zce_any& val, zds_context_t* ctx);

#define DECLARE_PACK_BUILTIN_ARRAY(TT) \
    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, const std::vector<TT>& val, zds_context_t* ctx);\
    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, const TT* vec, unsigned vec_size, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, const std::vector<bool>& val, zds_context_t* ctx);

    DECLARE_PACK_BUILTIN_ARRAY(zce_byte)
    DECLARE_PACK_BUILTIN_ARRAY(zce_uint16)
    DECLARE_PACK_BUILTIN_ARRAY(zce_uint32)
    DECLARE_PACK_BUILTIN_ARRAY(zce_uint64)
    DECLARE_PACK_BUILTIN_ARRAY(zce_int8)
    DECLARE_PACK_BUILTIN_ARRAY(zce_int16)
    DECLARE_PACK_BUILTIN_ARRAY(zce_int32)
    DECLARE_PACK_BUILTIN_ARRAY(zce_int64)
    DECLARE_PACK_BUILTIN_ARRAY(zce_float)
    DECLARE_PACK_BUILTIN_ARRAY(zce_double)

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, const char* val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, const std::string& val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, const zce::string_view& val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, const std::vector<std::string>& val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, const std::vector<zce_any>& val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, const std::map<zce_any, zce_any>& val, zds_context_t* ctx);

    int ZCE_API zds_pack_struct_header(zce_byte* buf, int size, zce_uint64 struct_prefix, zds_context_t* ctx, bool has_prefix);

    int ZCE_API zds_pack_struct_array_header(zce_byte* buf, int size, zce_uint64 array_size, zds_context_t* ctx);

    int ZCE_API zds_pack_any_array_header(zce_byte* buf, int size, zce_uint64 array_size, zds_context_t* ctx);

    ///////////////////////////////////////////////////////////////////////////

    int ZCE_API zds_unpack_builtin(zce_int64& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

    int ZCE_API zds_unpack_builtin(zce_float& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

    int ZCE_API zds_unpack_builtin(zce_double& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

    template<typename T>
    inline int zds_unpack_builtin(T& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx) {
        zce_int64 tmp = 0;
        int len = zds_unpack_builtin(tmp, buf, size, ctx);
        if (len > 0) val = (T)tmp;
        return len;
    }

	int ZCE_API zds_unpack_builtin(std::vector<bool>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

	int ZCE_API zds_unpack_builtin(std::vector<zce_byte>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

	int ZCE_API zds_unpack_builtin(std::vector<zce_uint16>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

	int ZCE_API zds_unpack_builtin(std::vector<zce_uint32>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

	int ZCE_API zds_unpack_builtin(std::vector<zce_uint64>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

	int ZCE_API zds_unpack_builtin(std::vector<zce_int8>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

	int ZCE_API zds_unpack_builtin(std::vector<zce_int16>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

	int ZCE_API zds_unpack_builtin(std::vector<zce_int32>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

	int ZCE_API zds_unpack_builtin(std::vector<zce_int64>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

	int ZCE_API zds_unpack_builtin(std::vector<zce_float>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

	int ZCE_API zds_unpack_builtin(std::vector<zce_double>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

    int ZCE_API zds_unpack_builtin(std::string& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

    int ZCE_API zds_unpack_builtin(zce_any& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

    int ZCE_API zds_unpack_builtin(std::vector<std::string>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

    int ZCE_API zds_unpack_struct_header(zce_uint64& struct_prefix, const zce_byte* buf, int size, zds_context_t* ctx, bool has_prefix);

    int ZCE_API zds_unpack_struct_array_header(zce_uint64& val, const zce_byte* buf, int size, zds_context_t* ctx);

    template<typename T>
    int zds_pack_array(zce_byte* buf, zce_int32 size, const std::vector<T>& val, zds_context_t* ctx) {

        int len = 0, ret = 0;

        len = zds_pack_struct_array_header(buf, size, val.size(), ctx);
        CHECKLEN_MOVEBUF_ADDRET_DECSIZE;

        for (auto iter = val.begin(); iter != val.end(); ++iter) {
            len = zds_pack(buf, size, *iter, ctx, false);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
        };

        return ret;
    }

    template<typename T>
    int zds_unpack_array(std::vector<T>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx) {
        int len = 0, ret = 0;
        zce_uint64 alen = 0;
        len = zds_unpack_struct_array_header(alen, buf, size, ctx);
        CHECKLEN_MOVEBUF_ADDRET_DECSIZE;

        //fast size check, to avoid a very large array size
        //at least every struct has 1 byte
        if (size < alen) return ZCE_ERROR_SHRTLEN;

        val.resize((size_t)alen);

        typename std::vector<T>::iterator iter;
        for (iter = val.begin(); iter != val.end(); ++iter) {
            len = zds_unpack(*iter, buf, size, ctx, false);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
        };

        return ret;
    }

    //skip one object
    int ZCE_API zds_unpack_skip(const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

    template <typename T, typename... Args>
    int zds_pack_multi(zce_byte* buf, zce_int32 size, zds_context_t* ctx, bool has_prefix, const T& val, Args&&... args) {
        int len = 0, ret = 0;
        if constexpr (is_builtin_type<T>()) {
            len = zds_pack_builtin(buf, size, val, ctx);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
        }
        else {
            len = zds_pack(buf, size, val, ctx, has_prefix);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
        }
        if constexpr (sizeof...(args) > 0) {
            len = zds_pack_multi(buf, size, ctx, has_prefix, std::forward<Args>(args)...);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
        }

        return ret;
    }
} //namespace zdp
