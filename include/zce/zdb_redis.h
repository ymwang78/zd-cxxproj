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
#ifndef __zdb_redis_h__
#define __zdb_redis_h__

#include <zce/zce_config.h>
#include <zce/zce_task.h>
#include <zce/zce_types.h>
#include <zce/zce_dblock.h>
#include <map>
#include <functional>

struct redisContext;
struct redisReply;
class zdb_redis_ptr;

class ZCE_API zdb_redis_ptr
{
    redisReply* reply_;

#ifdef WIN32
    //don't know why gcc call this
    zdb_redis_ptr(const zdb_redis_ptr& rhs) {}; 
#endif

public:

    zdb_redis_ptr(redisReply* r = 0)
        :reply_(r) {
    }

    zdb_redis_ptr& operator = (redisReply* r);

    ~zdb_redis_ptr();

    inline operator redisReply*() const {
        return this->reply_;
    }

    inline redisReply* operator->() const {
        return this->reply_;
    }

    inline bool operator == (redisReply* right) const {
        return (this->reply_ == right);
    }

    inline bool operator != (redisReply* right) const {
        return !(this->reply_ == right);
    }
};

class ZCE_API zdb_redis_connection : public zce_object
{
    redisContext* context_;

    bool connection_ok_;

public:

    typedef std::vector<std::pair<std::string, zce_int64> > kvpair_vec_t;

    zdb_redis_connection();

    zdb_redis_connection(const std::string& ip, unsigned short port, const char* passwd);

    bool connetion_ok() const {
        return connection_ok_;
    };

    void close();

    redisContext* context() { return context_; }

    bool key_exists(const std::string& k);

    int hget(zdb_redis_ptr& ptr, const std::string& k, const std::string& h);

    template<typename KEY, typename VAL>
    int hget(const std::string& k, const KEY& h, VAL& v);

    int hgetall(const std::string& k, std::map<std::string, std::string>&);

    int hgetall(const std::string& k, std::map<zce_int64, std::string>&);

    int hgetall(const std::string& k, std::function<int(const char*, size_t, const char*, size_t)>& f);

    int hdel(const std::string& k, const std::string& h);

    int hset(const std::string& k, const std::string& h, zce_int64 v, zce_uint32 expiresec);

    int hset(const std::string& k, const std::string& h, const zce_byte* v, unsigned l, zce_uint32 expiresec);

    int hinc(const std::string& k, const std::string& h, zce_int64 v, zce_uint32 expiresec, zce_int64* outv = 0);

    int set(const std::string& k, zce_int64 v, zce_uint32 expiresec);

    int set(const std::string& k, const std::string& v, zce_uint32 expiresec);

    int set(const std::string& k, const zce_byte* v, unsigned l, zce_uint32 expiresec);

    int get(const std::string& k, std::function<int(zdb_redis_ptr& ptr)>& f);

    int get(const std::string& k, zce_int64& v);

    int get(const std::string& k, std::string& v);

    int get(const std::string& k, zce_dblock& v);

    int del(const std::string& k);

    int inc(const std::string& k, zce_int64 val = 1, zce_int64 expiresec = 0, zce_int64* outv = 0);

    int zinc(const std::string& k, const std::string& m, zce_int64 val = 1, zce_int64 expiresec = 0, zce_int64* outv = 0);

    int zset(const std::string& k, const std::string& m, zce_int64 val, zce_int64 expiresec = 0);

    int zdel(const std::string& k, const std::string& m);

    int zrange(const std::string& k, kvpair_vec_t& vec, bool inc, int begin, int end);

    int expire(const std::string& k, zce_int64 expiresec);
};

class ZCE_API zdb_redis_databse : public zce_object
{
    std::string ip_;
    unsigned short port_;
    std::string passwd_;
    zce_tss tss_conn_;

public:

    zdb_redis_databse(const std::string& ip, unsigned short port, const char* passwd);

    zce_smartptr<zdb_redis_connection> get_connection();

	inline void get_conninfo(std::string& ip, unsigned short& port, std::string& passwd) const {
		ip = ip_;
		port = port_;
		passwd = passwd_;
	}
};


int ZCE_API zdb_redis_toval(zce_dblock& v, const zdb_redis_ptr& r);

int ZCE_API zdb_redis_toval(zce_astring& v, const zdb_redis_ptr& r);

int ZCE_API zdb_redis_toval(zce_int64& v, const zdb_redis_ptr& r);
///////////////////////////////////////////////////////////////////////////////
#include <zce/zce_string.h>

template<typename T>
int zdb_redis_toval(T& v, const zdb_redis_ptr& r)
{
    zce_int64 i64v = 0;
    int ret = zdb_redis_toval(i64v, r);
    if (ret < 0)
        return ret;
    v = i64v;
    return ret;
}

template<typename KEY, typename VAL>
int zdb_redis_connection::hget(const std::string& k, const KEY& h, VAL& v)
{
    zdb_redis_ptr r;
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

#endif // __zdb_redis_h__
