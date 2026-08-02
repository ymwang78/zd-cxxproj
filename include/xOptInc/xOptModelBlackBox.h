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
    ZCE_OBJECT_DECLARE;

  public:
    // 构造函数接受动态库路径
    xOptModelBlackBox(const std::string& dll_path);

    // 析构函数
    ~xOptModelBlackBox();

    int initializeModel() override;

    int setProblemType(XOPTF_PROBLEM_TYPE) override;

    std::string getVersion() const override;

    xOptModelFixableVariables getFixableVariables() const override;

    xOptModelParameters getParameters() const override;

    int setParameters(const xOptModelParameters& parameters) override;

    int validateModel() const override;

    int prepareRuntime(const xOptParsedVariableArr& arr) override;  // 运行时准备

    int generateEstimate(std::vector<double>& init_x,
                         const xOptModelFixableVariables& fixed_values) override;

    int getPortNum(bool is_input_port) const override;

    xOptVarCompMap getVarCompMap(bool isInPort, int iIndex) const override;

    std::vector<ReportMetaInfo> getReportMetas() const override;

    ReportData getReportByMetaName(const std::string& name) const override;

    // ***************************************热力学************************************************

    // 获取本模块支持的slate类型数量
    virtual int getNumberOfSlate() const override;

    // 获取每个端口使用的slate id
    virtual int getSlateIdOfPort(bool is_input_port, int port_index) const override;

    // slate_index是模块支持的slate类型索引，slate是具体的slate数据结构，返回值参考XOPTF_ERRCODE
    virtual int setSlate(int slate_index, const xOptSlate* slate) override;

    // ****************************************设置语言************************************************
    virtual int setLanguage(const std::string& language_code) override;
};
