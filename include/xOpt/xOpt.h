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

#ifndef XOPT_API
#    ifdef _WIN32
#        ifdef XOPT_EXPORTS
#            define XOPT_API __declspec(dllexport)
#        else
#            define XOPT_API __declspec(dllimport)
#        endif
#    else
#        define ZCE_API __attribute__((visibility("default")))
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

struct SolverInfo {
    std::string name;
    std::string solver_path;
    std::vector<SolverParameter> parameters;
};

class XOPT_API xOpt {
  public:
    static int registerSolver(const SolverInfo& solver);

    static xOptSolver* createSolver(const char* name, xOptProblem* problem,
                                    const char* solver_name = "IPOPT");

    static void printX(xOptSolver*);

    static void printF(xOptSolver*);

    static xOptProblem* createProblem(const char* problem_name, const char* problem_type,
                                      const char* problem_path, const void* arg);

    static xOptModelBase* createModel(const char* model_name, const xOptModelArgs* context);
};
