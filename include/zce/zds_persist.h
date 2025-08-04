#pragma once

#include <zce/zce_config.h>
#include <zce/zce_object.h>
#include <zce/zce_task.h>
#include <zce/zce_task_queue.h>
#include <zce/zce_dblock.h>
#include <zce/zce_string.h>
#include <zce/zdb_mgo.h>
#include <zce/zdb_redis.h>

namespace zce {
class zdb_mgo_connection;

class ZdbRedisConnection;

class ZdsPersist : public zce::Object {
  protected:
    zce::SmartPtr<zce::Scheduler> write_schedule_;
    zce::SmartPtr<zce::TaskQueue> queue_ptr_;

    class async_write_task : public zce::Task {
        zce::SmartPtr<ZdsPersist> persist_ptr_;
        std::string dbname_;
        std::string coll_;
        zce::RefBlock key_;
        zce::RefBlock obj_;
        bool upsert_;

      public:
        async_write_task(const zce::SmartPtr<ZdsPersist>& ptr, const std::string& dbname,
                         const std::string& coll, const zce::RefBlock& key, const zce::RefBlock& obj,
                         bool upsert);

        virtual void call();
    };

    ~ZdsPersist(){};

    virtual zce::SmartPtr<zdb_mgo_connection> get_mgo_conn(const std::string& dbname) = 0;

    virtual zce::SmartPtr<ZdbRedisConnection> get_redis_conn() = 0;

  public:
    int init();

    static zce::RefBlock to_dblock(const bsoncxx::document::view& v);

    static zce::RefBlock to_setdblock(const zce::RefBlock& dblock);

    static bsoncxx::document::value to_query(const std::string& v);

    static bsoncxx::document::value to_query(unsigned v);

    static bsoncxx::document::value to_query(int64_t v);

    ///////////////////////////////////////////////////////////////////////////

    template <typename T>
    int redist_read_data(const std::string& coll, const std::string& key, T& obj);

    int redist_write_data(const std::string& coll, const std::string& key, const zce::RefBlock& obj,
                          int expire = 3600 * 24 * 3);

    int redis_inc(const std::string& coll, const std::string& key, zce_int64 v, int expire,
                  zce_int64* outv);

    int redis_incrank(const std::string& coll, const std::string& key, zce_int64 v, int expire,
                      zce_int64* outv);

    ///////////////////////////////////////////////////////////////////////////

    int mongo_write_async(const std::string& dbname, const std::string& coll,
                          const bsoncxx::document::value& key, const zce::RefBlock& obj, bool upsert);

    ///////////////////////////////////////////////////////////////////////////

    template <typename KEYT>
    int inc(const std::string& dbname, const std::string& coll, const KEYT& key, zce_int64 incv,
            int expiresec, zce_int64* outv, bool tologdb, bool logconsist);

    template <typename KEYRANK, typename KEYT>
    int inc_rank(const std::string& dbname, const std::string& coll, const KEYRANK& rankid,
                 const KEYT& keyid, zce_int64 incv, int expiresec, zce_int64* outv,
                 bsoncxx::document::view* extraset, bool tologdb, bool logconsist);

    template <typename KEYT>
    int write_bson(const std::string& platname, const std::string& coll, const KEYT& key,
                   const zce::RefBlock& bsonobj, const zce::RefBlock* bsonextra = 0, bool upsert = true);
};

template <typename T>
int ZdsPersist::redist_read_data(const std::string& coll, const std::string& key, T& dblock) {
    int ret = -1;

    do {
        auto redis_conn = get_redis_conn();
        if (redis_conn == 0) break;

        ret = redis_conn->get(zce_string_format(1024, "%s:%s", coll.c_str(), key.c_str()), dblock);
        return ret;
    } while (0);

    return -1;
}

template <typename KEYT>
int ZdsPersist::inc(const std::string& dbname, const std::string& coll, const KEYT& key,
                     zce_int64 incv, int expiresec, zce_int64* outvptr, bool tologdb,
                     bool logconsist) {
    zce_int64 outv = 0;
    int ret = redis_inc(coll, zce::to_string(key), incv, expiresec, outv);
    ZCE_ASSERT(ret >= 0);

    if (outvptr) *outvptr = outv;

    if (!tologdb) return ret;

    if (logconsist) {
        auto objv = bsoncxx::builder::stream::document{}
                    << "$set" << bsoncxx::builder::stream::open_document << "value" << (int64_t)outv
                    << bsoncxx::builder::stream::close_document
                    << bsoncxx::builder::stream::finalize;

        return mongo_write_async(dbname, coll, to_query(key), to_dblock(objv.view()), true);
    } else {
        auto objv = bsoncxx::builder::stream::document{}
                    << "$inc" << bsoncxx::builder::stream::open_document << "value" << (int64_t)incv
                    << bsoncxx::builder::stream::close_document
                    << bsoncxx::builder::stream::finalize;
        return mongo_write_async(dbname, coll, to_query(key), to_dblock(objv.view()), true);
    }
}

template <typename KEYRANK, typename KEYT>
int ZdsPersist::inc_rank(const std::string& dbname, const std::string& coll, const KEYRANK& rankid,
                          const KEYT& keyid, zce_int64 incv, int expiresec, zce_int64* outvptr,
                          bsoncxx::document::view* extraset, bool tologdb, bool logconsist) {
    if (incv == 0) return 0;

    zce_int64 outv = 0;
    auto rediscoll = coll + ":" + zce::to_string(rankid);
    int ret = redis_incrank(rediscoll, zce::to_string(keyid), incv, expiresec, &outv);
    ZCE_ASSERT(ret >= 0);
    if (outvptr) *outvptr = outv;

    if (!tologdb) return ret;

    auto key = zce::to_string(rankid) + "_" + zce::to_string(keyid);

    if (logconsist) {
        if (extraset) {
            auto objv = bsoncxx::builder::stream::document{}
                        << "$set" << bsoncxx::builder::stream::open_document << "rankid" << rankid
                        << "keyid" << keyid << "value" << (int64_t)outv << "wincoin"
                        << (int64_t)outv << bsoncxx::builder::stream::close_document << "$set"
                        << *extraset << bsoncxx::builder::stream::finalize;

            return mongo_write_async(dbname, coll, to_query(key), to_dblock(objv), true);
        } else {
            auto objv = bsoncxx::builder::stream::document{}
                        << "$set" << bsoncxx::builder::stream::open_document << "rankid" << rankid
                        << "keyid" << keyid << "value" << (int64_t)outv << "wincoin"
                        << (int64_t)outv << bsoncxx::builder::stream::close_document
                        << bsoncxx::builder::stream::finalize;

            return mongo_write_async(dbname, coll, to_query(key), to_dblock(objv), true);
        }

    } else {
        if (extraset) {
            auto objv = bsoncxx::builder::stream::document{}
                        << "$inc" << bsoncxx::builder::stream::open_document << "value"
                        << (int64_t)incv << "wincoin" << (int64_t)incv
                        << bsoncxx::builder::stream::close_document << "$set"
                        << bsoncxx::builder::stream::open_document << "rankid" << rankid << "keyid"
                        << keyid << bsoncxx::builder::stream::close_document << "$set" << *extraset
                        << bsoncxx::builder::stream::finalize;

            return mongo_write_async(dbname, coll, to_query(key), to_dblock(objv), true);
        } else {
            auto objv =
                bsoncxx::builder::stream::document{}
                << "$inc" << bsoncxx::builder::stream::open_document << "value" << (int64_t)incv
                << "wincoin" << (int64_t)incv << bsoncxx::builder::stream::close_document << "$set"
                << bsoncxx::builder::stream::open_document << "rankid" << rankid << "keyid" << keyid
                << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;

            return mongo_write_async(dbname, coll, to_query(key), to_dblock(objv), true);
        }
    }
}

template <typename KEYT>
int ZdsPersist::write_bson(const std::string& platname, const std::string& coll, const KEYT& key,
                            const zce::RefBlock& bsonobj, const zce::RefBlock* bsonextra, bool upsert) {
    int ret = redist_write_data(coll, zce::to_string(key), bsonobj);

    bsoncxx::document::view obj(bsonobj.rd_ptr(), bsonobj.length());
    bsoncxx::document::view extra(bsonextra ? bsonextra->rd_ptr() : 0,
                                  bsonextra ? bsonextra->length() : 0);

    if (bsonextra == 0) {
        auto objvalue = bsoncxx::builder::stream::document{} << "$set" << obj
                                                             << bsoncxx::builder::stream::finalize;
        return mongo_write_async(platname, coll, to_query(key), to_dblock(objvalue.view()), upsert);
    } else {
        auto objvalue = bsoncxx::builder::stream::document{} << "$set" << obj << "$set" << extra
                                                             << bsoncxx::builder::stream::finalize;
        return mongo_write_async(platname, coll, to_query(key), to_dblock(objvalue.view()), upsert);
    }
}
}  // namespace zce
