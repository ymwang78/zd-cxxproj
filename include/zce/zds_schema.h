#pragma once

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
        zce_byte noarray_ifsmall : 1;
        zce_byte version : 7;
    };

    int ZCE_API write_varuint_raw(zce_byte* buf, int size, zce_uint64 val, zds_context_t* ctx);

    int ZCE_API read_varuint_raw(zce_uint64& val, const zce_byte* buf, int size, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, zce_int64 val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, zce_uint64 val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, zce_float val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, zce_double val, zds_context_t* ctx);

    template<typename T>
    inline int zds_pack_builtin(zce_byte* buf, zce_int32 size, T val, zds_context_t* ctx) {
        if (std::is_signed<T>::value) {
            zce_int64 tmp = (zce_int64)val;
            return zds_pack_builtin(buf, size, tmp, ctx);
        }
        else {
            zce_uint64 tmp = (zce_uint64)val;
            return zds_pack_builtin(buf, size, tmp, ctx);
        }
    }

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

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, const std::string& val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, const zce::string_view& val, zds_context_t* ctx);

    int ZCE_API zds_pack_builtin(zce_byte* buf, zce_int32 size, const std::vector<std::string>& val, zds_context_t* ctx);

    int ZCE_API zds_pack_struct_header(zce_byte* buf, int size, zce_uint64 struct_prefix, zds_context_t* ctx);

    int ZCE_API zds_pack_struct_array_header(zce_byte* buf, int size, zce_uint64 array_size, zds_context_t* ctx);

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

    int ZCE_API zds_unpack_builtin(std::vector<std::string>& val, const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

    int ZCE_API zds_unpack_struct_header(zce_uint64& struct_prefix, const zce_byte* buf, int size, zds_context_t* ctx);

    int ZCE_API zds_unpack_struct_array_header(zce_uint64& val, const zce_byte* buf, int size, zds_context_t* ctx);

    template<typename T>
    int zds_pack_array(zce_byte* buf, zce_int32 size, const std::vector<T>& val, zds_context_t* ctx) {

        int len = 0, ret = 0;

        len = zds_pack_struct_array_header(buf, size, val.size(), ctx);
        CHECKLEN_MOVEBUF_ADDRET_DECSIZE;

        for (auto iter = val.begin(); iter != val.end(); ++iter) {
            len = zds_pack(buf, size, *iter, false);
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

        val.resize(alen);
        typename std::vector<T>::iterator iter;
        for (iter = val.begin(); iter != val.end(); ++iter) {
            len = zds_unpack(*iter, buf, size, false);
            CHECKLEN_MOVEBUF_ADDRET_DECSIZE;
        };

        return ret;
    }

    int ZCE_API zds_unpack_skip(const zce_byte* buf, zce_int32 size, zds_context_t* ctx);

} //namespace zdp

class zdl_struct;
class zdl_member;
class zdl_parser_context;
class zvm_py;

class zpy_unmarshal_context
{
    zce_smartptr<zvm_py> zvm_;

    static std::string get_module_name(const std::string& structname);

    PyObject* default_struct_instance(const zce_smartptr<zdl_struct>& struct_ptr);

    PyObject* default_obj_from_member(const zce_smartptr<zdl_member>& member_ptr);
public:

    zpy_unmarshal_context(const zce_smartptr<zvm_py>& pyvm);

    ~zpy_unmarshal_context();

    int unmarshal_builtin(PyObject*& p, zce_byte pt, const zce_byte* buf, int size);

    int unmarshal_string(PyObject*& str, const zce_byte* buf, int size);

    int unmarshal_struct(PyObject*& obj, const zce_smartptr<zdl_struct>& struct_ptr, const zce_byte* buf, int size);

    int unmarshal_builtin_array(PyObject*& obj, int pt, const zce_byte* buf, int size);

    int unmarshal_struct_array(PyObject*& obj, const zce_smartptr<zdl_struct>& struct_ptr, const zce_byte* buf, int size);

    int unmarshal_member(PyObject*& obj, const zce_smartptr<zdl_member>& member, const zce_byte* buf, int size);

    int unmarshal_vector_from_struct(std::vector<PyObject*>& obj, const zce_smartptr<zdl_struct>& struct_ptr, const zce_byte* buf, int size);
};

class zpy_marshal_context
{
    zce_smartptr<zvm_py> zvm_;

    static std::string get_module_name(const std::string& structname);
    
public:
    zpy_marshal_context(const zce_smartptr<zvm_py>& pyvm);

    ~zpy_marshal_context();

    int marshal_string(zce_byte* buf, int size, PyObject* str, const char* varname);

    int marshal_struct(zce_byte* buf, int size, PyObject* obj, const zdl_struct* struct_ptr);

    int marshal_builtin_array(zce_byte* buf, int size, PyObject* obj, int ut, const char* varname);

    int marshal_struct_array(zce_byte* buf, int size, PyObject* obj, const zdl_struct* struct_ptr, const char* varname);

    int marshal_member(zce_byte* buf, int size, PyObject* obj, const zce_smartptr<zdl_member>& member);
};