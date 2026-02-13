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

    template <typename T>
    BsonRef& append_array(const std::string& key, const std::vector<T>& items) {
        bson_t child_array;
        if (bson_append_array_begin(_b, key.c_str(), (int)key.length(), &child_array)) {
            BsonRef arrayRef(&child_array);
            char idx_str[16];
            size_t i = 0;
            for (const auto& item : items) {
                std::snprintf(idx_str, sizeof(idx_str), "%zu", i++);
                arrayRef.append(idx_str, item);
            }
            bson_append_array_end(_b, &child_array);
        }
        return *this;
    };

    BsonRef& append_document(const std::string& key, std::function<void(BsonRef&)> builderFunc);

    template <typename T>
    BsonRef& append_object_array(const std::string& key, const std::vector<T>& items,
                                 std::function<void(BsonRef&, const T&)> serializer) {
        bson_t child_array;
        if (bson_append_array_begin(_b, key.c_str(), (int)key.length(), &child_array)) {
            BsonRef arrayRef(&child_array);

            char idx_str[16];
            size_t i = 0;

            for (const auto& item : items) {
                std::snprintf(idx_str, sizeof(idx_str), "%zu", i++);
                arrayRef.append_document(idx_str, [&](BsonRef& itemDoc) {
                    serializer(itemDoc, item);
                });
            }
            bson_append_array_end(_b, &child_array);
        }
        return *this;
    }

    BsonRef& append_binary(const std::string& key, const void* data, size_t length);

    template <typename T>
    BsonRef& append_binary(const std::string& key, const std::vector<T>& values) {
        return append_binary(key, values.data(), values.size() * sizeof(T));
    }


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

    template<typename T>
    T as_type(const T& default_val = T()) const {
        if constexpr(std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t> ||
            std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t> ||
            std::is_same_v<T, int32_t> || std::is_same_v<T, uint32_t> ||
            std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>) {
            return (T)as_int64(default_val);
        } else if constexpr(std::is_same_v<T, double> || std::is_same_v<T, float>) {
            return (T)as_double(default_val);
        } else if constexpr (std::is_same_v<T, bool>) {
            return as_bool(default_val);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return as_string(default_val);
        } else {
            throw std::runtime_error("Unsupported type in as_type");
        }
    }

    template <typename T>
    std::vector<T> as_vector() const {
        std::vector<T> result;
        if (!_valid || !BSON_ITER_HOLDS_ARRAY(&_iter)) return result;

        bson_iter_t child;
        if (bson_iter_recurse(&_iter, &child)) {
            while (bson_iter_next(&child)) {
                BsonView v(child);

                if constexpr (std::is_same_v<T, int32_t>)
                    result.push_back(v.as_int32());
                else if constexpr (std::is_same_v<T, int64_t>)
                    result.push_back(v.as_int64());
                else if constexpr (std::is_same_v<T, double>)
                    result.push_back(v.as_double());
                else if constexpr (std::is_same_v<T, bool>)
                    result.push_back(v.as_bool());
                else if constexpr (std::is_same_v<T, std::string>)
                    result.push_back(v.as_string());
            }
        }
        return result;
    }

    template <typename T>
    std::vector<T> as_vector_from_binary() const {
        std::vector<T> result;
        if (!_valid || !BSON_ITER_HOLDS_BINARY(&_iter)) return result;

        bson_subtype_t subtype;
        uint32_t len;
        const uint8_t* data;

        bson_iter_binary(&_iter, &subtype, &len, &data);

        if (len % sizeof(T) == 0) {
            result.resize(len / sizeof(T));
            std::memcpy(result.data(), data, len);
        }
        return result;
    }

    template <typename T>
    std::vector<T> as_object_vector(std::function<T(const BsonView&)> deserializer) const {
        std::vector<T> result;
        if (!_valid || !BSON_ITER_HOLDS_ARRAY(&_iter)) return result;

        bson_iter_t child;
        if (bson_iter_recurse(&_iter, &child)) {
            while (bson_iter_next(&child)) {
                BsonView item(child);
                result.push_back(deserializer(item));
            }
        }
        return result;
    }

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

    static BsonDoc from_json(const std::string& json);

    static BsonDoc from_json(const char* json, size_t length);

    const zce_byte* get_data() const;

    uint32_t get_size() const;

  private:
    bool _owned;
};

}  // namespace zce