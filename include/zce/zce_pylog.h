#pragma once
// ***************************************************************
//  zce_pylog   version:  1.0   -  date: 2026/06/20
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  统一的 Python 输出日志级别识别策略（供各项目的 stdout/stderr
//  重定向器共享，避免在 mpc / xRto 等处各写一套且行为不一致）。
//  Copyright (C) 2026 - All Rights Reserved
// ***************************************************************
#include <zce/zce_log.h>

#include <cctype>
#include <string>

namespace zce {

// 级别名（含常见别名）-> ZLOG 级别；大小写、内部/两端空白均不敏感。
// 未识别返回 -1，由调用方决定回退级别。
inline int pylog_map_level(const std::string& lvl_in) {
    std::string lvl;
    lvl.reserve(lvl_in.size());
    for (char c : lvl_in) {
        if (std::isspace((unsigned char)c)) continue;
        lvl.push_back((char)std::toupper((unsigned char)c));
    }
    if (lvl == "CRITICAL" || lvl == "FATAL") return ZLOG_FATAL;
    if (lvl == "ERROR" || lvl == "ERR") return ZLOG_ERROR;
    if (lvl == "WARNING" || lvl == "WARN" || lvl == "WARNI") return ZLOG_WARNI;
    if (lvl == "INFO" || lvl == "INFOR" || lvl == "INFORMATION") return ZLOG_INFOR;
    if (lvl == "DEBUG") return ZLOG_DEBUG;
    if (lvl == "TRACE") return ZLOG_TRACE;
    return -1;  // 未识别
}

// 根据捕获到的一行文本内容判定日志级别，支持以下三种常见格式：
//   "[hh:mm:ss] [LEVEL] msg"  —— 带时间戳的方括号格式
//   "[LEVEL] msg"             —— 裸方括号格式（出现在行内任意位置）
//   "LEVEL:logger:msg"        —— Python logging 默认格式
// 识别成功时把级别标记从 out_msg 中剥离（避免与日志视图的级别列重复）；
// 未识别到任何级别标记时返回 default_level，out_msg 为原文。
inline int pylog_detect_level(const std::string& line, int default_level, std::string& out_msg) {
    out_msg = line;

    // 形式 1/2：扫描每个 [...] 片段，取第一个内容为已知级别名的片段。
    // 这样 "[17:56:17] [ERROR] ..." 会跳过时间戳、命中 [ERROR]。
    size_t search = 0;
    while (true) {
        size_t lb = line.find('[', search);
        if (lb == std::string::npos) break;
        size_t rb = line.find(']', lb + 1);
        if (rb == std::string::npos) break;

        int level = pylog_map_level(line.substr(lb + 1, rb - lb - 1));
        if (level >= 0) {
            size_t m = rb + 1;
            while (m < line.size() && std::isspace((unsigned char)line[m])) ++m;
            out_msg = (m < line.size()) ? line.substr(m) : std::string();
            return level;
        }
        search = rb + 1;
    }

    // 形式 3：Python logging 默认的 "LEVEL:logger:msg" 前缀。
    size_t colon = line.find(':');
    if (colon != std::string::npos) {
        int level = pylog_map_level(line.substr(0, colon));
        if (level >= 0) {
            size_t m = colon + 1;
            while (m < line.size() && std::isspace((unsigned char)line[m])) ++m;
            out_msg = (m < line.size()) ? line.substr(m) : std::string();
            return level;
        }
    }

    return default_level;
}

}  // namespace zce
