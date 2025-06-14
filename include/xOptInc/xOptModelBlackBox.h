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

    // 实现xOptModelBase的所有接口
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

  private:
    struct Impl;
    struct Impl* m_pimpl;
};
