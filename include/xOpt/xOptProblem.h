#pragma once
// ***************************************************************
//  xOptProblem   version:  1.0   -  date:  2025/03/31
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xOpt.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#include "xOptInterface.h"

class XOPTIF_API xOptProblem {
  public:
    enum OPTIONS {
        OPTIONS_MAGIC = 0,  // Magic number to check if the object is valid, must be 'X'
        HAS_DERIVATIVE,     // 0: auto-calculate Jacobian; 1: manual Jacobian provided
        HAS_LINEAR_A,       // 0: no linear constraints (A); 1: linear constraints supported
        IS_SIMULATION,      // 0: optimization; 1: simulation
        OPTIONS_LIMIT
    };

    struct SolveEquantionsOptions {
        double tol;    // Tolerance for convergence
        int max_iter;  // Maximum number of iterations
    };

    virtual ~xOptProblem() = 0;

    virtual int initialize() = 0;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Return >= 0 for success, < 0 for error

    // 配置接口

    virtual int numVariables() const = 0;

    virtual int numConstraints() const = 0;

    virtual int getVariableNames(const char* names[], int names_size) const = 0;

    virtual int getConstraintNames(const char* names[], int names_size) const = 0;

    virtual int getOptions(double* options, int options_size) const = 0;

    // x_size == numVariables()
    virtual int getVariableBounds(double* xlow, double* xupp, int x_size) const = 0;

    // c_size == numConstraints()
    virtual int getConstraintBounds(double* clow, double* cupp, int c_size) const = 0;

    // x0_size == numVariables()
    virtual int getInitialX(double* x0, int x0_size) const = 0;

    // Linear constraints (A matrix) structure:
    // Usage:
    // 1. First call with lcons_rowidx or lcons_colidx or values == nullptr to query size.
    // 2. Allocate memory and second call to fill data.
    virtual int getLinearConstraints(int* lcons_rowidx, int* lcons_colidx, double* values,
                                     int& lcons_size) const = 0;

    // Objective Jacobian structure (column indices only)
    // Usage:
    //   1. First call with obj_colidx == nullptr to query size.
    //   2. Allocate memory and second call to fill data.
    virtual int getObjectiveGradientStructure(int* obj_colidx, int& obj_colidx_size) const = 0;

    // Constraint Jacobian structure (row and column indices)
    // Usage:
    //   1. First call with cons_rowidx or cons_colidx == nullptr to query size.
    //   2. Allocate memory and second call to fill data.
    virtual int getConstraintJacobianStructure(int* cons_rowidx, int* cons_colidx,
                                               int& nnz) const = 0;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    // 求解运行迭代接口

    // set X (x_size == numVariables())
    // 后续运行迭代接口都需要先调用setX()，否则会报错
    virtual int setX(const double* x, int x_size) = 0;

    // 返回当前点是否有效，在求解过程中调用
    virtual int runTimeCheck() const = 0;

    // Evaluate objective function
    virtual int evaluateObjective(double& obj) const = 0;

    // Evaluate constraint functions (cons_size == numConstraints())
    virtual int evaluateConstraints(double* cons, int cons_size) const = 0;

    // Evaluate objective gradient (x_size == numVariables(), grad_size == jCol_size)
    virtual int evaluateObjectiveGradient(double* grad, int grad_size) const = 0;

    // Evaluate constraint Jacobian values (x_size == numVariables(), values_size == iRow_jCol_size)
    virtual int evaluateConstraintsJacobianValues(double* values, int values_size) const = 0;
};
