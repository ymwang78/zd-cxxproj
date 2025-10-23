#pragma once

#include <zce/zce_config.h>
#include <zce/zce_object.h>
#include <zce/zce_task.h>
#include <zce/zce_task_queue.h>
#include <zce/zce_dblock.h>
#include <zce/zce_string.h>
#include <zce/zdb_redis.h>

namespace zce {
namespace zdb {
class RedisConnection;
}

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

    virtual zce::SmartPtr<zce::zdb::RedisConnection> get_redis_conn() = 0;

  public:
    int init();

    static zce::RefBlock to_setdblock(const zce::RefBlock& dblock);


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

    template <typename KEYT>
    int inc(const std::string& dbname, const std::string& coll, const KEYT& key, zce_int64 incv,
            int expiresec, zce_int64* outv, bool tologdb, bool logconsist);

    template <typename KEYRANK, typename KEYT>
    int inc_rank(const std::string& dbname, const std::string& coll, const KEYRANK& rankid,
                 const KEYT& keyid, zce_int64 incv, int expiresec, zce_int64* outv,
                 bool tologdb, bool logconsist);

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

    return -1;  // TODO: Implement logging to db if needed
}

template <typename KEYRANK, typename KEYT>
int ZdsPersist::inc_rank(const std::string& dbname, const std::string& coll, const KEYRANK& rankid,
                          const KEYT& keyid, zce_int64 incv, int expiresec, zce_int64* outvptr,
                          bool tologdb, bool logconsist) {
    if (incv == 0) return 0;

    zce_int64 outv = 0;
    auto rediscoll = coll + ":" + zce::to_string(rankid);
    int ret = redis_incrank(rediscoll, zce::to_string(keyid), incv, expiresec, &outv);
    ZCE_ASSERT(ret >= 0);
    if (outvptr) *outvptr = outv;

    if (!tologdb) return ret;

    return -1;  // TODO: Implement logging to db if needed
}

template <typename KEYT>
int ZdsPersist::write_bson(const std::string& platname, const std::string& coll, const KEYT& key,
                            const zce::RefBlock& bsonobj, const zce::RefBlock* bsonextra, bool upsert) {
    int ret = redist_write_data(coll, zce::to_string(key), bsonobj);

    //@todo 
}
}  // namespace zce
