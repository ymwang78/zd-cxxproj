#pragma once
// ***************************************************************
//  xOptSolver   version:  1.0   -  date:  2025/03/31
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xRto.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

#include "xOptInterface.h"

class xOptProblem;

class XOPTIF_API xOptSolver {
  public :
    enum SOLVE_RESULT : int {
        RESULT_OPTIMAL              = 0,
        RESULT_FEASIBLE             = 1,

        RESULT_UNKNOWN              = -1,
        RESULT_INFEASIBLE           = -2,
        RESULT_UNBOUNDED            = -3,
        RESULT_ITER_LIMIT           = -4,
        RESULT_INVALID_SETTINGS     = -5,
        RESULT_NUMERICAL_ISSUES     = -6,
        RESULT_INVALID_PROBLEM      = -7,
        RESULT_USER_PAUSE           = -100,
    };
    using boolean = unsigned char;

    virtual ~xOptSolver() = 0;

    virtual xOptProblem* getProblem() const = 0;

    virtual int getStringOptions(const char* option_names[], const char* option_values[],
                           int& options_size) const = 0;

    virtual int setStringOptions(boolean option_results[], const char* option_names[], const char* option_values[],
                                 int options_size) = 0;

    virtual int getIntOptions(const char* option_names[], int option_values[],
                              int& options_size) const = 0;

    virtual int setIntOptions(boolean option_results[], const char* option_names[], const int option_values[],
                              int options_size) = 0;

    virtual int getDoubleOptions(const char* option_names[], double option_values[],
                                 int& options_size) const = 0;

    virtual int setDoubleOptions(boolean option_results[], const char* option_names[], const double option_values[],
                                 int options_size) = 0;

    virtual int solve() = 0;

    virtual int pauseSolve() = 0;

    virtual int continueSolve() = 0;

    virtual int X(double* x, int x_size) const = 0;

    virtual int F(double* x, int x_size) const = 0;

    virtual int Xmul(double* x, int x_size) const = 0;

    virtual int Fmul(double* x, int x_size) const = 0;

};

typedef xOptSolver* (*CreateSolverFunc)(const char*, xOptProblem*);
typedef void (*RelaseSolverFunc)(xOptSolver*);


