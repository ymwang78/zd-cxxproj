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
#include <zce/zce_object.h>
#include <zce/zce_object_counter.h>

using xOptModelParameter = std::pair<std::string, double>;

using xOptModelParameters = std::unordered_map<std::string, double>;

using xOptModelFixableVariables = std::unordered_map<std::string, double>;

using xOptVarCompMap = std::unordered_map<std::string, std::string>;

struct xOptModelDescT {
    xOptModelParameters parameters;
    std::vector<std::string> fixable_varnames;
    std::unordered_map<std::string, xOptParsedVariable> variables;
    std::vector<xOptVarCompMap> inports_var_comp_map_vec;
    std::vector<xOptVarCompMap> outports_var_comp_map_vec;
};
extern xOptModelDescT loadFromJson(const char* path);

class xOptStreamSlate {
  public:
    explicit xOptStreamSlate(const std::string& type_name);

    ~xOptStreamSlate();

    const std::string& getName() const;

    void setName(const std::string& name);

    int addComponent(const std::string& comp_name, const std::string& description = "");

    int removeComponent(const std::string& comp_name);

    const std::vector<std::string>& getComponents() const;

    const std::vector<std::string>& getVariableNames() const;

    bool hasComponent(const std::string& comp_name) const;

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
    std::string name;         // 报告唯一标识
    std::string title;        // 报告标题
    std::string description;  // 报告描述
    std::string
        preferred_display_type;  // 首选显示类型: "line_chart", "table", "heatmap", "surface", etc.
    std::vector<std::string> dim_names;  // 维度名称
    std::vector<std::string> units;      // 每个维度的单位
};

struct ReportData {
    std::vector<double> flat_data;  // row-major 拍平
    std::vector<size_t> shape;      // 维度大小

    double at(std::initializer_list<size_t> indices) const {
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
struct xOptModelImplBase;

class xOptModelBase : public zce::Object {
  protected:
    std::string name_;
    zce::SmartPtr<xOptModelImplBase> pimpl_;

    xOptModelBase(const xOptModelBase&) = delete;
    xOptModelBase& operator=(const xOptModelBase&) = delete;

  public:
    template <typename T>
    zce::SmartPtr<T> impl() const {
        return zce::SmartPtr<T>::__dynamic_cast(pimpl_);
    }

    xOptModelBase();

    ~xOptModelBase();

    const std::string& getName() const { return name_; };

    void setName(const std::string& name) { name_ = name; };

    int getVariableIndex(const std::string& varname, int hint_index = -1) const;

    // 初始化模型, 获取默认参数等，做好接受参数等准备
    virtual int initializeModel() = 0;

    virtual int setProblemType(XOPTF_PROBLEM_TYPE) = 0;

    virtual int setComponents(const std::vector<std::string>& components) = 0;

    virtual xOptModelParameters getParameters() const = 0;

    virtual int setParameters(const xOptModelParameters& parameters) = 0;

    virtual int getPortNum(bool is_input_port) const = 0;

    virtual xOptVarCompMap getVarCompMap(bool is_input_port, int index) const = 0;

    // 这里不能用fixVariables的原因是因为setInPortVariableValues是必须的
    virtual xOptModelFixableVariables getInPortVariableValues() const;

    virtual int setInPortVariableValues(const xOptModelFixableVariables& nvpairs);

    virtual xOptModelFixableVariables getFixableVariables() const = 0;

    virtual int fixVariables(const xOptModelFixableVariables& varnames,
                             bool try_fixed_in_model_first) = 0;

    virtual int validateModel() const = 0;

    // 以上是模型初始化准备，以下是运行时准备

    virtual int prepareRuntime(const xOptParsedVariableArr& arr) = 0;

    virtual const xOptParsedVariableArr& getVariables() const;

    // 生成估计，给出当前初值
    virtual int generateEstimate(std::vector<double>& initx);

    // 以下API更新内置的变量当前值，供外部查看
    virtual int setVariableValues(const std::vector<double>& values);

    // hint index是帮助提示索引，不是必须的，不清楚的情况下填-1
    virtual xOptParsedVariable& getVariable(const std::string& varname, int hint_index);

    virtual int setVariableValue(const std::string& varname, int hint_index, double value,
                                 bool update_initx);

    // 这里是返回需要流程固定的变量的索引, 单元模块自己能固定的索引不需要返回
    virtual std::vector<int> getFlowsheetFixedVariableIndexes() const;

    virtual const xOptModelFixableVariables& getFlowsheetFixedVariables() const;

    virtual std::vector<int> getStreamVariableIndexes(const xOptStreamSlate& stream,
                                                      bool is_input_port, int index) const;

    virtual const zce::SmartPtr<xOptProblemBase>& getProblem() const;

    virtual std::vector<ReportMetaInfo> getReportMetas() const = 0;

    virtual ReportData getReportByMetaName(const std::string& name) const = 0;

    // 把当前值设置为更新值
    virtual int updateCurrentToInitialValue();

    // ***************************************热力学************************************************

    virtual int getNumberOfSlate() { return 0; };

    virtual int setSlates(int count, const xOptSlate* slate[]) { return 0; };

    virtual int getNumberOfThermoBlock() { return 0; };

    virtual int getThermoBlocks(int count, xOptThermoBlock blocks[]) { return 0; };
};
