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

using xOptModelParameters = std::unordered_map<std::string, zce::Any>;

using xOptModelFixableVariables = std::unordered_map<std::string, double>;

using xOptVarCompMap = std::unordered_map<std::string, std::string>;

using xOptNameStrValueMap = std::unordered_map<std::string, std::string>;

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

    xOptStreamSlate(xOptStreamSlate&& rhs);

    ~xOptStreamSlate();

    const std::string& getName() const;

    const std::string& getThermoMethod() const;

    int addComponent(const std::string& comp_name, const std::string& var_name,
                     const std::string& description = "");

    int removeComponent(const std::string& comp_name);

    void clearComponents();

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
    //xOptModel* m_parent;
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

    virtual std::string getVersion() const { return "v1.0.0"; };

    int getVariableIndex(const std::string& varname, int hint_index = -1) const;

    // 初始化模型, 获取默认参数等，做好接受参数等准备
    virtual int initializeModel() = 0;

    virtual int setProblemType(XOPTF_PROBLEM_TYPE) = 0;

    virtual xOptModelParameters getParameters() const = 0;

    virtual xOptNameStrValueMap getParameterDescs() const { return {}; }

    virtual int setParameters(const xOptModelParameters& parameters) = 0;

    virtual int getPortNum(bool is_input_port) const = 0;

    virtual xOptVarCompMap getVarCompMap(bool is_input_port, int index) const = 0;

    // 这里不能用fixVariables的原因是因为setInPortVariableValues是必须的
    virtual xOptModelFixableVariables getInPortVariableValues() const;

    // 在generateEstimate的时候需要setInPortVariableValues, 因为此时没有完整变量表值
    virtual int setInPortVariableValues(const xOptModelFixableVariables& nvpairs);

    virtual const xOptModelFixableVariables& getDefaultFixableVariables() const;

    virtual xOptModelFixableVariables getFixableVariables() const = 0;

    virtual int validateModel() const = 0;

    // 以上是模型初始化准备，以下是运行时准备

    virtual int prepareRuntime(const xOptParsedVariableArr& arr) = 0;

    virtual const xOptParsedVariableArr& getVariables() const;

    // 生成估计，给出当前初值
    virtual int generateEstimate(std::vector<double>& initx, const xOptModelFixableVariables& fixed_values);

    // 以下API更新内置的变量当前值，供外部查看
    virtual int setVariableValues(const std::vector<double>& values);

    // hint index是帮助提示索引，不是必须的，不清楚的情况下填-1
    virtual xOptParsedVariable& getVariable(const std::string& varname, int hint_index);

    virtual int setVariableValue(const std::string& varname, int hint_index, double value,
                                 bool update_initx);

    virtual int setVariableBounds(const std::string& varname, int hint_index, double lower,
                                  double upper);

    virtual std::vector<int> getStreamVariableIndexes(const xOptStreamSlate& stream,
                                                      bool is_input_port, int index) const;

    virtual const zce::SmartPtr<xOptProblemBase>& getProblem() const;

    virtual std::vector<ReportMetaInfo> getReportMetas() const = 0;

    virtual ReportData getReportByMetaName(const std::string& name) const = 0;

    // 把当前值设置为更新值
    virtual int updateCurrentToInitialValue();

    // ***************************************热力学************************************************

    // 获取本模块支持的slate类型数量
    virtual int getNumberOfSlate() const { return -1; };

    // 获取每个端口使用的slate id
    virtual int getSlateIdOfPort(bool is_input_port, int port_index) const { return -1; };

    // slate_index是模块支持的slate类型索引，slate是具体的slate数据结构，返回值参考XOPTF_ERRCODE
    virtual int setSlate(int slate_index, const xOptSlate* slate) { return -1; };

    virtual int getNumberOfThermoBlock() { return 0; };

    virtual int getThermoBlocks(int count, xOptThermoBlock blocks[]) { return 0; };

    // ***************************************语言/国际化*******************************************

    /// 设置模型当前使用的语言（如 "zh"、"en"）。
    /// 模型可据此切换参数名称、报告标题等多语言文本的输出语言。
    /// 默认不操作，子类按需重写。
    virtual int setLanguage(const std::string& lang) { return 0; };
};
