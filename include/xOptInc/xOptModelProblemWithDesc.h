#pragma once
// ***************************************************************
//  xOptModelProblemWithDesc   version:  1.0   -  date:  2025/05/05
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xOpt.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
// 用户只实现了一个xOptProblem,
// 从json描述文件获来构建出model
// ***************************************************************
#include "xOptInc/xOptModelBase.h"

class xOptModelProblemWithDesc : public xOptModelBase {
    friend class xOptProblemWithDesc;

  public:
    xOptModelProblemWithDesc(xOptProblem* external_problem, const xOptModelDescT& desc);

    xOptModelProblemWithDesc(xOptProblem* external_problem, xOptModelDescT&& desc);

    xOptModelProblemWithDesc(xOptProblem* external_problem, const char* json_desc_path);

    ~xOptModelProblemWithDesc();

    int initializeModel() override;

    int setProblemType(XOPTF_PROBLEM_TYPE) override;

    int setComponents(const std::vector<std::string>& components) override;

    xOptModelParameters getParameters() const override;

    int setParameters(const xOptModelParameters& parameters) override;

    xOptModelFixableVariables getFixableVariables() const override;

    int fixVariables(const xOptModelFixableVariables& varnames) override;

    int validateModel() const override;

    int prepareRuntime(const xOptParsedVariableArr& arr) override;

    const xOptParsedVariableArr& getVariables() const override;

    int setVariableValues(const std::vector<double>& values) override;

    const xOptParsedVariable& getVariable(const std::string& varname, int hint_index) override;

    int setVariableValue(const std::string& varname, int hint_index, double value) override;

    int getPortNum(bool is_input_port) const override;

    xOptVarCompMap getVarCompMap(bool isInPort, int iIndex) const override;

    std::vector<int> getFlowsheetFixedVariableIndexes() const override;

    xOptProblem* getProblem() const override;

    std::vector<ReportMetaInfo> getReportMetas() const override;

    ReportData getReportByMetaName(const std::string& name) const override;

  private:
    struct Impl;
    struct Impl* m_pimpl;
};
