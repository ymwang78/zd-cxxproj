#pragma once
// ***************************************************************
//  xOptModelSym   version:  1.0   -  date:  2025/04/02
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xRto.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
// SymPy compatible model
// ***************************************************************
#include "xOptInc/xOptModelBase.h"

class xOptModelSym : public xOptModelBase {
  public:

    xOptModelSym();

    ~xOptModelSym();

    int fixVar(std::vector<std::string>& varname, const char* target_model_name,
               xOptProblem* problem, const std::vector<int>& var_index);

    //////////////////////////////////////////////////////////////////////////////////
    // 以下函数为xOptModelBase的接口
    //////////////////////////////////////////////////////////////////////////////////

    int setProblemType(XOPTF_PROBLEM_TYPE) override;

    std::vector<xOptModelParameter> getParameters() const override;

    int setParameter(const xOptModelParameter& parameter) override;

    int setComponents(const std::vector<std::string>& components) override;

    int validateModel() const override;

    std::vector<std::string> getFixableVariables() const override;

    int fixVariables(const std::vector<std::string>& varnames) override;

    int unFixVariables(const std::vector<std::string>& varnames) override { return -1; };

    std::vector<int> getFixedVariableIndexes() override { return std::vector<int>(); }

    xOptParsedVariableArr getVariables() const override;

    int getPortNum(bool is_input_port) const override;

    xOptVarCompMap getVarCompMap(bool isInPort, int iIndex) const override;

    xOptProblem* buildProblem() override;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    int loadFromFile(const std::string& filepath);

    int addParameter(const std::string& name, const std::string& array_expr,
                     const std::vector<std::vector<double> >& vec, bool modify_if_exits);

    int delParameter(const std::string& name);

    int addMacro(const xOptParsedMacro& macro, bool modify_if_exits);

    int delMacro(const std::string& name);

    int setObjectExpr(const std::string& expr);

    int addVariable(const std::string& name, const std::string& array_expr, double low_bound,
                    double upper_bound, const std::vector<double>& initial, const std::string& unit,
                    bool modify_if_exists);

    int addConstraintExpr(const std::string& name, const std::string& expr,
                          const xOptIndexMap& index_map, double low_bound, double upper_bound,
                          bool modify_if_exists);

  private:
    struct Impl;
    struct Impl* m_pimpl;
};
