#pragma once
// ***************************************************************
//  xOptModelComp   version:  1.0   -  date:  2025/04/02
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xOpt.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
// Combine multiple models into a larger model.
// ***************************************************************
#include "xOptInc/xOptModelBase.h"

class xOptModelSym;

class xOptModelComp : public xOptModelBase {
    friend class xOptProblemComp;

    struct Impl;
    struct Impl* m_pimpl;

  public:
    xOptModelComp();

    ~xOptModelComp();

    int addModel(const std::string& name, xOptModelBase* sub_model);

    int addLink(const std::string& from_model_name, int from_var_index,
                const std::string& to_model_name, int to_var_index);

    int addLink(const std::string& from_model_name, const std::vector<std::string>& from_names,
                const std::string& to_model_name, const std::vector<int>& to_indexes);

    int addLink(const std::string& from_model_name, const std::vector<int>& from_indexes,
                const std::string& to_model_name, const std::vector<int>& to_indexes);

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
};