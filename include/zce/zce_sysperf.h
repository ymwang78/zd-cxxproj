#pragma once
#include <zce/zce_config.h>
#include <zce/zce_types.h>
#include <vector>
#include <stdarg.h>
#include <sstream>

#ifdef __cplusplus
extern "C"
{
#endif

    int ZCE_API zce_net_traffic_persec(int &nbytesend, int&nbyterecv);

#ifdef __cplusplus
}
#endif
