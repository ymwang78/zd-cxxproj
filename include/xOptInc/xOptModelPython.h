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

    int initialize() override;

    int setProblemType(XOPTF_PROBLEM_TYPE) override;

    xOptModelParameters getParameters() const override;

    int setParameter(const xOptModelParameter& parameter) override;

    int setParameters(const xOptModelParameters& parameters) override;

    int setComponents(const std::vector<std::string>& components) override;

    int validateModel() const override;

    xOptModelFixableVariables getFixableVariables() const override;

    int fixVariables(const xOptModelFixableVariables& varnames) override;

    xOptParsedVariableArr getVariables() const override;

    int setVariableValue(const std::string& varname, double value) override { return 0; };

    int getPortNum(bool is_input_port) const override;

    xOptVarCompMap getVarCompMap(bool is_input_port, int index) const override;

    std::vector<int> getCompVariableIndexes(const xOptStreamType& stream, bool is_input_port,
                                            int index) const override;

    xOptProblem* buildProblem() override;

    xOptProblem* getProblem() const override;

  private:
    struct Impl;
    struct Impl* m_pimpl;
};
