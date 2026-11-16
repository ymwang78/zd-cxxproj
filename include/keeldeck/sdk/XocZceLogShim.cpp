// ***************************************************************
//  XocZceLogShim —— zce 日志宏在插件侧的落点（仅 Windows）
//  -------------------------------------------------------------
//  This file is a part of project KeelDeck.
// ***************************************************************
//
// 插件里但凡编进一段展开成 ZCE_DEBUG / ZERROR / ZTRACE 的代码，就会引用 libzce 日志层的
// zlog_getlevel() / zlog_logv()。两个来源都躲不掉：
//
//   - zgen 生成的 *_pack.cpp 对 `[o] x[0~1]` 这类可选成员越界时打一条 ZCE_DEBUG；
//   - libQTExt 的 xTableView.cpp / xItemDelegate.cpp 里的日志（issue #264 / M3a）。
//
// 而这两个符号**不在** keeldeck_core.def 的再导出清单里，还正是
// tools/check_plugin_symbols.ps1 的 $forbiddenPatterns 明令禁止导入的名字
// （'zlog_'）——那条禁令要留着：插件真去绑核心的 zlog，等于承认插件可以直接用
// libzce 的日志层，而日志层带全局状态（级别、文件句柄、锁）。
//
// 所以给出两个**同名的本地定义**，把日志转给核心的 xocLog——落在同一个 LogView、
// 同一份 zlog 文件里。它们既不导出也不导入，符号闸门根本看不见；进程里的 zlog 状态
// 仍然只有核心那一份。Linux 上核心 .so 默认导出全部符号，插件直接绑到真身，
// 本文件整份是空的。
//
// 用法：插件 target 把本文件加进自己的源列表即可，一个插件一份本地定义，互不干扰。
// 想让日志前缀带上插件名就定义 XOC_ZCE_LOG_SHIM_TAG（默认 "plugin"）：
//
//     target_sources(FooPlugin PRIVATE framework/sdk/XocZceLogShim.cpp)
//     target_compile_definitions(FooPlugin PRIVATE XOC_ZCE_LOG_SHIM_TAG="FooPlugin")
//
// 这是 #223 接口的一个已知缺口（生成代码与第三方库依赖日志宏，而白名单没有给日志
// 留位置），记录在 modules/FrontSensor/plugin/README.md "已知接口缺口"。
#ifdef _WIN32

#include "XocDiagnostics.h"

#include <zce/zce_log.h>

#include <QString>
#include <cstdarg>
#include <cstdio>

#ifndef XOC_ZCE_LOG_SHIM_TAG
#    define XOC_ZCE_LOG_SHIM_TAG "plugin"
#endif

unsigned zlog_getlevel() {
    // 调用方（生成代码、libQTExt）只在 ZLOG_DEBUG 级别打日志；核心的 LogView 本来
    // 也按级别过滤，这里放行，过滤留给核心那一份配置。
    return ZLOG_DEBUG;
}

void zlog_logv(unsigned level, const char* funcname, const char* filename, int linenumber,
               const char* fmt, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buffer, sizeof(buffer), fmt ? fmt : "", args);
    va_end(args);

    XocLogLevel mapped = XocLogLevel::Debug;
    if (level >= ZLOG_ERROR) {
        mapped = XocLogLevel::Error;
    } else if (level >= ZLOG_WARNI) {
        mapped = XocLogLevel::Warning;
    } else if (level >= ZLOG_INFOR) {
        mapped = XocLogLevel::Info;
    }
    xocLog(mapped, QStringLiteral("%1 zce: %2 (%3 %4:%5)")
                       .arg(QStringLiteral(XOC_ZCE_LOG_SHIM_TAG),
                            QString::fromUtf8(buffer).trimmed(),
                            QString::fromUtf8(funcname ? funcname : ""),
                            QString::fromUtf8(filename ? filename : ""))
                       .arg(linenumber));
}

#endif  // _WIN32
