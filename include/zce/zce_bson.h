#pragma once
// ***************************************************************
//  zce_bson   version:  1.0   -  date: 2016/11/14
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#include <bson/bson.h>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <zce/zce_types.h>

namespace zce {

class BsonRef {
  public:
    explicit BsonRef(bson_t* b) : _b(b) {}

    BsonRef& append(const std::string& key, int32_t value);

    BsonRef& append(const std::string& key, int64_t value);

    BsonRef& append(const std::string& key, double value);

    BsonRef& append(const std::string& key, bool value);

    BsonRef& append(const std::string& key, const std::string& value);

    BsonRef& append(const std::string& key, const char* value);

    BsonRef& append_document(const std::string& key, std::function<void(BsonRef&)> builderFunc);

    BsonRef& append_array(const std::string& key, std::function<void(BsonRef&)> builderFunc);

    bson_t* raw() const { return _b; }

  protected:
    bson_t* _b;
};

class BsonView {
  public:
    explicit BsonView(const bson_t* b);

    explicit BsonView(const bson_iter_t& iter, bool valid = true);

    inline bool is_valid() const { return _valid; }
    explicit inline operator bool() const { return _valid; }

    BsonView operator[](const std::string& key) const;

    BsonView operator[](size_t index) const;

    int32_t as_int32(int32_t default_val = 0) const;

    int64_t as_int64(int64_t default_val = 0) const;

    double as_double(double default_val = 0.0) const;

    bool as_bool(bool default_val = false) const;

    std::string as_string(const std::string& default_val = "") const;

    std::string key() const;

    void for_each(std::function<void(const BsonView&)> callback) const;

  private:
    bson_iter_t _iter;
    bool _valid = false;
};

class BsonDoc : public BsonRef {
  public:
    BsonDoc();

    BsonDoc(const uint8_t* data, size_t length);

    BsonDoc(const BsonDoc&) = delete;

    BsonDoc& operator=(const BsonDoc&) = delete;

    BsonDoc(BsonDoc&& other) noexcept;

    ~BsonDoc();

    std::string to_json() const;

    const zce_byte* get_data() const;

    uint32_t get_size() const;

  private:
    bool _owned;
};

}  // namespace zce