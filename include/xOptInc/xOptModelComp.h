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
    ZCE_OBJECT_DECLARE;

    friend class xOptProblemComp;

  public:
    xOptModelComp();

    ~xOptModelComp();

    int addModel(const std::string& name, const zce::SmartPtr<xOptModelBase>& sub_model,
                 bool keep_problem_null = false, bool force_prepare = false);

    int addLink(const std::string& from_model_name, int from_var_index,
                const std::string& from_var_name, const std::string& to_model_name,
                int to_var_index, const std::string& to_var_name);

    int addLink(const std::string& from_model_name, const std::vector<std::string>& from_names,
                const std::string& to_model_name, const std::vector<int>& to_indexes,
                bool overwrite_initx);

    int addLink(const std::string& from_model_name, const std::vector<int>& from_indexes,
                const std::string& to_model_name, const std::vector<int>& to_indexes);

    int fullfillInitialValue(const std::string& model_name,
                             const std::vector<const char*>& variable_names,
                             const std::vector<double>& initial_values);

    int printX() const;

    //================= 模型初始化 ==================
    int initializeModel() override;

    //================= 模型配置 ==================
    int setProblemType(XOPTF_PROBLEM_TYPE) override;

    int setComponents(const std::vector<std::string>& components) override;

    xOptModelParameters getParameters() const override;

    int setParameters(const xOptModelParameters& parameters) override;

    xOptModelFixableVariables getFixableVariables() const override;

    int fixVariables(const xOptModelFixableVariables& varnames,
                     bool try_fixed_in_model_first) override;

    int validateModel() const override;

    //================= 模型运行时 ==================
    int prepareRuntime(const xOptParsedVariableArr& arr) override;

    int setVariableValues(const std::vector<double>& values) override;

    int getPortNum(bool is_input_port) const override;

    xOptVarCompMap getVarCompMap(bool isInPort, int iIndex) const override;

    std::vector<ReportMetaInfo> getReportMetas() const override;

    ReportData getReportByMetaName(const std::string& name) const override;
};