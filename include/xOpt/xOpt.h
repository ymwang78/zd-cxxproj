#pragma once
// ***************************************************************
//  xOpt   version:  1.0   -  date:  2025/03/31
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xOpt.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

#ifdef _WIN32
#    ifndef XOPT_API
#        ifdef XOPT_EXPORTS
#            define XOPT_API __declspec(dllexport)
#        else
#            define XOPT_API __declspec(dllimport)
#        endif
#    endif
#else
#    define ZCE_API __attribute__((visibility("default")))
#endif

#ifndef ZCE_DEFINED_LOGLEVEL
typedef enum _zlog_level {
    ZLOG_TRACE = 0,
    ZLOG_DEBUG = 1,   /* debug */
    ZLOG_INFOR = 2,   /* info */
    ZLOG_WARNI = 3,   /* warn */
    ZLOG_ERROR = 4,   /* error */
    ZLOG_FATAL = 5,   /* fatal */
    ZLOG_BIZDT = 100, /* bizdata */
    ZLOG_NONEL = 255, /* none */
} ZLOG_LEVEL;
#    define ZCE_DEFINED_LOGLEVEL
#endif

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <zce/zce_array.h>

extern "C" {
typedef void (*loggerFunc)(int level, const char* str);
void xOptSetLogger(loggerFunc logger);
void xOptSetLoggerLevel(int level);
}

using xOptIndexMap = std::map<std::string, std::pair<std::string, std::string>>;

struct xOptParsedExternal {
    std::string name;
    std::vector<int> args_size;
    int args_total;
    std::string eval_fn;
    std::string jac_fn;
    std::string struct_fn;
    std::string lib_path;
};
using xOptParsedExternalMap = std::unordered_map<std::string, xOptParsedExternal>;

struct xOptParsedMacro {
    std::string name;
    std::string expression;
    std::string index_expr;
    xOptIndexMap indexes;
    std::vector < std::vector<double> > values;
};
using xOptParsedMacroMap = std::unordered_map<std::string, xOptParsedMacro>;

using xOptParsedParamter = xOptParsedMacro;

using xOptParsedParamterMap = std::unordered_map<std::string, xOptParsedParamter>;

struct xOptParsedVariable {
    std::string name;
    std::string array_expr;  // 数组长度表达式
    double lower;
    double upper;
    std::vector<double> initial;
    std::string unit;
    bool fixed = false;
    int expand_index;  // 展开后的数组下标
};

using xOptParsedVariableMap = std::unordered_map<std::string, xOptParsedVariable>;
using xOptParsedVariableArr = zce_indexed_array<xOptParsedVariable, std::string, std::function<std::string(const xOptParsedVariable&)>>;

struct xOptParsedConstraint {
    std::string name;
    std::string expression;
    xOptIndexMap indexes;
    double lower;
    double upper;
    bool enabled = true;
};

using xOptParsedConstraintMap = std::unordered_map<std::string, xOptParsedConstraint>;
using xOptParsedConstraintArr = zce_indexed_array<xOptParsedConstraint, std::string, std::function<std::string(const xOptParsedConstraint&)>>;

struct xOptParsedModel {
    std::string name;
    std::string description;
    std::string objective_expr;
    std::vector<xOptParsedExternal> externals;
    std::vector<xOptParsedParamter> parameters;
    std::vector<xOptParsedMacro> macros;
    std::vector<xOptParsedVariable> variables;
    std::vector<xOptParsedConstraint> constraints;
};
