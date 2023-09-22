#pragma once

namespace zdp
{
	int ZCE_API zds_pack(zce_byte* buf, zce_int32 size, zce_int64 val);

	int ZCE_API zds_unpack(zce_int64* val, zce_byte* buf, zce_int32 size);

}