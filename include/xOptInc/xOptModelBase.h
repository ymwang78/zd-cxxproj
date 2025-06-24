#pragma once
// ***************************************************************
//  xOptModelBase   version:  1.0   -  date:  2025/04/02
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xRto.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
// 由于内部还是有大量的方法，所以不能依赖第三方模型接口xOptModel，必须用自己的
// ***************************************************************
#include "xOptInc/xOpt.h"
#include "xOpt/xOptModel.h"
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

using xOptModelParameter = std::pair<std::string, double> ;

using xOptModelParameters = std::unordered_map<std::string, double>;

using xOptModelFixableVariables = std::unordered_map<std::string, double>;

using xOptVarCompMap = std::unordered_map<std::string, std::string> ;

struct xOptModelDescT {
    xOptModelParameters parameters;
    std::vector<std::string> fixable_varnames;
    std::vector<xOptVarCompMap> inports_var_comp_map_vec;
    std::vector<xOptVarCompMap> outports_var_comp_map_vec;
};
extern xOptModelDescT loadFromJson(const char* path);

class xOptStreamType {
  public:
    struct VariableTemplate {
        std::string name;          // 变量名 (如 "temperature", "mole_frac")
        std::string comp_binding;  // 绑定的组分("*"=所有, ""=全局, "H2O"=特定组分)
        std::string unit;          // 单位 ("K", "Pa", "kg/s")
        double lower = -1e20;
        double upper = 1e20;
        double initial = 1.0;
    };

    explicit xOptStreamType(const std::string& type_name);

    const std::string& getName() const;

    void setName(const std::string& name);

    int addComponent(const std::string& comp_name, const std::string& description = "");

    int removeComponent(const std::string& comp_name);

    const std::vector<std::string>& getComponents() const;

    const std::vector<std::string>& getVariableNames() const;

    bool hasComponent(const std::string& comp_name) const;

    int addVariableTemplate(const VariableTemplate& var_def);

    int removeVariableTemplate(const std::string& var_name);

    const std::vector<VariableTemplate>& getVariableTemplates() const;

    // 展开所有实际变量名（{portname}.temperature, {portname}.{component}.{variablen}）
    std::vector<std::string> expandVariables(const std::string& port_name) const;

    bool validateVariable(const std::string& full_var_name) const;

  private:
    struct Impl;
    struct Impl* m_pimpl;
};

struct xOptPort;
class xOptModelBase;

struct xOptStream {
    enum StreamType : unsigned char { Material, Energy, Information };

    std::string m_name;
    StreamType m_stream_type;

    std::vector<std::string> resolved_variables;  // buildProblem() 之后展开为实际变量名

    struct xOptPort* m_output_port;
    struct xOptPort* m_input_port;
};

struct ReportMetaInfo {
    std::string name; // 报告唯一标识
    std::string title; // 报告标题
    std::string description; // 报告描述
    std::string preferred_display_type; // 首选显示类型: "line_chart", "table", "heatmap", "surface", etc.
    std::vector<std::string> dim_names; // 维度名称
    std::vector<std::string> units; // 每个维度的单位
    std::vector<size_t> dim_sizes; // 维度大小   例如 [20, 5]
};

struct ReportData {
    std::vector<double> flat_data; // row-major 拍平

    double at(const std::vector<size_t>& shape, std::initializer_list<size_t> indices) const {
        // 计算偏移量，按 row-major 展开规则
        size_t offset = 0;
        size_t stride = 1;
        for (auto it = indices.end(); it != indices.begin();) {
            --it;
            offset += (*it) * stride;
            stride *= shape[it - indices.begin()];
        }
        return flat_data[offset];
    }
};

struct xOptPort {
    enum PortType : unsigned char { InputPort, OutputPort };

    xOptModel* m_parent;
    std::string m_name;
    PortType m_port_type;

    xOptStream* m_stream;
};

class xOptProblem;

class xOptModelBase {
  protected:
    std::string name_;
    std::vector<double> X_;

  public:

    const std::string& getName() const { return name_; };

    void setName(const std::string& name) { name_ = name; };

    std::vector<double>& getX() { return X_; };

    virtual ~xOptModelBase() = default;

    // 初始化模型, 获取默认参数等，做好接受参数等准备
    virtual int initializeModel() = 0;

    virtual int setProblemType(XOPTF_PROBLEM_TYPE) = 0;

    virtual int setComponents(const std::vector<std::string>& components) = 0;

    virtual xOptModelParameters getParameters() const = 0;

    virtual int setParameters(const xOptModelParameters& parameters) = 0;

    virtual xOptModelFixableVariables getFixableVariables() const = 0;

    virtual int fixVariables(const xOptModelFixableVariables& varnames) = 0;

    virtual int validateModel() const= 0;

    // 以上是模型初始化准备，以下是运行时准备
    virtual int prepareRuntime() = 0;

    virtual xOptParsedVariableArr getVariables() const;

    virtual int setVariableValue(const std::string& varname, double value) { return 0; };

    virtual int getPortNum(bool is_input_port) const = 0;

    virtual xOptVarCompMap getVarCompMap(bool is_input_port, int index) const = 0;

    virtual std::vector<int> getStreamVariableIndexes(const xOptStreamType& stream,
                                                         bool is_input_port, int index) const;

    virtual std::vector<int> getFixableVariableIndexes(const xOptModelFixableVariables& vars) const;

    virtual xOptProblem* getProblem() const = 0;

    virtual std::vector<ReportMetaInfo> getReportMetas() const = 0;

    virtual ReportData getReportByMetaName(const std::string& name) const = 0;
};
