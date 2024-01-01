// ***************************************************************
//  zdb_ecpg   version:  1.0   -  date: 2015/01/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __vx_ecpg_h__
#define __vx_ecpg_h__

#include <time.h>
#include <pgtypes_timestamp.h>
#include <pgtypes_date.h>

#ifdef __cplusplus
extern "C" 
{
#endif

#define zdb_ecpg_notfound conn_t->lastdesc = sqlca.sqlerrm.sqlerrmc, conn_t->lasterr = 0
#define zdb_ecpg_errno conn_t->lastdesc = sqlca.sqlerrm.sqlerrmc, conn_t->lasterr = sqlca.sqlcode
    struct zdb_allocator_t;

    struct ZCE_API zdb_ecpg_conn_t
    {
        long            dbid;
        int             lasterr;
        const char*     lastdesc;
        const char*     laststmt;
        const char*     lastsql;
        //zdb_allocator_t* allocator;
        char            connid[32];    //thread spec
    };

    //dbname[@hostname][:port]
    int ZCE_API zdb_ecpg_init(struct zdb_ecpg_conn_t* conn_t, long dbid);

    int ZCE_API zdb_ecpg_connect(struct zdb_ecpg_conn_t* conn, const char* db, const char* user, const char* pwd);

    void ZCE_API zdb_ecpg_close(struct zdb_ecpg_conn_t* conn);

    timestamp ZCE_API zdb_ecpg_timet_timestamp(time_t t);

    time_t ZCE_API zdb_ecpg_timestamp_timet(timestamp);

    date ZCE_API zdb_ecpg_timet_date(time_t t);

    time_t ZCE_API zdb_ecpg_date_timet(date d);
#ifdef __cplusplus
}  /* end extern "C" */
#endif

#ifdef __cplusplus

#include <zce/zce_object.h>
#include <zce/zce_task.h>

class ZCE_API zdb_ecpg_connection : public zce_object
{
    zdb_ecpg_conn_t conn_t;

public:

    zdb_ecpg_connection() {}; //for compile only

    zdb_ecpg_connection(long dbid, const std::string& dbhost, const std::string& dbname, const std::string& dbuser, const std::string& dbpasswd);

    zdb_ecpg_conn_t* conn_ptr() { return &conn_t; };

    bool connection_ok() { return conn_t.lasterr != -200 && conn_t.lasterr != -221; }

    void close();
};

class ZCE_API zdb_ecpg_database : public zce_object
{
    long dbid_;
    std::string dbhost_;
    std::string dbname_;
    std::string dbuser_;
    std::string dbpasswd_;
    zce_tss tss_conn_;

public:

    zdb_ecpg_database(const std::string& dbhost, const std::string& dbname, const std::string& dbuser, const std::string& dbpasswd);

    zce_smartptr<zdb_ecpg_connection> get_connection();

    void reset_connection();

    long dbid() const { return dbid_; }
};

#endif

#endif // __vx_ecpg_h__
