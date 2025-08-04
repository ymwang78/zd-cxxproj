#pragma once
// ***************************************************************
//  zdb_redis   version:  1.0   -  date: 2015/01/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

#include <zce/zce_config.h>
#include <zce/zce_task.h>
#include <zce/zce_types.h>
#include <zce/zce_dblock.h>
#include <zce/zce_string.h>
#include <map>
#include <functional>

struct redisContext;
struct redisReply;

namespace zce {
class Object;

class ZCE_API ZdbRedis {
    redisReply* reply_;

#ifdef WIN32
    // don't know why gcc call this
    ZdbRedis(const ZdbRedis& rhs) {};
#endif

  public:
    ZdbRedis(redisReply* r = 0) : reply_(r) {}

    ZdbRedis& operator=(redisReply* r);

    ~ZdbRedis();

    inline operator redisReply*() const noexcept { return this->reply_; }

    inline redisReply* operator->() const noexcept { return this->reply_; }

    inline bool operator==(redisReply* right) const noexcept { return (this->reply_ == right); }

    inline bool operator!=(redisReply* right) const noexcept { return !(this->reply_ == right); }
};

///////////////////////////////////////////////////////////////////////////////

int ZCE_API zdb_redis_toval(zce::RefBlock& v, const zce::ZdbRedis& r);

int ZCE_API zdb_redis_toval(zce_astring& v, const zce::ZdbRedis& r);

int ZCE_API zdb_redis_toval(zce_int64& v, const zce::ZdbRedis& r);

template <typename T>
int zdb_redis_toval(T& v, const zce::ZdbRedis& r) {
    zce_int64 i64v = 0;
    int ret = zdb_redis_toval(i64v, r);
    if (ret < 0) return ret;
    v = i64v;
    return ret;
}

class ZCE_API ZdbRedisConnection : public zce::Object {
    redisContext* context_;

  public:
    typedef std::vector<std::pair<std::string, zce_int64> > kvpair_vec_t;

    ZdbRedisConnection();

    ZdbRedisConnection(bool ssl, const std::string& ip, unsigned short port, const char* passwd);

    bool connetion_ok() const;

    void close();

    redisContext* context() { return context_; }

    bool key_exists(const std::string& k);

    int hget(ZdbRedis& ptr, const std::string& k, const std::string& h);

    template <typename KEY, typename VAL>
    int hget(const std::string& k, const KEY& h, VAL& v);

    int hgetall(const std::string& k, std::map<std::string, std::string>&);

    int hgetall(const std::string& k, std::map<zce_int64, std::string>&);

    int hgetall(const std::string& k,
                std::function<int(const char*, size_t, const char*, size_t)>& f);

    int hdel(const std::string& k, const std::string& h);

    int hset(const std::string& k, const std::string& h, zce_int64 v, zce_uint32 expiresec);

    int hset(const std::string& k, const std::string& h, const zce_byte* v, unsigned l,
             zce_uint32 expiresec);

    int hinc(const std::string& k, const std::string& h, zce_int64 v, zce_uint32 expiresec,
             zce_int64* outv = 0);

    int set(const std::string& k, zce_int64 v, zce_uint32 expiresec);

    int set(const std::string& k, const std::string& v, zce_uint32 expiresec);

    int set(const std::string& k, const zce_byte* v, unsigned l, zce_uint32 expiresec);

    int get(const std::string& k, std::function<int(ZdbRedis& ptr)>& f);

    int get(const std::string& k, zce_int64& v);

    int get(const std::string& k, std::string& v);

    int get(const std::string& k, zce::RefBlock& v);

    int del(const std::string& k);

    int inc(const std::string& k, zce_int64 val = 1, zce_int64 expiresec = 0, zce_int64* outv = 0);

    int zinc(const std::string& k, const std::string& m, zce_int64 val = 1, zce_int64 expiresec = 0,
             zce_int64* outv = 0);

    int zset(const std::string& k, const std::string& m, zce_int64 val, zce_int64 expiresec = 0);

    int zdel(const std::string& k, const std::string& m);

    int zrange(const std::string& k, kvpair_vec_t& vec, bool inc, int begin, int end);

    int expire(const std::string& k, zce_int64 expiresec);

    int llen(const char* col, unsigned& len);

    int lpush(const std::string& k, const zce_byte* v, unsigned l, zce_uint32 expiresec = 0);

    int rpush(const std::string& k, const zce_byte* v, unsigned l, zce_uint32 expiresec = 0);

    int lpop(const std::string& k, zce::RefBlock& v);
};

class ZCE_API ZdbRedisDatabase : public zce::Object {
    bool ssl_;
    std::string ip_;
    unsigned short port_;
    std::string passwd_;
    zce::Tss tss_conn_;

  public:
    ZdbRedisDatabase(bool ssl, const std::string& ip, unsigned short port, const char* passwd);

    zce::SmartPtr<ZdbRedisConnection> get_connection();

    inline void get_conninfo(std::string& ip, unsigned short& port, std::string& passwd) const {
        ip = ip_;
        port = port_;
        passwd = passwd_;
    }
};


template <typename KEY, typename VAL>
int ZdbRedisConnection::hget(const std::string& k, const KEY& h, VAL& v) {
    ZdbRedis r;
    int ret = hget(r, k, zce::to_string(h));
    if (ret < 0) {
        ZERROR(k, h, hex_t<int>(ret));
        return ret;
    }
    ret = zdb_redis_toval(v, r);
    if (ret < 0) {
        ZERROR(k, h, hex_t<int>(ret));
    }
    return ret;
}

}  // namespace zce


