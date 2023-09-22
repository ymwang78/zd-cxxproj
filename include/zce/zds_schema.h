#pragma once

namespace zdp
{
    int ZCE_API write_varuint_raw(zce_byte* buf, int size, zce_uint64 val);

	int ZCE_API zds_pack(zce_byte* buf, zce_int32 size, zce_int64 val);

	int ZCE_API zds_unpack(zce_int64* val, zce_byte* buf, zce_int32 size);

	int ZCE_API zds_pack(zce_byte* buf, zce_int32 size, const std::string& val);

	int ZCE_API zds_unpack(std::string& val, const zce_byte* buf, zce_int32 size);

    int ZCE_API pack_builtin_array(zce_byte* buf, zce_int32 size, const std::vector<bool>& val, zce_int32 fixed_len);

    int ZCE_API pack_builtin_array(zce_byte* buf, zce_int32 size, const std::vector<zce_byte>& val, zce_int32 fixed_len);

    int ZCE_API pack_builtin_array(zce_byte* buf, zce_int32 size, const std::vector<zce_uint16>& val, zce_int32 fixed_len);

    int ZCE_API pack_builtin_array(zce_byte* buf, zce_int32 size, const std::vector<zce_uint32>& val, zce_int32 fixed_len);

    int ZCE_API pack_builtin_array(zce_byte* buf, zce_int32 size, const std::vector<zce_uint64>& val, zce_int32 fixed_len);

    int ZCE_API pack_builtin_array(zce_byte* buf, zce_int32 size, const std::vector<zce_int8>& val, zce_int32 fixed_len);

    int ZCE_API pack_builtin_array(zce_byte* buf, zce_int32 size, const std::vector<zce_int16>& val, zce_int32 fixed_len);

    int ZCE_API pack_builtin_array(zce_byte* buf, zce_int32 size, const std::vector<zce_int32>& val, zce_int32 fixed_len);

    int ZCE_API pack_builtin_array(zce_byte* buf, zce_int32 size, const std::vector<zce_int64>& val, zce_int32 fixed_len);

    int ZCE_API pack_builtin_array(zce_byte* buf, zce_int32 size, const std::vector<zce_float>& val, zce_int32 fixed_len);

    int ZCE_API pack_builtin_array(zce_byte* buf, zce_int32 size, const std::vector<zce_double>& val, zce_int32 fixed_len);

    template<typename T>
    int pack_array(zce_byte* buf, zce_int32 size, const std::vector<T>& val, int fixed_len) {
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
    int unpack_array(std::vector<T>& val, const zce_byte* buf, zce_int32 size, int fixed_len) {
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
}