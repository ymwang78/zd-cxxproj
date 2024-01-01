// ***************************************************************
//  rpcdemo_inc   version:  1.0   -  date: 6/22/2015
//  -------------------------------------------------------------
//  Yongming Wang(wym@zhidu.biz)
//  Zhidu Network Co. Ltd.
//  -------------------------------------------------------------
//  Copyright (C) 2015 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __rpcdemo_inc_h__
#define __rpcdemo_inc_h__

#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/resource.h>
typedef long long LONGLONG;
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <set>
#include <zce/zce_config.h>
#include <zce/zce_dblock.h>
#include <zce/zce_thread.h>
#include <zce/zce_reactor.h>
#include "rpcdemo_config_proto.h"

#endif // __rpcdemo_inc_h__
