#pragma once
// ***************************************************************
//  xOptModelFixVars   version:  1.0   -  date:  2025/06/06
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xOpt.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
// 用于固定变量的模型类
// ***************************************************************
#include "xOptInc/xOptModelBase.h"

class xOptModelFixVars : public xOptModelBase {
    friend class xOptProblemFixVars;
  public:
    xOptModelFixVars();

    ~xOptModelFixVars() = default;

    int fixVar(std::vector<std::string>& varname, const std::string& model_name,
               xOptProblem* problem, const std::vector<int>& var_index);

    int addFixableVariables(const std::vector<std::pair<std::string, double>>& vars);

    int clearUnfixedVariables();

    int setProblemType(XOPTF_PROBLEM_TYPE) override;

    std::vector<xOptModelParameter> getParameters() const override;

    int setParameter(const xOptModelParameter& parameter) override;

    int setComponents(const std::vector<std::string>& components) override;

    int validateModel() const override;

    std::vector<std::string> getFixableVariables() const override;

    int fixVariables(const std::vector<std::string>& varnames) override;

    int unFixVariables(const std::vector<std::string>& varnames) override;

    std::vector<int> getFixedVariableIndexes() override;

    xOptParsedVariableArr getVariables() const override;

    int setVariableValue(const std::string& varname, double value) override;

    int getPortNum(bool is_input_port) const override;

    xOptVarCompMap getVarCompMap(bool is_input_port, int index) const override;

    xOptProblem* buildProblem() override;

  private:
    struct Impl;
    struct Impl* m_pimpl;
};