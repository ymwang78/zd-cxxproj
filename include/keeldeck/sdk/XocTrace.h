#pragma once

// ***************************************************************
//  XocTrace —— ZTRACE 的插件安全替身（header-only）
//  -------------------------------------------------------------
//  This file is a part of project KeelDeck.
// ***************************************************************
//
// ZTRACE(a, b, c) 是 libzce 的流式变参日志宏：把每个实参 operator<< 进一个
// ostringstream，每个后面跟一个 '|'，然后整条交给 zlog。宏本身要 zlog_getlevel /
// zce::Logger 两个 libzce 符号，插件按 #223 的 B 模式碰不得。
//
// 这里按同样的规则重新实现一遍，落点换成 XocDiagnostics.h 的 xocLog(Trace, ...)：
//   - 每个实参后面补一个 '|'（含最后一个，与 zce::Logger 一致，末尾确实带分隔符）；
//   - float 用 setprecision(8)，double 用 setprecision(16)；
//   - const char* 为空指针时打 "null"，不是崩。
// 这样把调用点从 ZTRACE(...) 改成 xocTrace(...) 是纯改名，文案逐字不变。
//
// 放在 framework/sdk/ 而不是 framework/interfaces/：这是模板，整份编进调用方自己的
// 二进制，不跨 DLL 边界，不属于 ABI 面。

#include "XocDiagnostics.h"

#include <QByteArray>
#include <QString>
#include <iomanip>
#include <sstream>
#include <string>

namespace xoc_trace_detail {

inline void appendArg(std::ostringstream& os, float v) { os << std::setprecision(8) << v << '|'; }

inline void appendArg(std::ostringstream& os, double v) { os << std::setprecision(16) << v << '|'; }

inline void appendArg(std::ostringstream& os, const char* v) { os << (v ? v : "null") << '|'; }

inline void appendArg(std::ostringstream& os, const QString& v) {
    os << v.toUtf8().constData() << '|';
}

inline void appendArg(std::ostringstream& os, const QByteArray& v) { os << v.constData() << '|'; }

template <typename T>
inline void appendArg(std::ostringstream& os, const T& v) {
    os << v << '|';
}

inline void appendArgs(std::ostringstream&) {}

template <typename T, typename... Rest>
inline void appendArgs(std::ostringstream& os, const T& first, const Rest&... rest) {
    appendArg(os, first);
    appendArgs(os, rest...);
}

}  // namespace xoc_trace_detail

/// 与 ZTRACE / ZERROR 同形同文的流式日志
template <typename... Args>
inline void xocLogStream(XocLogLevel level, const Args&... args) {
    std::ostringstream os;
    xoc_trace_detail::appendArgs(os, args...);
    xocLog(level, QString::fromStdString(os.str()));
}

/// ZTRACE(...) 的替身
template <typename... Args>
inline void xocTrace(const Args&... args) {
    xocLogStream(XocLogLevel::Trace, args...);
}

/// ZERROR(...) 的替身（流式，不是 printf —— 与被替换的宏一致）
template <typename... Args>
inline void xocError(const Args&... args) {
    xocLogStream(XocLogLevel::Error, args...);
}
