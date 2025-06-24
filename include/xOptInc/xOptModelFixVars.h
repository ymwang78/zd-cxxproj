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

    //================= 模型初始化 ==================

    int initializeModel() override;

    int setComponents(const std::vector<std::string>& components) override;

    int setProblemType(XOPTF_PROBLEM_TYPE) override;

    xOptModelParameters getParameters() const override;

    int setParameters(const xOptModelParameters& parameters) override;

    xOptModelFixableVariables getFixableVariables() const override;

    int fixVariables(const xOptModelFixableVariables& varnames) override;

    int validateModel() const override;

    //================= 模型运行时 ==================

    int prepareRuntime() override;

    xOptParsedVariableArr getVariables() const override;

    int setVariableValues(const std::vector<double>& values) override;

    int setVariableValue(const std::string& varname, double value) override;

    int getPortNum(bool is_input_port) const override;

    xOptVarCompMap getVarCompMap(bool is_input_port, int index) const override;

    xOptProblem* getProblem() const override;

    std::vector<ReportMetaInfo> getReportMetas() const override;
    
    ReportData getReportByMetaName(const std::string& name) const override;

  private:
    struct Impl;
    struct Impl* m_pimpl;
};