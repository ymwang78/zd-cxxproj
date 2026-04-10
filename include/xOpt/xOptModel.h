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

#ifdef __cplusplus
extern "C" {
#endif

enum XOPT_PHASE { PHASE_LIQUID = 0, PHASE_VAPOR = 1, PHASE_MAX };

enum MIXTURE_CONDITION { MIXCOND_T, MIXCOND_P, MIXCOND_MAX };

enum FLASH_CONDITION { FLASHCOND_T, FLASHCOND_P, FLASHCOND_VF, FLASHCOND_H, FLASHCOND_MAX };

static const int XOPT_MAX_COMPONENTS = 128;

struct xOptSlate {
    const char* name;           // e.g. "StreamPort"
    const char* thermo_method;  // e.g. "SRK"
    const char* components[XOPT_MAX_COMPONENTS];
};

struct xOptThermoBlock {
    const char* block_name;                                                // 打印调试用
    const char* slate_name;                                                // 对应的slate名称
    const char* property_names[8];                                         // 物性名称
    const char* output_varname_properties[8];                              // 物性名称对应的变量名
    const char* output_varname_fugacity[XOPT_MAX_COMPONENTS];              // 对应的逸度系数变量名
    const char* output_varname_activity_coefficient[XOPT_MAX_COMPONENTS];  // 对应的活度系数变量名
    const char* varname_conditions[MIXCOND_MAX];                           // 对应的条件变量名
    const char* varname_components[XOPT_MAX_COMPONENTS];                   // 对应的组分变量名
};

// ***************************************************************

struct xOptThermo;

typedef struct xOptThermo* xOptThermoHandle;

typedef int (*xOptThermo_calcMixtureProperties)(
    xOptThermoHandle handle, double properties[8], double fugacity[XOPT_MAX_COMPONENTS],
    double activity_coefficient[XOPT_MAX_COMPONENTS],
    double g_properties[8][XOPT_MAX_COMPONENTS + MIXCOND_MAX],
    double g_fugacity[XOPT_MAX_COMPONENTS][XOPT_MAX_COMPONENTS + MIXCOND_MAX],
    double g_activity_coefficient[XOPT_MAX_COMPONENTS][XOPT_MAX_COMPONENTS + MIXCOND_MAX],
    const xOptSlate* slate, XOPT_PHASE phase, double conditions[MIXCOND_MAX],
    double components[XOPT_MAX_COMPONENTS]);

typedef int (*xOptThermo_calcFlash)(xOptThermoHandle handle, double& vapor_fraction,
                                    double lique_components[XOPT_MAX_COMPONENTS],
                                    double vapor_components[XOPT_MAX_COMPONENTS],
                                    const xOptSlate* slate, double conditions[FLASHCOND_MAX],
                                    double components[XOPT_MAX_COMPONENTS]);

// ***************************************************************

struct xOptPlatform;

typedef struct xOptPlatform* xOptPlatformHandle;

typedef xOptThermoHandle (*xOptPlatform_getThermo)(xOptPlatformHandle handle);

// ***************************************************************

struct xOptModel;

typedef struct xOptModel* xOptModelHandle;

XOPTIF_API xOptModelHandle xOptModel_createModel(xOptPlatformHandle platform, const char* name);

XOPTIF_API void xOptModel_freeModel(xOptModelHandle model);

// 设置组分列表：
// 组分列表的名字约定为
//["C1P", "C2P", "C3P", "C4P", "C5P", "C6P", "C7P", "C8P", "C9P", "C10P", "C6N", "C7N", "C8N",
//"C9N", "C10N", "C6A", "C7A", "C8A", "C9A", "C10A", "H2"]
XOPTIF_API int xOptModel_setComponents(xOptModelHandle model, const char* names[], int size);

// parameter指不在模型方程组中作为变量出现的参数，可能会影响方程结构
// 例如：板效率
XOPTIF_API int xOptModel_getParameters(xOptModelHandle model, const char* names[],
                                       double defualt_values[], int& size);

XOPTIF_API int xOptModel_setParameters(xOptModelHandle model, const char* name[], double value[],
                                       int size);

XOPTIF_API int xOptModel_setProblemType(xOptModelHandle model, XOPTF_PROBLEM_TYPE);

// 获取可以被fix的变量以及初值，如：回流比、板效率
// 根据用户界面配置是否被fix
XOPTIF_API int xOptModel_getFixableVariables(xOptModelHandle model, const char* names[],
                                             double initial_values[],
                                             int& size);

// 固定变量, 返回值参考XOPTF_ERRCODE
XOPTIF_API int xOptModel_fixVariables(xOptModelHandle model, const char* names[],
                                      const double values[], int size);

// 获取进料流股的个数，不考虑能量流股
XOPTIF_API int xOptModel_getInPortNum(xOptModelHandle model);

// 获取出料流股的个数，不考虑能量流股
XOPTIF_API int xOptModel_getOutPortNum(xOptModelHandle model);

// 获取进料流股对应的变量映射表
// streamNames可能为：T,P,fi_C1P,fi_C2P等等
// variableNames为对应变量在模型内部的名称
// 如果上游没有提供进料所对应的某个变量，例如P，则模型中该变量被固定在初始值
XOPTIF_API int xOptModel_getInPortVariableMap(xOptModelHandle model, int iPortIndex,
                                              const char* streamNames[],
                                 const char* variableNames[], int& size);

// 获取出料流股对应的变量映射表
XOPTIF_API int xOptModel_getOutPortVariableMap(xOptModelHandle model, int iPortIndex,
                                               const char* streamNames[],
                                  const char* variableNames[], int& size);

XOPTIF_API int xOptModel_validateModel(xOptModelHandle model);

// 构造对应的xOptProblem
XOPTIF_API xOptProblem* xOptModel_buildProblem(xOptModelHandle model);

XOPTIF_API void xOptModel_deleteProblem(xOptModelHandle model, xOptProblem* problem);

XOPTIF_API int xOptModel_getReportMetaAbstracts(xOptModelHandle model, const char* names[],
                                                const char* titles[],
                                   const char* descriptions[],
                                   const char* preferred_display_types[], int dim_size[],
                                   int& size);

XOPTIF_API int xOptModel_getReportMetaDims(xOptModelHandle model, const char* dim_names[],
                                           const char* dim_units[],
                                           const char* name,
                              int dim_size);

XOPTIF_API int xOptModel_getReportData(xOptModelHandle model, double data[], int shape[],
                                       const char* name, int& data_size,
                          int& shape_size);

// 查询测度/单位接口

// 热力学接口
XOPTIF_API int xOptModel_getNumberOfSlate(xOptModelHandle model);

XOPTIF_API int xOptModel_setSlate(xOptModelHandle model, int slate_index, const xOptSlate* slate);

XOPTIF_API int xOptModel_getNumberOfThermoBlock(xOptModelHandle model);

XOPTIF_API int xOptModel_getThermoBlocks(xOptModelHandle model, int count,
                                         xOptThermoBlock blocks[]);

#ifdef __cplusplus
}
#endif
