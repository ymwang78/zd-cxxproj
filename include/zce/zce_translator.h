#pragma once
// ***************************************************************
//  zce_translator   version:  1.0   -  date:  2025/06/15
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project ZCE.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include <zce/zce_singleton.h>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>  // 使用 nlohmann/json 单头文件库
#include <filesystem>
#include <zce/zce_convertor.h>

class zce_translator {
  public:
    bool load(const std::string& path) {
        std::filesystem::path pth(zce::CharacterConvertor::fromUtf8(path));
        std::ifstream file(pth);
        if (!file.is_open()) return false;

        nlohmann::json j;
        file >> j;

        translations_.clear();
        for (auto& [key, value] : j.items()) {
            translations_[key] = value;
        }
        return true;
    }

    std::string tr(const std::string& msgid) const {
        auto it = translations_.find(msgid);
        return it != translations_.end() ? it->second : msgid;
    }

  private:
    struct Impl;

    std::unordered_map<std::string, std::string> translations_;
};
