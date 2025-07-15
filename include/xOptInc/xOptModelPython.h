#pragma once
// ***************************************************************
//  xOptModelPython   version:  1.0   -  date:  2025/06/14
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xOpt.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

#include "xOptInc/xOptModelBase.h"

class xOptModelPython : public xOptModelBase {
  public:
    xOptModelPython(const std::string& python_module_file,
                    const std::string& factory_function = "createModel");

    ~xOptModelPython();

    int initializeModel() override;

    int setProblemType(XOPTF_PROBLEM_TYPE) override;

    int setComponents(const std::vector<std::string>& components) override;

    xOptModelParameters getParameters() const override;

    int setParameters(const xOptModelParameters& parameters) override;

    int validateModel() const override;

    int prepareRuntime(const xOptParsedVariableArr& arr) override;

    int setInPortVariableValues(const xOptModelFixableVariables& nvpairs) override;

    xOptModelFixableVariables getFixableVariables() const override;

    int fixVariables(const xOptModelFixableVariables& varnames) override;

    int getPortNum(bool is_input_port) const override;

    xOptVarCompMap getVarCompMap(bool is_input_port, int index) const override;

    xOptProblem* getProblem() const override;

    std::vector<ReportMetaInfo> getReportMetas() const override;

    ReportData getReportByMetaName(const std::string& name) const override;

};
