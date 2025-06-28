#pragma once
// ***************************************************************
//  xOptModelBlackBox   version:  1.0   -  date:  2025/05/05
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xOpt.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
// 真正的BlackBox Model
// 用户提供了一个实现了xOptModel接口的动态库
// ***************************************************************
#include "xOptInc/xOptModelBase.h"

class xOptModelBlackBox : public xOptModelBase {
  public:
    // 构造函数接受动态库路径
    xOptModelBlackBox(const std::string& dll_path);

    // 析构函数
    ~xOptModelBlackBox();

    int initializeModel() override;

    int setProblemType(XOPTF_PROBLEM_TYPE) override;

    int setComponents(const std::vector<std::string>& components) override;

    xOptModelParameters getParameters() const override;

    int setParameters(const xOptModelParameters& parameters) override;

    xOptModelFixableVariables getFixableVariables() const override;

    int fixVariables(const xOptModelFixableVariables& varnames) override;

    int validateModel() const override;

    int prepareRuntime(const xOptParsedVariableArr& arr) override;  // 运行时准备

    int getPortNum(bool is_input_port) const override;

    xOptVarCompMap getVarCompMap(bool isInPort, int iIndex) const override;

    xOptProblem* getProblem() const override;

    std::vector<ReportMetaInfo> getReportMetas() const override;

    ReportData getReportByMetaName(const std::string& name) const override;
};
