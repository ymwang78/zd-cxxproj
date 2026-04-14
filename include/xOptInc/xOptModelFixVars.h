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
    ZCE_OBJECT_DECLARE;

    friend class xOptProblemFixVars;

  public:
    xOptModelFixVars();

    ~xOptModelFixVars() = default;

    //int fixVar(std::vector<std::string>& varname, const std::string& model_name,
    //           xOptProblem* problem, const std::vector<int>& var_index);

    //int fixVar(std::vector<std::string>& varname, const std::string& model_name,
    //           xOptProblem* problem, const xOptModelFixableVariables& tofix);

    //int fixVar(std::vector<std::string>& varname, std::vector<int>& var_indexes,
    //                 xOptModelBase* ref_model, const xOptModelFixableVariables& tofix);

    //================= 模型初始化 ==================

    int initializeModel() override;

    int setComponents(const std::vector<std::string>& components) override;

    int setProblemType(XOPTF_PROBLEM_TYPE) override;

    xOptModelParameters getParameters() const override;

    int setParameters(const xOptModelParameters& parameters) override;

    xOptModelFixableVariables getFixableVariables() const override;

    int validateModel() const override;

    //================= 模型运行时 ==================

    int prepareRuntime(const xOptParsedVariableArr& arr) override;

    int getPortNum(bool is_input_port) const override;

    xOptVarCompMap getVarCompMap(bool is_input_port, int index) const override;

    std::vector<ReportMetaInfo> getReportMetas() const override;

    ReportData getReportByMetaName(const std::string& name) const override;

    //================= Slate / 组分体系接口 ==================

    /// 本模型仅使用 1 个 Slate（对应唯一输出端口）。
    int getNumberOfSlate() const override;

    /// 输出端口 0 使用 Slate 0；输入端口返回 -1（本模型无输入端口）。
    int getSlateIdOfPort(bool is_input_port, int port_index) const override;

    /// slate_index 只接受 0；将 slate 中的组分列表传入 setComponents()。
    int setSlate(int slate_index, const xOptSlate* slate) override;
};
