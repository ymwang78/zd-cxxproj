#pragma once

#include <zce/zce_task.h>
#include <zce/zce_bytes.h>
#include <functional>
#include <nlohmann/json.hpp>

// 使用 nlohmann::json 作为 BSON 文档类型
using bson_document = nlohmann::json;
using bson_array = nlohmann::json;
using bson_value = nlohmann::json;

// BSON 文档视图类型
class bson_document_view {
  private:
    const nlohmann::json& doc_;

  public:
    bson_document_view(const nlohmann::json& doc) : doc_(doc) {}

    // 查找字段
    nlohmann::json::const_iterator find(const std::string& field) const { return doc_.find(field); }

    // 检查是否到达末尾
    bool end() const { return doc_.is_null(); }

    // 获取原始 JSON 对象
    const nlohmann::json& get_json() const { return doc_; }

    // 获取字段值
    const nlohmann::json& operator[](const std::string& field) const { return doc_[field]; }

    // 检查字段是否存在
    bool contains(const std::string& field) const { return doc_.contains(field); }

    // 获取字段类型
    nlohmann::json::value_t type() const { return doc_.type(); }

    // 获取数组
    const nlohmann::json& get_array() const { return doc_; }

    // 获取文档
    const nlohmann::json& get_document() const { return doc_; }

    // 获取字符串值
    std::string get_string() const { return doc_.get<std::string>(); }

    // 获取整数32值
    int32_t get_int32() const { return doc_.get<int32_t>(); }

    // 获取整数64值
    int64_t get_int64() const { return doc_.get<int64_t>(); }

    // 获取双精度值
    double get_double() const { return doc_.get<double>(); }

    // 获取布尔值
    bool get_bool() const { return doc_.get<bool>(); }

    // 获取原始数据
    const uint8_t* raw() const {
        // 这里需要序列化为 BSON 格式
        static std::vector<uint8_t> buffer;
        buffer.clear();
        // TODO: 实现 JSON 到 BSON 的转换
        return buffer.data();
    }

    // 获取长度
    size_t length() const { return doc_.dump().size(); }
};

// BSON 文档值类型
class bson_document_value {
  private:
    nlohmann::json doc_;

  public:
    bson_document_value() = default;
    bson_document_value(const nlohmann::json& doc) : doc_(doc) {}

    // 获取视图
    bson_document_view view() const { return bson_document_view(doc_); }

    // 获取 JSON 对象
    const nlohmann::json& get_json() const { return doc_; }

    // 设置 JSON 对象
    void set_json(const nlohmann::json& doc) { doc_ = doc; }
};

// BSON 数组值类型
class bson_array_value {
  private:
    nlohmann::json array_;

  public:
    bson_array_value() = default;
    bson_array_value(const nlohmann::json& arr) : array_(arr) {}

    // 获取视图
    bson_document_view view() const { return bson_document_view(array_); }

    // 获取 JSON 数组
    const nlohmann::json& get_json() const { return array_; }

    // 设置 JSON 数组
    void set_json(const nlohmann::json& arr) { array_ = arr; }
};

// BSON 构建器命名空间
namespace bson_builder {
namespace basic {
// 键值对结构 - 必须在其他类之前定义
struct kvp {
    std::string key;
    nlohmann::json value;

    kvp(const std::string& k, const nlohmann::json& v) : key(k), value(v) {}
};

// 创建键值对
inline kvp make_kvp(const std::string& key, const nlohmann::json& value) { return kvp(key, value); }

// 子文档构建器
class sub_document {
  private:
    nlohmann::json& parent_;
    std::string key_;

  public:
    sub_document(nlohmann::json& parent, const std::string& key) : parent_(parent), key_(key) {}

    // 添加键值对
    void append(const std::string& key, const nlohmann::json& value) { parent_[key_] = value; }

    // 添加键值对（使用 kvp 结构）
    void append(const kvp& pair) { parent_[pair.key] = pair.value; }

    // 添加嵌套文档
    void append(const std::string& key, std::function<void(sub_document)> func) {
        nlohmann::json subdoc;
        sub_document sub(subdoc, key);
        func(sub);
        parent_[key] = subdoc;
    }
};

// 子数组构建器
class sub_array {
  private:
    nlohmann::json& parent_;

  public:
    sub_array(nlohmann::json& parent) : parent_(parent) {}

    // 添加元素
    void append(const nlohmann::json& value) { parent_.push_back(value); }

    // 添加嵌套文档
    void append(std::function<void(sub_document)> func) {
        nlohmann::json subdoc;
        sub_document sub(subdoc, "");
        func(sub);
        parent_.push_back(subdoc);
    }
};

// 文档构建器
class document {
  private:
    nlohmann::json doc_;

  public:
    document() = default;

    // 添加键值对
    void append(const std::string& key, const nlohmann::json& value) { doc_[key] = value; }

    // 添加键值对（使用 kvp 结构）
    void append(const kvp& pair) { doc_[pair.key] = pair.value; }

    // 添加嵌套文档
    void append(const std::string& key, std::function<void(sub_document)> func) {
        nlohmann::json subdoc;
        sub_document sub(subdoc, key);
        func(sub);
        doc_[key] = subdoc;
    }

    // 获取文档值
    bson_document_value extract() { return bson_document_value(doc_); }

    // 获取 JSON 对象
    const nlohmann::json& get_json() const { return doc_; }
};

// 数组构建器
class array {
  private:
    nlohmann::json array_;

  public:
    array() = default;

    // 添加元素
    void append(const nlohmann::json& value) { array_.push_back(value); }

    // 添加嵌套文档
    void append(std::function<void(sub_document)> func) {
        nlohmann::json subdoc;
        sub_document sub(subdoc, "");
        func(sub);
        array_.push_back(subdoc);
    }

    // 获取数组值
    bson_array_value extract() { return bson_array_value(array_); }

    // 获取 JSON 数组
    const nlohmann::json& get_json() const { return array_; }
};
}  // namespace basic

namespace stream {
// 标记类型 - 必须在 document 类之前定义
struct open_document {};
struct close_document {};
struct open_array {};
struct close_array {};
struct finalize {};

// 流式文档构建器
class document {
  private:
    nlohmann::json doc_;
    std::string current_key_;

  public:
    document() = default;

    // 流式操作符
    document& operator<<(const std::string& key) {
        current_key_ = key;
        return *this;
    }

    document& operator<<(const nlohmann::json& value) {
        if (!current_key_.empty()) {
            doc_[current_key_] = value;
            current_key_.clear();
        }
        return *this;
    }

    document& operator<<(int64_t value) {
        if (!current_key_.empty()) {
            doc_[current_key_] = value;
            current_key_.clear();
        }
        return *this;
    }

    document& operator<<(int32_t value) {
        if (!current_key_.empty()) {
            doc_[current_key_] = value;
            current_key_.clear();
        }
        return *this;
    }

    document& operator<<(double value) {
        if (!current_key_.empty()) {
            doc_[current_key_] = value;
            current_key_.clear();
        }
        return *this;
    }

    document& operator<<(bool value) {
        if (!current_key_.empty()) {
            doc_[current_key_] = value;
            current_key_.clear();
        }
        return *this;
    }

    // 打开文档
    document& operator<<(const open_document&) {
        // 处理嵌套文档
        return *this;
    }

    // 关闭文档
    document& operator<<(const close_document&) {
        // 处理文档关闭
        return *this;
    }

    // 打开数组
    document& operator<<(const open_array&) {
        // 处理数组开始
        return *this;
    }

    // 关闭数组
    document& operator<<(const close_array&) {
        // 处理数组结束
        return *this;
    }

    // 完成构建
    bson_document_value operator<<(const finalize&) { return bson_document_value(doc_); }

    // 获取 JSON 对象
    const nlohmann::json& get_json() const { return doc_; }
};

// 流式数组构建器
class array {
  private:
    nlohmann::json array_;

  public:
    array() = default;

    // 流式操作符
    array& operator<<(const nlohmann::json& value) {
        array_.push_back(value);
        return *this;
    }

    array& operator<<(int64_t value) {
        array_.push_back(value);
        return *this;
    }

    array& operator<<(int32_t value) {
        array_.push_back(value);
        return *this;
    }

    array& operator<<(double value) {
        array_.push_back(value);
        return *this;
    }

    array& operator<<(bool value) {
        array_.push_back(value);
        return *this;
    }

    // 完成构建
    bson_array_value operator<<(const finalize&) { return bson_array_value(array_); }

    // 获取 JSON 数组
    const nlohmann::json& get_json() const { return array_; }
};
}  // namespace stream
}  // namespace bson_builder
// 为兼容性添加类型别名
namespace bsoncxx {
using document = bson_builder::basic::document;
using array = bson_builder::basic::array;
using document_view = bson_document_view;
using document_value = bson_document_value;
using array_value = bson_array_value;

namespace builder {
namespace basic {
using document = bson_builder::basic::document;
using array = bson_builder::basic::array;
using sub_document = bson_builder::basic::sub_document;
using sub_array = bson_builder::basic::sub_array;
using kvp = bson_builder::basic::kvp;
}  // namespace basic
namespace stream {
using document = bson_builder::stream::document;
using array = bson_builder::stream::array;
}  // namespace stream
}  // namespace builder
}  // namespace bsoncxx

namespace zce {
class RefBlock;
};

zce_int64 ZCE_API zdb_mgo_getintfield(const bson_document_view& p, const std::string& field,
                                      zce_int64 defaultval);

#define zdb_mgo_getfield_x(x)                                                          \
    inline x zdb_mgo_getfield(const bson_document_view& doc, const std::string& field, \
                              x defaultval = 0) {                                      \
        return (x)zdb_mgo_getintfield(doc, field, defaultval);                         \
    }

zdb_mgo_getfield_x(zce_char);
zdb_mgo_getfield_x(zce_byte);
zdb_mgo_getfield_x(zce_int16);
zdb_mgo_getfield_x(zce_uint16);
zdb_mgo_getfield_x(zce_int32);
zdb_mgo_getfield_x(zce_uint32);
zdb_mgo_getfield_x(zce_int64);
zdb_mgo_getfield_x(zce_uint64);

std::string ZCE_API zdb_mgo_getfield(const bson_document_view& p, const std::string& field,
                                     const char* defaultval);

std::string ZCE_API zdb_mgo_getfield(const bson_document_view& p, const std::string& field,
                                     const std::string& defaultval);

bool ZCE_API zdb_mgo_getfield(const bson_document_view& p, const std::string& field,
                              bool def = false);

int ZCE_API zdb_mgo_getfield(const bson_document_view& p, const std::string& field,
                             zce::RefBlock& dblock);

int ZCE_API zdb_mgo_getfield(const bson_document_view& p, const std::string& field,
                             bson_document_view& subv);

#ifndef WIN32
void ZCE_API zdb_mgo_getarray(std::vector<long long>& v, const bson_document_view& doc,
                              const std::string& field);
#endif

void ZCE_API zdb_mgo_getarray(std::vector<zce_int64>& v, const bson_document_view& doc,
                              const std::string& field);

void ZCE_API zdb_mgo_getarray(std::vector<int>& v, const bson_document_view& p,
                              const std::string& field);

void ZCE_API zdb_mgo_getarray(std::vector<unsigned>& v, const bson_document_view& p,
                              const std::string& field);

void ZCE_API zdb_mgo_getarray(std::vector<short>& v, const bson_document_view& p,
                              const std::string& field);

void ZCE_API zdb_mgo_getarray(std::vector<zce_byte>& v, const bson_document_view& doc,
                              const std::string& field);

void ZCE_API zdb_mgo_getarray(std::vector<std::string>& v, const bson_document_view& doc,
                              const std::string& field);

void ZCE_API zdb_mgo_getarray(std::vector<zce_bytevec>& v, const bson_document_view& doc,
                              const std::string& field);

void ZCE_API zdb_mgo_append_subarray(bsoncxx::builder::basic::sub_array& v,
                                     const std::vector<zce_byte>& arr);

int ZCE_API zdb_mgo_getobjarray(const bson_document_view& doc, const std::string& field,
                                std::function<int(const bson_document_view& subv)> f);

class ZCE_API zdb_mgo_obj {
    const bson_document_view& doc_;

  public:
    zdb_mgo_obj(const bson_document_view&);

    zce_int64 get(const std::string& f, zce_int64 def = 0);

    std::string get(const std::string& f, const std::string& def = "");

    int get_dblock(const std::string& f, zce::RefBlock& dblock);

    bool get_bool(const std::string& f, bool def = false);
};

template <typename T>
bsoncxx::array_value zdb_mgo_toarray(const std::vector<T>& arr) {
    bsoncxx::builder::stream::array v{};
    for (auto iter = arr.begin(); iter != arr.end(); ++iter) {
        v << (int64_t)*iter;
    }
    return v << bsoncxx::builder::stream::finalize;
};

namespace zdp {
typedef typename bsoncxx::builder::basic::document bson_doc;
typedef typename bsoncxx::builder::basic::sub_document bson_builder;
typedef typename bson_document_view bson_view;

template <typename T>
struct response_json_t {
    int errcode;
    std::string errdesc;
    T data;
};

template <typename T>
int zbson_pack(bsoncxx::builder::basic::sub_document& _obj_builder, const response_json_t<T>& _t) {
    int ret = 0;

    _obj_builder.append(bsoncxx::builder::basic::kvp("errcode", _t.errcode));
    _obj_builder.append(bsoncxx::builder::basic::kvp("errdesc", _t.errdesc));

    if (_t.errcode >= 0) {
        _obj_builder.append(bsoncxx::builder::basic::kvp(
            "data", [&_t, &ret](bsoncxx::builder::basic::sub_document subdoc) {
                ret = zdp::zbson_pack(subdoc, _t.data);
            }));
    }
    return ret;
}

template <typename T>
int zbson_pack(bsoncxx::builder::basic::sub_document& _obj_builder,
               const response_json_t<std::vector<T> >& _t) {
    int ret = 0;

    _obj_builder.append(bsoncxx::builder::basic::kvp("errcode", _t.errcode));
    _obj_builder.append(bsoncxx::builder::basic::kvp("errdesc", _t.errdesc));

    if (_t.errcode >= 0) {
        _obj_builder.append(bsoncxx::builder::basic::kvp(
            "data", [&_t, &ret](bsoncxx::builder::basic::sub_array child) {
                for (unsigned i = 0; i < _t.data.size(); ++i) {
                    child.append([&_t, &ret, i](bsoncxx::builder::basic::sub_document subdoc) {
                        ret = zbson_pack(subdoc, _t.data[i]);
                    });
                }
            }));
    }
    return ret;
}

std::string to_json(bson_document_view view);

bson_document_value from_json(const char* data, int len);
}  // namespace zdp
