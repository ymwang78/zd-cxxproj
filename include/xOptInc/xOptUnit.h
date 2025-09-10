#pragma once
// ***************************************************************
//  xOptUnit   version:  1.0   -  date:  2025/08/27
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project xOpt.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#include <zce/zce_object.h>
#include <zce/zce_singleton.h>
#include <unordered_map>

struct xOptMetric : public zce::Object {
    std::string name;       // 测度名称
    std::string base_unit;  // 基准单位, 其他单位以此为基准进行换算
};
typedef zce::SmartPtr<xOptMetric> xOptMetricPtr;

struct xOptUnit : public zce::Object {
    xOptMetricPtr metric;  // 所属测度
    std::string name;      // 单位名称
    double scale, offset;  // 换算系数, value_in_base_unit = scale * value_in_this_unit + offset
};
typedef zce::SmartPtr<xOptUnit> xOptUnitPtr;
typedef zce::SmartPtr<const xOptUnit> xOptUnitConstPtr;

struct xOptValue {
    double value;
    xOptUnitConstPtr unit;

    explicit xOptValue(double val = 0, const xOptUnitConstPtr& u = xOptUnitConstPtr(nullptr))
        : value(val), unit(u) {}

    std::optional<xOptValue> convertTo(const xOptUnitConstPtr& target_unit) const noexcept;
};

struct xOptUnitSystem : public zce::Object {
    std::string name;  // 单位体系名称
    std::unordered_map<std::string, xOptUnitPtr>
        metric_default_units;  // 单位列表,key是测度名,value是该测度使用哪个单位作为默认显示
};
typedef zce::SmartPtr<xOptUnitSystem> xOptUnitSystemPtr;

class xOptUnitAll : public zce::Object {
    std::unordered_map<std::string, xOptMetricPtr> metrics_;  // 测度列表,key是测度名称
    std::unordered_map<std::string, xOptUnitConstPtr> units_;  // 单位列表,key是单位名称
    std::unordered_map<std::string, xOptUnitSystemPtr>
        unit_systems_;  // 单位体系列表,key是单位体系名称, 例如SI
  public:
    int loadFromJson(const char* path);
    xOptUnitConstPtr getUnit(const std::string& unit_name) const;
    xOptMetricPtr getMetric(const std::string& metric_name) const;
    xOptUnitSystemPtr getUnitSystem(const std::string& system_name);
    std::optional<double> convertTo(double value, const std::string& from_unit_name,
                                  const std::string& to_unit_name) const noexcept;
};

typedef zce::Singleton<xOptUnitAll> xOptUnitSigt;