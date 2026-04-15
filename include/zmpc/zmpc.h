#pragma once
#include "zmpc_proto.h"

enum _ZMPC_ERRCODE {
    ZMPC_ERRCODE_OK = 0x03010000,  // 成功

    ZMPC_ERRCODE_BASE = 0x83010000,  // == ZAP_ERROR_MPC,
    ZMPC_ERRCODE_EXCEPTION,
    ZMPC_ERRCODE_MEMORY,         // 内存不足,
    ZMPC_ERRCODE_INDEXERR,
    ZMPC_ERRCODE_BUSY,          // 运行状态不允许的操作, 例如加载配置
    ZMPC_ERRCODE_DATAFULL,      // 现有数据已满
    ZMPC_ERRCODE_INVALIDXV,     // 不是已知的MV/DV/CV/TV等
    ZMPC_ERRCODE_INVALIDCONFIG,
    ZMPC_ERRCODE_PRJINCOMPLETE,
    ZMPC_ERRCODE_DATASOURCE,
    ZMPC_ERRCODE_ERRCONFIG,
    ZMPC_ERRCODE_CONTROLLER,
    ZMPC_ERRCODE_CONNECT,  // 连接错误
    ZMPC_ERRCODE_READTAG,  // 读取标签错误
    ZMPC_ERRCODE_PYMPC,
    ZMPC_ERRCODE_DUPSTATUS,
    ZMPC_ERRCODE_INVALIDSCRIPTNAME,
    ZMPC_ERRCODE_DISABLEDSCRIPT,
    ZMPC_ERRCODE_TESTINGFULL,
    ZMPC_ERRCODE_INVALIDVTYPE,
    ZMPC_ERRCODE_OUTLIMIT,      // 超限
    ZMPC_ERRCODE_USERCANCEL,    // 用户取消操作
    ZMPC_ERRCODE_PROJECTEXIST,  // 项目已存在
    ZMPC_ERRCODE_PROJECTNOTEXIST,
    ZMPC_ERRCODE_PATHFILEWRITE,
    ZMPC_ERRCODE_PATHFILEREAD,
    ZMPC_ERRCODE_INVALIDVALUE,
    ZMPC_ERRCODE_MATRIXSIZE,
};

enum _TAGCTX_INDEX {
    TAGCTX_INDEX_UNKNOWN = 0,
    TAGCTX_INDEX_TAGNAME,
    TAGCTX_INDEX_TRACKTG,
    TAGCTX_INDEX_HILIMIT,
    TAGCTX_INDEX_LOLIMIT,
    TAGCTX_INDEX_IRVALUE,
    TAGCTX_INDEX_ONOFFSW,
    TAGCTX_INDEX_STPOINT,
    TAGCTX_INDEX_CURSTPT,
    TAGCTX_INDEX_ENDSTPT,
    TAGCTX_INDEX_SLOPETG,
    TAGCTX_INDEX_DELTABD,
};

enum ControlStatus { ControlStatusOff, ControlStatusOn, ControlStatusFeedForward };

enum TestingStatus { TestingStatusOff, TestingStatusOn };

// 错误码定义
static const int XV_ERROR_CONNECT = (1 << TAGCTX_INDEX_UNKNOWN);  // 连接错误
static const int XV_ERROR_TAGNAME = (1 << TAGCTX_INDEX_TAGNAME);
static const int XV_ERROR_TRACKTAGNAME = (1 << TAGCTX_INDEX_TRACKTG);
static const int XV_ERROR_HIGHLIMIT = (1 << TAGCTX_INDEX_HILIMIT);
static const int XV_ERROR_LOWLIMIT = (1 << TAGCTX_INDEX_LOLIMIT);
static const int XV_ERROR_IRVTAGNAME = (1 << TAGCTX_INDEX_IRVALUE);
static const int XV_ERROR_ONOFFTAGNAME = (1 << TAGCTX_INDEX_ONOFFSW);
static const int XV_ERROR_SETPTAGNAME = (1 << TAGCTX_INDEX_STPOINT);
static const int XV_ERROR_CURSPTAGNAME = (1 << TAGCTX_INDEX_CURSTPT);
static const int XV_ERROR_ENDSPTAGNAME = (1 << TAGCTX_INDEX_ENDSTPT);
static const int XV_ERROR_SLOPETAGNAME = (1 << TAGCTX_INDEX_SLOPETG);
static const int XV_ERROR_DELTATAGNAME = (1 << TAGCTX_INDEX_DELTABD);

// MV错误定义
static const int MV_ERROR_MVTAGNAME = XV_ERROR_TAGNAME;
static const int MV_ERROR_HIGHTLIMIT = XV_ERROR_HIGHLIMIT;
static const int MV_ERROR_LOWLIMIT = XV_ERROR_LOWLIMIT;
static const int MV_ERROR_IRVTAGNAME = XV_ERROR_IRVTAGNAME;
static const int MV_ERROR_ONOFFTAGNAME = XV_ERROR_ONOFFTAGNAME;
static const int MV_ERROR_TRACKTAGNAME = XV_ERROR_TRACKTAGNAME;
static const int MV_ERROR_SPTAGNAME = XV_ERROR_SETPTAGNAME;

// DV错误定义
static const int DV_ERROR_DVTAGNAME = XV_ERROR_TAGNAME;
static const int DV_ERROR_DVPREDTAGNAME = XV_ERROR_DELTATAGNAME;

// CV错误定义
static const int CV_ERROR_CVTAGNAME = XV_ERROR_TAGNAME;
static const int CV_ERROR_IRVTAGNAME = XV_ERROR_IRVTAGNAME;
static const int CV_ERROR_HIGHTLIMIT = XV_ERROR_HIGHLIMIT;
static const int CV_ERROR_LOWLIMIT = XV_ERROR_LOWLIMIT;
static const int CV_ERROR_ONOFFTAGNAME = XV_ERROR_ONOFFTAGNAME;
static const int CV_ERROR_SPTAGNAME = XV_ERROR_SETPTAGNAME;
static const int CV_ERROR_CURSETPOINTAGNAME = XV_ERROR_CURSPTAGNAME;
static const int CV_ERROR_ENDSETPOINTAGNAME = XV_ERROR_ENDSPTAGNAME;
static const int CV_ERROR_DELTABINDINGTAGNAME = XV_ERROR_DELTATAGNAME;

static const int CV_ERROR_SLOPETAGNAME = XV_ERROR_SLOPETAGNAME;

// EV错误定义
static const int EV_ERROR_EVTAGNAME = XV_ERROR_TAGNAME;

// TV错误定义
static const int TV_ERROR_TAGNAME = XV_ERROR_TAGNAME;
static const int TV_ERROR_HIGHLIMIT = XV_ERROR_HIGHLIMIT;
static const int TV_ERROR_LOWLIMIT = XV_ERROR_LOWLIMIT;
static const int TV_ERROR_TRACKTAGNAME = XV_ERROR_TRACKTAGNAME;

static const double INVALID_SAMPLED_VALUE = -9999.0;
static const double DEFAULT_MV_TESTHIGHLIMIT = 100.0;
static const double DEFAULT_MV_TESTLOWLIMIT = 1.0;
static const double DEFAULT_ENGINEER_HILIMIT = 10000.0;
static const double DEFAULT_ENGINEER_LOLIMIT = -1000.0;
static const int MAX_TESTING_POINTS = 300000;
static const int MPC_ONLINE_CONTROLLER_DATA_LEN = 10000;
static const int MPC_SIMULATION_CONTROLLER_DATA_LEN = 10000;
static const int MPC_REDICT_MODEL_LENGTH = 4000;

namespace zce {
    struct AppOptions;
}

extern std::string getVMRelativeFilePath(const std::string& host_dir, const std::string& vmname,
                                         const std::string& vmpath);

extern int zmpc_init(const struct zce::AppOptions* start_t);

extern int partPack(zce_byte* buf, int size, const zmpc::ProjectFull& _t, zce_uint64 mask);

extern int partUnpack(zce_uint64& mask, zmpc::ProjectFull& _t, const zce_byte* buf, int size);
