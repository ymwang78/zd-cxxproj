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
    ZCE_OBJECT_DECLARE;

    friend class xOptProblemWithDesc;

  public:
    xOptModelProblemWithDesc(const zce::SmartPtr<xOptProblemBase>& external_problem,
                             const xOptModelDescT& desc);

    xOptModelProblemWithDesc(const zce::SmartPtr<xOptProblemBase>& external_problem,
                             xOptModelDescT&& desc);

    xOptModelProblemWithDesc(const zce::SmartPtr<xOptProblemBase>& external_problem,
                             const char* json_desc_path);

    ~xOptModelProblemWithDesc();

    int initializeModel() override;

    int setProblemType(XOPTF_PROBLEM_TYPE) override;

    xOptModelParameters getParameters() const override;

    int setParameters(const xOptModelParameters& parameters) override;

    xOptModelFixableVariables getFixableVariables() const override;

    int validateModel() const override;

    int prepareRuntime(const xOptParsedVariableArr& arr) override;

    const xOptParsedVariableArr& getVariables() const override;

    int setVariableValues(const std::vector<double>& values) override;

    xOptParsedVariable& getVariable(const std::string& varname, int hint_index) override;

    int setVariableValue(const std::string& varname, int hint_index, double value, bool update_initx) override;

    int getPortNum(bool is_input_port) const override;

    xOptVarCompMap getVarCompMap(bool isInPort, int iIndex) const override;

    std::vector<ReportMetaInfo> getReportMetas() const override;

    ReportData getReportByMetaName(const std::string& name) const override;

    //================= Slate / 组分体系接口 ==================

    /// 若模型有端口则使用 1 个 Slate，否则返回 -1。
    int getNumberOfSlate() const override;

    /// 所有端口均使用 Slate 0；无端口时返回 -1。
    int getSlateIdOfPort(bool is_input_port, int port_index) const override;

    int setSlate(int slate_index, const xOptSlate* slate) override;
};
