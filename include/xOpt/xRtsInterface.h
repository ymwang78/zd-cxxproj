#pragma once
// ***************************************************************
//  xRtsInterface   version:  1.0   -  date:  2025/08/07
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xRto.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

#include <QStringList>

#ifdef RTSCORE_LIBRARY
#    define RTSCORE_API Q_DECL_EXPORT
#else
#    define RTSCORE_API Q_DECL_IMPORT
#endif

#ifndef ZCE_DEFINED_LOGLEVEL
typedef enum _zlog_level {
    ZLOG_TRACE = 0,
    ZLOG_DEBUG = 1,   /* debug */
    ZLOG_INFOR = 2,   /* info */
    ZLOG_WARNI = 3,   /* warn */
    ZLOG_ERROR = 4,   /* error */
    ZLOG_FATAL = 5,   /* fatal */
    ZLOG_BIZDT = 6, /* bizdata */
    ZLOG_NONEL = 7, /* none */
} ZLOG_LEVEL;
#    define ZCE_DEFINED_LOGLEVEL
#endif

class xRtsCallbackInterface {
  public:
    // 获取可用模型信息
    virtual QStringList getFlowSheetInfo() = 0;

    // 获取某个模型的计算任务列表
    virtual QStringList getFlowSheetComputeTask(QString flowSheetName) = 0;

    // 切换当前计算任务 0-成功，负值失败
    virtual int setCurrentComputeTask(QString flowSheetName, QString computeTaskName) = 0;

    // 获取模型下可调用的宏信息
    virtual QStringList getMacroInfo(QString flowSheetName) = 0;

    // 调用平台宏 0-成功，负值失败
    // envDict: JSON string with environment variables passed to the macro, e.g. {"key": "value"}
    /*
    返回值:
    - Python/Lua 宏需要将完整结果写入 __rts_result__
    - 平台直接将 __rts_result__ 作为 JSON 返回给 RTS
    例如: {
        "errcode": 0,
        "errdesc": "ok",
        "data": {}
    }
    */
    virtual QString callMacro(QString flowSheetName, QString macroName, QString envDict) = 0;

    // 调用模型求解
    /*
    sInfo参数：{
        "lastfail":-1,
        "failNum": 0
    }
    返回值: {
        "errcode": 0,
        "errdesc": "123",
        "data":  {
            "optvarlist" : {
            "optVar2" : 231.2,
            "optVar1" : 231.2,
            "optVar3" : 231.2,
            "optVar4" : 231.2
            }
        }
    }
    */
    virtual QString solve(QString flowSheetName, QString computeTaskName, QString sInfo) = 0;

    // 打印日志
    virtual void appendLog(ZLOG_LEVEL level, QString message) = 0;
};

class QWidget;

class RTSCORE_API xRtsInterface {
  public:

    virtual ~xRtsInterface() = default;

    // 创建 RTS Widget，返回 Widget 句柄
    virtual QWidget* createRtsWidget(QWidget* parent) = 0;

    // 销毁 RTS Widget，释放 Widget 资源
    virtual void destroyRtsWidget() = 0;

    // 设置平台回调接口
    virtual int setRtsCallback(xRtsCallbackInterface*) = 0;

    // 导出 RTS 项目配置（用于平台持久化）
    virtual QString exportProject() = 0;

    // 导入 RTS 项目配置（用于恢复持久化数据）
    virtual int importProject(QString) = 0;

    // 启动 RTS 实例，0-成功，负值失败
    virtual int startRts() = 0;

    // 停止 RTS 实例，0-成功，负值失败
    virtual int stopRts() = 0;

    // 检查 RTS 实例是否正在运行
    virtual bool isRtsRunning() = 0;

    // 重置 RTS 实例到初始状态，0-成功，负值失败
    virtual int resetRts() = 0;
};

extern "C" {
RTSCORE_API xRtsInterface* createRtsInterface();
RTSCORE_API void destroyRtsInterface(xRtsInterface*);
}
