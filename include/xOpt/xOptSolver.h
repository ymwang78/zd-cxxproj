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
    enum SOLVE_RESULT {
        RESULT_UNKNOWN = 0,
        RESULT_OPTIMAL = 1,
        RESULT_FEASIBLE = 2,
        RESULT_INFEASIBLE = 3,
        RESULT_UNBOUNDED = 4,
        RESULT_ITER_LIMIT = 5,
    };

    enum SOLVE_OPTIONS {
        OPT_MAX_ITER = 0,
        OPT_NONDERIVATIVE_LINESEARCH,
        OPT_VERIFY_GRADIENT,
        OPT_OPTIONS_LIMIT,
    };

    virtual ~xOptSolver() = 0;

    virtual xOptProblem* getProblem() const = 0;

    virtual int getParameters(double* parameters, int parameters_size) const = 0;

    virtual void setParameters(const double* parameters, int parameters_size) = 0;

    virtual int setParameter(SOLVE_OPTIONS option, double value) = 0;

    virtual int solve() = 0;

    virtual int continueSolve() = 0;

    virtual int X(double* x, int x_size) const = 0;

    virtual int F(double* x, int x_size) const = 0;

    virtual int Xmul(double* x, int x_size) const = 0;

    virtual int Fmul(double* x, int x_size) const = 0;

};
