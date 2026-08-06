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

typedef struct _xOptThermo {
    int dummy;
}*xOptThermoHandle;

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

typedef struct xOptThermoT {
    size_t size;
    xOptThermoHandle handle;
    xOptThermo_calcMixtureProperties calcMixtureProperties;
    xOptThermo_calcFlash calcFlash;
} xOptThermoT;

// ***************************************************************
typedef struct _xOptPlatform {
    int dummy;
}* xOptPlatformHandle;

typedef xOptThermoT* (*xOptPlatform_createThermo)(xOptPlatformHandle handle, const char* args);

typedef void (*xOptThermo_freeThermo)(xOptThermoT* thermo);

typedef struct xOptPlatformT {
    size_t size;
    xOptPlatformHandle handle;
    xOptPlatform_createThermo createThermo;
    xOptThermo_freeThermo freeThermo;
} xOptPlatformT;

// ***************************************************************

typedef struct _xOptProblem {
    int dummy;
}* xOptProblemHandle;

typedef int* (*xOptProblem_destroyProblem)(xOptProblemHandle handle);
/* configurations */

typedef int (*xOptProblem_numVariables)(xOptProblemHandle handle);

typedef int (*xOptProblem_numConstraints)(xOptProblemHandle handle);

typedef int (*xOptProblem_getVariableNames)(xOptProblemHandle handle, const char* names[],
                                            int names_size);

typedef int (*xOptProblem_getVariableDescriptions)(xOptProblemHandle handle,
                                                   const char* descriptions[],
                                                   int descriptions_size);

typedef int (*xOptProblem_getConstraintNames)(xOptProblemHandle handle, const char* names[],
                                              int names_size);

typedef int (*xOptProblem_getOptions)(xOptProblemHandle handle, double* options, int options_size);

typedef int (*xOptProblem_getVariableBounds)(xOptProblemHandle handle, double* xlow, double* xupp,
                                             int x_size);

typedef int (*xOptProblem_getConstraintBounds)(xOptProblemHandle handle, double* clow, double* cupp,
                                               int c_size);

typedef int (*xOptProblem_getInitialX)(xOptProblemHandle handle, double* x0, int x0_size);

typedef int (*xOptProblem_getLinearConstraints)(xOptProblemHandle handle, int* lcons_rowidx,
                                                int* lcons_colidx, double* values, int* lcons_size);

typedef int (*xOptProblem_getObjectiveGradientStructure)(xOptProblemHandle handle, int* obj_colidx,
                                                         int* obj_colidx_size);

typedef int (*xOptProblem_getConstraintJacobianStructure)(xOptProblemHandle handle,
                                                          int* cons_rowidx, int* cons_colidx,
                                                          int* nnz);

/* iterator */

typedef int (*xOptProblem_setX)(xOptProblemHandle handle, const double* x, int x_size);

typedef int (*xOptProblem_runTimeCheck)(xOptProblemHandle handle);

typedef int (*xOptProblem_evaluateObjective)(xOptProblemHandle handle, double* obj);

typedef int (*xOptProblem_evaluateConstraints)(xOptProblemHandle handle, double* cons,
                                               int cons_size);

typedef int (*xOptProblem_evaluateObjectiveGradient)(xOptProblemHandle handle, double* grad,
                                                     int grad_size);

typedef int (*xOptProblem_evaluateConstraintsJacobianValues)(xOptProblemHandle handle,
                                                             double* values, int values_size);

typedef struct xOptProblemT {
    size_t size; 
    xOptProblemHandle handle;
    xOptProblem_destroyProblem destroyProblem;
    xOptProblem_numVariables numVariables;
    xOptProblem_numConstraints numConstraints;
    xOptProblem_getVariableNames getVariableNames;
    xOptProblem_getVariableDescriptions getVariableDescriptions;
    xOptProblem_getConstraintNames getConstraintNames;
    xOptProblem_getOptions getOptions;
    xOptProblem_getVariableBounds getVariableBounds;
    xOptProblem_getConstraintBounds getConstraintBounds;
    xOptProblem_getInitialX getInitialX;
    xOptProblem_getLinearConstraints getLinearConstraints;
    xOptProblem_getObjectiveGradientStructure getObjectiveGradientStructure;
    xOptProblem_getConstraintJacobianStructure getConstraintJacobianStructure;
    xOptProblem_setX setX;
    xOptProblem_runTimeCheck runTimeCheck;
    xOptProblem_evaluateObjective evaluateObjective;
    xOptProblem_evaluateConstraints evaluateConstraints;
    xOptProblem_evaluateObjectiveGradient evaluateObjectiveGradient;
    xOptProblem_evaluateConstraintsJacobianValues evaluateConstraintsJacobianValues;
} xOptProblemT;

// ***************************************************************
// 返回码约定
// ------------------------------------------------------------------
// 本接口中所有返回 int 状态的函数遵循同一约定：
//
//     < 0   失败。调用方应当放弃本次操作。
//     = 0   正常。
//     > 0   正常，且带有含义。
//
// 也就是说，**判定失败必须写 `ret < 0`，不能写 `ret != 0`**。大于零表示操作
// 已经成功完成，同时有一些调用方可能关心的情况需要一并告知——例如模型接受了
// 一份它认为不合规、但调用方明确要求接受的配置。这类结果与普通成功在数值上
// 无法区分，正是它需要被单独报出来的原因。
//
// 正值的具体含义由各函数自行定义，通常为按位组合，以便新增一位而不改变已有
// 位对既有宿主的含义。
//
// 注意：本文件中另有一批函数用返回值表示**数量**（getVariableNames、
// getVariableBounds、evaluateConstraints 等），它们的正值是长度而不是状态。
// 上述约定只适用于原本仅返回 0 / -1 的那些函数。
//
// 这条约定此前是事实上成立而未写明的：xOptModelBlackBox 各处判定用的都是
// `< 0`。写下来是为了让它成为可依赖的契约，而不是下一个实现者需要自行考证的
// 巧合。
// ***************************************************************

// ***************************************************************
typedef struct _xOptModel {
    int dummy;
}* xOptModelHandle;

typedef void (*xOptModel_destroyModel)(xOptModelHandle model);

typedef const char* (*xOptModel_getVersion)(xOptModelHandle model);

typedef int (*xOptModel_setLanguage)(xOptModelHandle model, const char* language_code);

// slates
typedef int (*xOptModel_getNumberOfSlate)(xOptModelHandle model);

typedef int (*xOptModel_getSlateIdOfPort)(xOptModelHandle model, bool is_input_port,
                                          int port_index);

typedef int (*xOptModel_setSlate)(xOptModelHandle model, int slate_index, const xOptSlate* slate);

// parameter指不在模型方程组中作为变量出现的参数，可能会影响方程结构
// 例如：板效率
typedef int (*xOptModel_getParameters)(xOptModelHandle model, const char* names[],
                                       double defualt_values[], int& size);

typedef int (*xOptModel_setParameters)(xOptModelHandle model, const char* name[], double value[],
                                       int size);

// 字符串参数通道。数值通道的取值只能是 double，无法表达取值本身是名字的参数——
// 例如 RadFrac 的 SpecifyVariableName（["BottomFlowrate", "RefluxFlowrate", ...]），
// 它与数值通道里的 SpecifyVariableValue 一一对应。
// 一个名字对应一个字符串列表；values 为 NULL 时只回填 size，与数值通道的约定一致。
// 返回的 const char* 由模型持有，在下一次 setSlate/setParameters/销毁之前有效。
// 三者均为可选接口，未实现时为 NULL，宿主必须先判空再调用。
typedef int (*xOptModel_getStringParameterNames)(xOptModelHandle model, const char* names[],
                                                 int& size);

typedef int (*xOptModel_getStringParameter)(xOptModelHandle model, const char* name,
                                            const char* values[], int& size);

typedef int (*xOptModel_setStringParameter)(xOptModelHandle model, const char* name,
                                            const char* values[], int size);

typedef int (*xOptModel_setProblemType)(xOptModelHandle model, XOPTF_PROBLEM_TYPE);

// 获取可以被fix的变量以及初值，如：回流比、板效率
// 根据用户界面配置是否被fix
typedef int (*xOptModel_getFixableVariables)(xOptModelHandle model, const char* names[],
                                             double initial_values[], int& size);

// 获取进料流股的个数，不考虑能量流股
typedef int (*xOptModel_getInPortNum)(xOptModelHandle model);

// 获取出料流股的个数，不考虑能量流股
typedef int (*xOptModel_getOutPortNum)(xOptModelHandle model);

// 获取进料流股对应的变量映射表
// streamNames可能为：T,P,fi_C1P,fi_C2P等等
// variableNames为对应变量在模型内部的名称
// 如果上游没有提供进料所对应的某个变量，例如P，则模型中该变量被固定在初始值
typedef int (*xOptModel_getInPortVariableMap)(xOptModelHandle model, int iPortIndex,
                                              const char* streamNames[],
                                              const char* variableNames[], int& size);

// 获取出料流股对应的变量映射表
typedef int (*xOptModel_getOutPortVariableMap)(xOptModelHandle model, int iPortIndex,
                                               const char* streamNames[],
                                               const char* variableNames[], int& size);

typedef int (*xOptModel_validateModel)(xOptModelHandle model);

// 构造对应的xOptProblem
typedef int (*xOptModel_buildProblem)(xOptModelHandle model, xOptProblemT* problem);

typedef int (*xOptModel_generateEstimate)(xOptModelHandle model, double initx[], int& size,
                                          const char fixed_var_names[],
                                          const double fixed_var_values[], int fixed_var_size);

typedef int (*xOptModel_getReportMetaAbstracts)(xOptModelHandle model, const char* names[],
                                                const char* titles[], const char* descriptions[],
                                                const char* preferred_display_types[],
                                                int dim_size[], int& size);

typedef int (*xOptModel_getReportMetaDims)(xOptModelHandle model, const char* dim_names[],
                                           const char* dim_units[], const char* name, int dim_size);

typedef int (*xOptModel_getReportData)(xOptModelHandle model, double data[], int shape[],
                                       const char* name, int& data_size, int& shape_size);

// 热力学接口
typedef int (*xOptModel_getNumberOfThermoBlock)(xOptModelHandle model);

typedef int (*xOptModel_getThermoBlocks)(xOptModelHandle model, int count,
                                         xOptThermoBlock blocks[]);

struct xOptModelT {
    size_t size;
    xOptModelHandle handle;
    xOptModel_destroyModel destroyModel;
    xOptModel_setLanguage setLanguage;
    xOptModel_getNumberOfSlate getNumberOfSlate;
    xOptModel_getSlateIdOfPort getSlateIdOfPort;
    xOptModel_setSlate setSlate;
    xOptModel_getParameters getParameters;
    xOptModel_setParameters setParameters;
    xOptModel_setProblemType setProblemType;
    xOptModel_getFixableVariables getFixableVariables;
    xOptModel_getInPortNum getInPortNum;
    xOptModel_getOutPortNum getOutPortNum;
    xOptModel_getInPortVariableMap getInPortVariableMap;
    xOptModel_getOutPortVariableMap getOutPortVariableMap;
    xOptModel_validateModel validateModel;
    xOptModel_buildProblem buildProblem;
    xOptModel_generateEstimate generateEstimate;
    xOptModel_getReportMetaAbstracts getReportMetaAbstracts;
    xOptModel_getReportMetaDims getReportMetaDims;
    xOptModel_getReportData getReportData;
    xOptModel_getNumberOfThermoBlock getNumberOfThermoBlock;
    xOptModel_getThermoBlocks getThermoBlocks;
    // Optional. If null or empty, host code treats the model as version "v1.0.0".
    xOptModel_getVersion getVersion;
    // ---- 尾部扩展 ------------------------------------------------------
    // 新接口只能追加在这里，且必须是可选的（允许为 NULL）。宿主分配本结构并在
    // size 里填 sizeof(xOptModelT)；模型据此判断调用方的副本是否长到覆盖某个
    // 字段，只有覆盖到了才写入，否则会越过调用方的分配写坏内存。相应地，模型
    // 不应要求 size 恰好等于自己的 sizeof——那样一来任何用旧头文件编译的宿主
    // 都会被新模型拒绝，而追加本可以是兼容的。
    xOptModel_getStringParameterNames getStringParameterNames;
    xOptModel_getStringParameter getStringParameter;
    xOptModel_setStringParameter setStringParameter;
};

/* xOptModelT model= {sizeof(xOptModelT)} */
XOPTIF_API int xOptModel_createModel(xOptModelT* model, xOptPlatformT* platform, const char* name);

#ifdef __cplusplus
}
#endif
