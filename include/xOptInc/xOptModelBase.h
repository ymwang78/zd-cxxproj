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

typedef std::pair<std::string, std::string> xOptModelParameter;

typedef std::unordered_map<std::string, std::string> xOptVarCompMap;

struct xOptModelDescT {
    std::vector<xOptModelParameter> parameters;
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

struct xOptPort {
    enum PortType : unsigned char { InputPort, OutputPort };

    xOptModel* m_parent;
    std::string m_name;
    PortType m_port_type;

    xOptStream* m_stream;
};

class xOptProblem;

class xOptModelBase {
    std::string name_;
    std::vector<double> X_;

  public:

    const std::string& getName() const { return name_; };

    void setName(const std::string& name) { name_ = name; };

    std::vector<double>& getX() { return X_; };

    virtual ~xOptModelBase() = default;

    virtual int initialize() { return 0; };

    virtual int setProblemType(XOPTF_PROBLEM_TYPE) = 0;

    virtual std::vector<xOptModelParameter> getParameters() const = 0;

    virtual int setParameter(const xOptModelParameter& parameter) = 0;

    virtual int setComponents(const std::vector<std::string>& components) = 0;

    virtual int validateModel() const = 0;

    virtual std::vector<std::string> getFixableVariables() const = 0;

    virtual int fixVariables(const std::vector<std::string>& varnames) = 0;

    virtual int unFixVariables(const std::vector<std::string>& varnames) = 0;

    virtual std::vector<int> getFixedVariableIndexes() = 0;

    virtual xOptParsedVariableArr getVariables() const = 0;

    virtual int setVariableValue(const std::string& varname, double value) { return 0; };

    virtual int getPortNum(bool is_input_port) const = 0;

    virtual xOptVarCompMap getVarCompMap(bool is_input_port, int index) const = 0;

    virtual std::vector<int> getCompVariableIndexes(const xOptStreamType& stream,
                                                         bool is_input_port, int index) const;

    virtual xOptProblem* buildProblem() = 0;

    virtual xOptProblem* getProblem() const { return nullptr; }
};
