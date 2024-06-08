#pragma execution_character_set("utf-8")
// ***************************************************************
//  rv_inc   version:  1.0   -  date: 8/4/2010
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __rv_inc_h__
#define __rv_inc_h__

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
#include <vector>
#include <map>
#include <deque>
#include <set>
#include <zce/zce_object.h>

#ifndef _WIN32
#   if (FD_SETSIZE < 8192)
#       warning FD_SETSIZE maybe too small
#   endif
#endif

const int EnumRvStormUasKickUser = 0x10001; //param uid

namespace zdp_ktv
{
    enum {
        ERV_SERVICE_STORM = 9,
        ERV_SERVICE_RVDBS,
        ERV_SERVICE_RVUAS,
        ERV_SERVICE_RVMSS,
        ERV_SERVICE_RVMDS,
        ERV_SERVICE_RVKSS,
        ERV_SERVICE_RVCOIN,
        ERV_SERVICE_VXZCE_STUB,
        ERV_SERVICE_VXZCE_ROUTER,
        ERV_SERVICE_VXCMS,
        ERV_SERVICE_VIEW,
        ERV_SERVICE_RVKTV = 20,
        ERV_SERVICE_RVDATASRV = 23,
        ERV_SERVICE_MONITOR = 40,
    };

    enum ERV_MACHINEID {
        ERV_MACHINEID_PARTNER,    //PARTNER
        ERV_MACHINEID_IPADDR,     //IP
        ERV_MACHINEID_MAC,        //MAC地址
        ERV_MACHINEID_DISKDRIVE , //硬盘
        ERV_MACHINEID_MAINBOARD,  //主板
        ERV_MACHINEID_OS,         //操作系统
        ERV_MACHINEID_SID,        //Administrator账户的SID
        ERV_MACHINEID_NAME,       //计算机名
        ERV_MACHINEID_AUTHACCOUNT,
        ERV_MACHINEID_AUTHTOKEN,
        ERV_MACHINEID_AUTHENCPWD,
        ERV_MACHINEID_AUTHPASSWD,
        ERV_MACHINEID_INTRONUM,   //子渠道，安装包名
        ERV_MACHINEID_LIMIT,
    };
};

static const char VXKEY_PUBLIC[] = "00040000b867ca1ae99ab8fe57a4176c6ba59889d1f8b80dc8174ae33e614bc7e8e8e56fe3608d4e06a1d6f5854456d334e1e5f7fead06b7e801fad9087e545617d66e1df7c0ee1c5422fd19d695cd2e4801421bdc8dc911dcbfc99d157ae2ee0a3d371371aee6a8b94d08c7a1f0110ed9f6843771689b5b15cc5575d1317a44fba26eb10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003";
static const char VXKEY_PRIVATE[] = "00040000b867ca1ae99ab8fe57a4176c6ba59889d1f8b80dc8174ae33e614bc7e8e8e56fe3608d4e06a1d6f5854456d334e1e5f7fead06b7e801fad9087e545617d66e1df7c0ee1c5422fd19d695cd2e4801421bdc8dc911dcbfc99d157ae2ee0a3d371371aee6a8b94d08c7a1f0110ed9f6843771689b5b15cc5575d1317a44fba26eb100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000037aefdc11f111d0a98fc2ba4847c3bb068bfb255e8564dc977eeb87da9b45ee4a9795b38959c139f90382e48ccdebeea5547359cff00151e605a98d8eba8ef412d8c050e5b63cca93904c19813d76cd89246ec563b42116700bccf08916628936beb7f8d7ba76dd5a4136c6e1d6a37e04830df7eb2a69477203dbc0001c63ae4bdd79a46f77f4ac9d3cd978f8e192ca11c3172203fb16d804373ce2b445777ce7d7b0d9c67ace461b0cd8f9b662e69fa2d47b5f3a1be085bc6d59bde66c5b1155d526d0544ad3209f414a2df38a3c43bc62d07ef853774ff0cc8a976c2331ec597bea179ea6cc76a53344ed05b51aa78dd85848403a4de48e5e0e1c5e64b1d7ed93a66d9fa54dc868d33ba5fb410c86b6820f6c02a7649002cf7dec782e4fa89a8fcb3bd9a7342ebcb33b512441ef151738523f7c1295ae7d9e3bd3eef2e760e38e19e03831e215bf80dc1ea25c282d2841e054a58cfa354b33070f9d6ccbf2e6529c0fbf19dda46e222df35923671a5e903adad57c33edb43eb412e998768ff355e8817257560c0c354e8d9a6f30121ef7d86652f71d5440c59516dcf67f04d18aec75496cde6b5d9b9411bea6c46683166537e8ff9cb2c52fe3cf131660e812";

//SERVICENAME前两个字符为SERVICE_TYPE，不能重复！
static const char SITE_TOPC[] = "TOPC";
static const char SERVICENAME_CAD[] = "GCAD"; //CARD
static const char SERVICENAME_COI[] = "COIN";
static const char SERVICENAME_GAM[] = "GAME";
static const char SERVICENAME_GAT[] = "GATE";
static const char SERVICENAME_RED[] = "GRED"; //REDPACK, FUDAI
static const char SERVICENAME_P2P[] = "P2SP";
static const char SERVICENAME_DBS[] = "RDBS";
static const char SERVICENAME_VDS[] = "VDBS";
static const char SERVICENAME_VDM[] = "VDMS"; //vxdbs master
static const char SERVICENAME_UAS[] = "XUAS";
static const char SERVICENAME_ROM[] = "XROM"; 
static const char SERVICENAME_IMS[] = "XIMS";
static const char SERVICENAME_LOG[] = "ZLOG";

static bool _ispiuprop(int propid) {
    return false;
    //return propid >= 1000000 && propid < 2000000;
};

static bool _isfreepiuprop(int propid) {
    return false;
    //return propid == 1000001 || propid == 1000006;
};

static bool _is_ppiid(unsigned v){
    return false;// v >= 10000000 && v < 100000000;
}

static bool _is_pptempiid(unsigned v){
    return false;// v >= 400000000;
}

#endif //__rv_inc_h__
