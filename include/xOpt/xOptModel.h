#pragma once
// ***************************************************************
//  xOptModel   version:  1.0   -  date:  2025/03/31
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xRto.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************

#include "xOptInterface.h"

class xOptProblem;

// ***************************************************************
// 注意：
// Model DLL 除了实现xOptModel子类之外，还必须提供创建模型和销毁模型的方法:
// 1. extern "C" xOptModel* createModel(const char* name);
// 2. extern "C" void freeModel(xOptModel*);
// ***************************************************************

class XOPTIF_API xOptModel {
  public:

    virtual ~xOptModel() = 0;

    // 设置组分列表：
    // 组分列表的名字约定为
    //["C1P", "C2P", "C3P", "C4P", "C5P", "C6P", "C7P", "C8P", "C9P", "C10P", "C6N", "C7N", "C8N",
    //"C9N", "C10N", "C6A", "C7A", "C8A", "C9A", "C10A", "H2"]
    virtual int setComponents(const char* names[], int size) = 0;

    // parameter指不在模型方程组中作为变量出现的参数，可能会影响方程结构
    // 例如：板效率
    virtual int getParameters(const char* names[], const char* values[], int& size) = 0;

    virtual int setParameter(const char* name, const char* value) = 0;

    virtual int setProblemType(XOPTF_PROBLEM_TYPE) = 0;

    // 获取可以被fix的变量，如：回流比、板效率
    // 根据用户界面配置是否被fix
    virtual int getFixableVariables(const char* names[], int& size) = 0;

    // 固定变量, 返回值参考XOPTF_ERRCODE
    virtual int fixVariables(const char* names[], int& size) = 0;

    // 获取进料流股的个数，不考虑能量流股
    virtual int getInPortNum() = 0;

    // 获取出料流股的个数，不考虑能量流股
    virtual int getOutPortNum() = 0;

    // 获取进料流股对应的变量映射表
    // streamNames可能为：T,P,fi_C1P,fi_C2P等等
    // variableNames为对应变量在模型内部的名称
    // 如果上游没有提供进料所对应的某个变量，例如P，则模型中该变量被固定在初始值
    virtual int getInPortVariableMap(int iPortIndex, const char* streamNames[],
                                     const char* variableNames[], int& size) = 0;

    // 获取出料流股对应的变量映射表
    virtual int getOutPortVariableMap(int iPortIndex, const char* streamNames[],
                                      const char* variableNames[], int& size) = 0;

    virtual int validateModel() const = 0;

    // 构造对应的xOptProblem
    virtual xOptProblem* buildProblem() = 0;
};
