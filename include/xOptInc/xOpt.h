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
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <zce/zce_array.h>
#include <zce/zce_object.h>
#include <zce/zce_object_counter.h>
#include "xOpt/xOptSolver.h"
#include "xOpt/xOptProblem.h"

#ifndef XOPT_API
#    ifdef _WIN32
#        ifdef XOPT_EXPORTS
#            define XOPT_API __declspec(dllexport)
#        else
#            define XOPT_API __declspec(dllimport)
#        endif
#    else
#        define XOPT_API __attribute__((visibility("default")))
#    endif
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

class xOptSolver;
class xOptProblem;
class xOptModelBase;

struct XOPT_API xOptModelArgs {
    const char* model_type;
    const char* model_path;
    const char* problem_type;
    const char* problem_path;
    const char* thermo_path;
};

struct SolverParameter {
    std::string name;
    std::string type;
    std::string default_value;
    std::string description;
};

struct SolverInfo : public zce::Object {
    std::string name;
    std::string solver_path;
    std::string status;
    CreateSolverFunc lpfn_create_solver;
    DestroySolverFunc lpfn_release_solver;
    std::vector<SolverParameter> parameters;
};
typedef zce::SmartPtr<SolverInfo> SolverInfoPtr;

class xOptSolverWrapper : public zce::Object {
    SolverInfoPtr solver_info_;
    xOptSolver* solver_;

  public:
    xOptSolverWrapper(xOptSolver* solver, const SolverInfoPtr& solver_info);

    ~xOptSolverWrapper();

    xOptSolver* operator->() const { return solver_; };

    xOptSolver* get() const { return solver_; }
};

typedef zce::SmartPtr<xOptSolverWrapper> xOptSolverWrapperPtr;

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
    std::vector<std::vector<double>> values;
};
using xOptParsedMacroMap = std::unordered_map<std::string, xOptParsedMacro>;

using xOptParsedParamter = xOptParsedMacro;

using xOptParsedParamterMap = std::unordered_map<std::string, xOptParsedParamter>;

struct xOptParsedVariable {
    std::string name;
    double lower;
    double upper;
    double initx;
    double current;  // 当前值
    int flags;
    std::string link_variable;
    std::string unit;
    double precision;
    std::string external_tag;
    std::string description;
    std::string array_expr;  // 数组长度表达式
    int expand_index;        // 展开后的数组下标
    std::vector<double> initial;
};

using xOptParsedVariableMap = std::unordered_map<std::string, xOptParsedVariable>;
using xOptParsedVariableArr =
    zce::ArrayWithIndex<xOptParsedVariable, std::string,
                        std::function<std::string(const xOptParsedVariable&)>>;

struct xOptParsedConstraint {
    std::string name;
    std::string expression;
    xOptIndexMap indexes;
    double lower;
    double upper;
    bool enabled = true;
};

using xOptParsedConstraintMap = std::unordered_map<std::string, xOptParsedConstraint>;
using xOptParsedConstraintArr =
    zce::ArrayWithIndex<xOptParsedConstraint, std::string,
                        std::function<std::string(const xOptParsedConstraint&)>>;

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

class xOptProblemBase : public zce::Object, public xOptProblem {
  public:
    virtual ~xOptProblemBase() {}
};

typedef zce::SmartPtr<xOptProblemBase> xOptProblemBasePtr;

class XOPT_API xOpt {
  public:
    static const std::map<std::string, SolverInfoPtr>& getSolvers();

    static int registerSolver(const SolverInfoPtr& solver);

    static xOptSolverWrapperPtr createSolver(const char* name, xOptProblem* problem,
                                                     const char* solver_name,
                                    xOptLogFunc loggerFunction);

    static void printX(xOptSolver*);

    static void printF(xOptSolver*);

    static xOptProblemBasePtr createProblem(const char* problem_name, const char* problem_type,
                                      const char* problem_path, const void* arg);

    static xOptModelBase* createModel(const char* model_name, const xOptModelArgs* context);
};

extern "C" {
void xOptLogPrintX(const double* x, int x_size);
}
