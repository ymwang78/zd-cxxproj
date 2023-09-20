#pragma once

#include <zce/zce_task.h>
#include <zce/zce_bytes.h>
#include <mongocxx/client.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>

class zce_dblock;

class ZCE_API zdb_mgo_connection : public zce_smartptr_mtbase, public mongocxx::v_noabi::client
{
    bool connection_ok_;

    mongocxx::database database_;

public:

    zdb_mgo_connection() {};

    zdb_mgo_connection(const std::string& uri, const std::string& dbname);

    bool connetion_ok() const;

    mongocxx::collection collection(const std::string& coll) const;

    mongocxx::database& dbase() {
        return database_;
    }
};

class ZCE_API zdb_mgo_database
{
    std::string uri_;

    std::string dbname_;

    zce_tss tss_conn_;

public:

    zdb_mgo_database(const std::string& uri, const std::string& dbname);

    zce_smartptr<zdb_mgo_connection> get_connection();
};

zce_int64 ZCE_API zdb_mgo_getintfield(const bsoncxx::document::view& p, const std::string& field, zce_int64 defaultval);

#define zdb_mgo_getfield_x(x) inline x zdb_mgo_getfield(const bsoncxx::document::view& doc, const std::string& field, x defaultval = 0) { \
    return (x)zdb_mgo_getintfield(doc, field, defaultval); \
}

zdb_mgo_getfield_x(zce_char);
zdb_mgo_getfield_x(zce_byte);
zdb_mgo_getfield_x(zce_int16);
zdb_mgo_getfield_x(zce_uint16);
zdb_mgo_getfield_x(zce_int32);
zdb_mgo_getfield_x(zce_uint32);
zdb_mgo_getfield_x(zce_int64);
zdb_mgo_getfield_x(zce_uint64);

std::string ZCE_API zdb_mgo_getfield(const bsoncxx::document::view& p, const std::string& field, const char* defaultval);

std::string ZCE_API zdb_mgo_getfield(const bsoncxx::document::view& p, const std::string& field, const std::string& defaultval);

bool ZCE_API zdb_mgo_getfield(const bsoncxx::document::view& p, const std::string& field, bool def = false);

int ZCE_API zdb_mgo_getfield(const bsoncxx::document::view& p, const std::string& field, zce_dblock& dblock);

int ZCE_API zdb_mgo_getfield(const bsoncxx::document::view& p, const std::string& field, bsoncxx::document::view& subv);

void ZCE_API zdb_mgo_getarray(std::vector<zce_int64>& v, const bsoncxx::document::view& doc, const std::string& field);

void ZCE_API zdb_mgo_getarray(std::vector<int>& v, const bsoncxx::document::view& p, const std::string& field);

void ZCE_API zdb_mgo_getarray(std::vector<unsigned>& v, const bsoncxx::document::view& p, const std::string& field);

void ZCE_API zdb_mgo_getarray(std::vector<short>& v, const bsoncxx::document::view& p, const std::string& field);

void ZCE_API zdb_mgo_getarray(std::vector<zce_byte>& v, const bsoncxx::document::view& doc, const std::string& field);

void ZCE_API zdb_mgo_getarray(std::vector<std::string>& v, const bsoncxx::document::view& doc, const std::string& field);

int ZCE_API  zdb_mgo_getobjarray(const bsoncxx::document::view& doc, const std::string& field,
    std::function<int(const bsoncxx::document::view& subv)> f);

class ZCE_API zdb_mgo_obj
{
    const bsoncxx::document::view& doc_;

public:

    zdb_mgo_obj(const bsoncxx::document::view&);

    zce_int64 get(const std::string& f, zce_int64 def = 0);

    std::string get(const std::string& f, const std::string& def = "");

    int get_dblock(const std::string& f, zce_dblock& dblock);

    bool get_bool(const std::string& f, bool def = false);
};

template< typename T>
bsoncxx::array::value zdb_mgo_toarray(const std::vector<T>& arr)
{
    bsoncxx::builder::stream::array v{};
    for (auto iter = arr.begin(); iter != arr.end(); ++iter) {
        v << (int64_t)*iter;
    }
    return v << bsoncxx::builder::stream::finalize;
};

namespace zdp
{
    typedef typename bsoncxx::builder::basic::document bson_doc;
    typedef typename bsoncxx::builder::basic::sub_document bson_builder;
    typedef typename bsoncxx::document::view bson_view;

    template <typename T>
    struct response_json_t {
        int error_code;
        std::string error_desc;
        T data;
    };

    template <typename T>
    int zbson_pack(bsoncxx::builder::basic::sub_document& _obj_builder, const response_json_t<T>& _t)
    {
        int ret = 0;

        _obj_builder.append(bsoncxx::builder::basic::kvp("error_code", _t.error_code));
        _obj_builder.append(bsoncxx::builder::basic::kvp("error_desc", _t.error_desc));

        if (_t.error_code >= 0) {
            _obj_builder.append(bsoncxx::builder::basic::kvp("data",
                [&_t, &ret](bsoncxx::builder::basic::sub_document subdoc) {
                    ret = zdp::zbson_pack(subdoc, _t.data);
                }
            ));
        }
        return ret;
    }

    template <typename T>
    int zbson_pack(bsoncxx::builder::basic::sub_document& _obj_builder, 
        const response_json_t<std::vector<T> >& _t)
    {
        int ret = 0;

        _obj_builder.append(bsoncxx::builder::basic::kvp("error_code", _t.error_code));
        _obj_builder.append(bsoncxx::builder::basic::kvp("error_desc", _t.error_desc));

        if (_t.error_code >= 0) {
            _obj_builder.append(bsoncxx::builder::basic::kvp("data",
                [&_t, &ret](bsoncxx::builder::basic::sub_array child) {
                for (unsigned i = 0; i < _t.data.size(); ++i) {
                    child.append([&_t, &ret, i](bsoncxx::builder::basic::sub_document subdoc) {
                        ret = zbson_pack(subdoc, _t.data[i]);
                    });
                }
            }
            ));
        }
        return ret;
    }

    std::string to_json(bsoncxx::document::view view);

    bsoncxx::document::value from_json(const char* data, int len);
}
