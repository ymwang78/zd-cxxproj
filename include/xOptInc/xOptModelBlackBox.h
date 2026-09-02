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
    // dll_path   动态库路径
    // create_arg 按单元的配置串，由调用方给定，本类只负责原样转发给 DLL 的
    //            xOptModel_createModel 的 name 形参——内容不作解释，含义由
    //            那个 DLL 自己定。空串时仍传历史值 "BlackBoxModel"，所以既有
    //            调用方不受影响。以上是本构造函数的全部契约。
    //
    //            为什么需要它：黑箱 DLL 能从宿主拿到的只有自己的路径，
    //            部署配置里其余字段一个都到不了。有些 DLL 需要按单元区分
    //            ——例如 xRtoCapeOpen.dll 要知道连哪个 CAPE-OPEN 组件。
    //
    //            这个值从哪来是**调用方的约定，不是这里的规定**：xOpt 的
    //            createModel 目前拿 ProblemPath 来填（该字段在
    //            ModelType="BlackBox" 下本来就闲置），别的集成点大可以另有
    //            来源。写在这里只为让读者找得到现成的例子。
    xOptModelBlackBox(const std::string& dll_path, const std::string& create_arg = std::string());

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
