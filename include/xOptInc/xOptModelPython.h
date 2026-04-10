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
    ZCE_OBJECT_DECLARE;

  public:
    xOptModelPython(const std::string& python_module_file,
                    const std::string& factory_function = "createModel");

    ~xOptModelPython();

    int initializeModel() override;

    int setProblemType(XOPTF_PROBLEM_TYPE) override;

    int setComponents(const std::vector<std::string>& components) override;

    xOptModelParameters getParameters() const override;

    xOptNameStrValueMap getParameterDescs() const override;

    int setParameters(const xOptModelParameters& parameters) override;

    int validateModel() const override;

    int prepareRuntime(const xOptParsedVariableArr& arr) override;

    int generateEstimate(std::vector<double>& init_x) override;

    int setInPortVariableValues(const xOptModelFixableVariables& nvpairs) override;

    xOptModelFixableVariables getFixableVariables() const override;

    int fixVariables(const xOptModelFixableVariables& varnames,
                     bool try_fixed_in_model_first) override;

    int getPortNum(bool is_input_port) const override;

    xOptVarCompMap getVarCompMap(bool is_input_port, int index) const override;

    std::vector<ReportMetaInfo> getReportMetas() const override;

    ReportData getReportByMetaName(const std::string& name) const override;

    //================= Slate / 组分体系接口 ==================

    /// 委托 Python 对象的 getNumberOfSlate()；若方法不存在则返回 -1。
    int getNumberOfSlate() const override;

    /// 委托 Python 对象的 getSlateIdOfPort()；若方法不存在则返回 -1。
    int getSlateIdOfPort(bool is_input_port, int port_index) const override;

    /// 将 slate 的名称、热力学方法、组分列表传给 Python 的 setSlate()；
    /// 若方法不存在则回退到 setComponents()。
    int setSlate(int slate_index, const xOptSlate* slate) override;

    //================= 语言/国际化接口 ==================

    /// 将语言代码传给 Python 对象的 setLanguage()；若方法不存在则忽略。
    int setLanguage(const std::string& lang) override;
};
