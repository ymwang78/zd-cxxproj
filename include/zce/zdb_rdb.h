#pragma once
#ifndef __ZDB_RDB__H__
#define __ZDB_RDB__H__

#include <zce/zce_smartptr.h>
#include <zce/zce_object.h>

class zdb_stmt : public zce_smartptr_mtbase
{
public:
	enum field_type_e {
		FIELD_TYPE_UNKNOW,
		FIELD_TYPE_BOOL,
		FIELD_TYPE_CHAR,
		FIELD_TYPE_BYTE,
		FIELD_TYPE_SMALLINT,
		FIELD_TYPE_INT,
		FIELD_TYPE_BIGINT,
		FIELD_TYPE_NUMBER,
		FIELD_TYPE_TEXT,
		FIELD_TYPE_DATETM,
		FIELD_TYPE_DATETMTZ,
	};

    struct _endl_t {
    };
    static _endl_t endl;
    
    struct none {
    };
    static none _none;

    struct none_ignoreidx { //will ingore index 
    };

    template<typename T0, 
        typename T1,
        typename T2 = none_ignoreidx,
        typename T3 = none_ignoreidx,
        typename T4 = none_ignoreidx,
        typename T5 = none_ignoreidx,
        typename T6 = none_ignoreidx,
        typename T7 = none_ignoreidx,
        typename T8 = none_ignoreidx,
        typename T9 = none_ignoreidx
    >
    struct body {
        T0 v0;
        T1 v1;
        T2 v2;
        T3 v3;
        T4 v4;
        T5 v5;
        T6 v6;
        T7 v7;
        T8 v8;
        T9 v9;
    };

    template<typename T0,
        typename T1,
        typename T2,
        typename T3,
        typename T4,
        typename T5,
        typename T6,
        typename T7,
        typename T8,
        typename T9
    >
    zdb_stmt& operator << (const body<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& v) {
        (*this) << v.v0 << v.v1 << v.v2 << v.v3 << v.v4 << v.v5 << v.v6 << v.v7 << v.v8 << v.v9;
        return *this;
    }

    template<typename T0,
        typename T1,
        typename T2,
        typename T3,
        typename T4,
        typename T5,
        typename T6,
        typename T7,
        typename T8,
        typename T9
    >
    zdb_stmt& operator >> (body<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& v) {
        (*this) >> v.v0 >> v.v1 >> v.v2 >> v.v3 >> v.v4 >> v.v5 >> v.v6 >> v.v7 >> v.v8 >> v.v9;;
        return *this;
    }

    virtual int init() = 0;
    virtual int reset() = 0;
    virtual int go() = 0;
    virtual int end_row() = 0;    //important: <0, error; = 0, sucess, nomore row; >0 has more row
    virtual int lasterr_code() const = 0;
    virtual int lasterr_column() const = 0;
    virtual const char* get_sqlstr() = 0;
	virtual int get_row() { return 0; };
	virtual int get_column() { return 0; };
	virtual int get_rows_effected() { return 0; }
	virtual const char* get_column_name(int colidx) = 0;
	virtual field_type_e get_next_filed_type() { return FIELD_TYPE_UNKNOW; };

	virtual zdb_stmt& operator << (double v) = 0;
    virtual zdb_stmt& operator << (zce_int64 v) = 0;
    virtual zdb_stmt& operator << (int v) = 0;
    virtual zdb_stmt& operator << (short v) = 0;
    virtual zdb_stmt& operator << (unsigned char v) = 0;
    virtual zdb_stmt& operator << (const std::string& v) = 0;
    virtual zdb_stmt& operator << (const struct timespec& v) = 0;
    virtual zdb_stmt& operator << (const none&) = 0; 
    virtual zdb_stmt& operator << (const none_ignoreidx&) = 0;

	virtual zdb_stmt& operator >> (double& v) = 0;
    virtual zdb_stmt& operator >> (zce_int64& v) = 0;
    virtual zdb_stmt& operator >> (int& v) = 0;
    virtual zdb_stmt& operator >> (short& v) = 0;
    virtual zdb_stmt& operator >> (unsigned char& v) = 0;
    virtual zdb_stmt& operator >> (std::string& v) = 0;
    virtual zdb_stmt& operator >> (struct timespec& v) = 0;
    virtual zdb_stmt& operator >> (none&) = 0;
    virtual zdb_stmt& operator >> (none_ignoreidx&) = 0;

    virtual zdb_stmt& operator << (zce_uint64 v) {
        return operator << ((zce_int64)v);
    }
    virtual zdb_stmt& operator << (const unsigned v) {
        return operator << ((int)v);
    }
    virtual zdb_stmt& operator << (unsigned short v) {
        return operator << ((short)v);
    }
    virtual zdb_stmt& operator << (char v) {
        return operator << ((unsigned char)v);
    }
    virtual zdb_stmt& operator >> (zce_uint64& v) {
        return operator >> ((zce_int64&)v);
    }
    virtual zdb_stmt& operator >> (unsigned& v) {
        return operator >> ((int&)v);
    }
    virtual zdb_stmt& operator >> (unsigned short& v) {
        return operator >> ((short&)v);
    }
    virtual zdb_stmt& operator >> (char& v) {
        return operator >> ((unsigned char&)v);
    }

    //<0, error; = 0, sucess, no more row; >0 has more row
    int operator << (const _endl_t&) {
        return go();
    }

    //<0, error; = 0, sucess, no more row; >0 has more row
    int operator >> (const _endl_t&) {
        return end_row();
    }
};

//////////////////////////////////////////////////////////////////////////

extern zdb_stmt& operator<<(zdb_stmt& stmt_ptr, const std::vector<std::string>& vecargs);

class ZCE_API zdb_connection : public zce_smartptr_mtbase
{
public:

    virtual bool connetion_ok() = 0;

    virtual void close() = 0;

    virtual void begin() = 0;

    virtual void commit() = 0;

    virtual void rollback() = 0;

    virtual int create_stmt(zce_smartptr<zdb_stmt>& stmt_ptr, const char* sql, bool multi) = 0;

	int execute(zce_smartptr<zdb_stmt>& stmt_ptr, const char* sql, const std::vector<std::string>& vecargs);

    int execute_multi(zce_smartptr<zdb_stmt>& stmt_ptr, const char* sql);

    template<typename OUT_TYPE>
    int execute_multi(const char* sql, OUT_TYPE& inst) {
        try {
            zce_smartptr<zdb_stmt> stmt_ptr;

            int rc = create_stmt(stmt_ptr, sql, true);
            if (rc < 0)
                return rc;

            stmt_ptr->reset();

            rc = *stmt_ptr << zdb_stmt::endl;

            if (rc > 0) {

                rc = *stmt_ptr >> inst >> zdb_stmt::endl;

                if (rc < 0) {
                    return rc;
                }

                return 1;
            }
            return rc;
        }
        catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            close();
        }
        return -1;
    };

    template<typename IN_TYPE, typename OUT_TYPE>
    int execute(const char* sql, OUT_TYPE& inst, const IN_TYPE& input) {
        try {
            zce_smartptr<zdb_stmt> stmt_ptr;

            int rc = create_stmt(stmt_ptr, sql, false);
            if (rc < 0)
                return rc;

            stmt_ptr->reset();

            rc = *stmt_ptr << input << zdb_stmt::endl;

            if (rc > 0) {

                rc = *stmt_ptr >> inst >> zdb_stmt::endl;

                if (rc < 0) {
                    return rc;
                }

                return 1;
            }
            return rc;
        }
        catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            close();
        }
        return -1;
    };

    template<typename IN_TYPE, typename OUT_TYPE>
    int execute(const char* sql, std::vector<OUT_TYPE>& vec, const IN_TYPE& input) {
        try {
            zce_smartptr<zdb_stmt> stmt_ptr;

            int rc = create_stmt(stmt_ptr, sql, false);
            if (rc < 0)
                return rc;

            stmt_ptr->reset();

            rc = *stmt_ptr << input << zdb_stmt::endl;
            if (rc <= 0)
                return rc;

            do{
                OUT_TYPE m;
                rc = *stmt_ptr >> m >> zdb_stmt::endl;
                if (rc < 0) {
                    return rc;
                }
                vec.push_back(std::move(m));
            } while (rc > 0);

            if (vec.empty())
                return rc;
            return (int)vec.size();
        }
        catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            close();
        }
        return -1;
    };
};

typedef zce_smartptr<zdb_connection> zdb_connection_ptr;

class ZCE_API zdb_database : public zce_smartptr_mtbase
{
public:
    class zdb_database_impl : public zce_smartptr_mtbase {
    public:
        virtual zce_smartptr<zdb_connection> get_connection() = 0;
    };

    typedef zdb_database::zdb_database_impl* (*pfn_zdb_database_impl_create)(const zce_astring& connection_str);

    enum ERV_DATABASE {
        ERV_DATABASE_UNKNOW,
        ERV_DATABASE_SQLITE,
        ERV_DATABASE_MYSQL,
        ERV_DATABASE_PGSQL,
        ERV_DATABASE_LIMIT,
    };

    zdb_database(ERV_DATABASE e, const zce_astring& connection_str);

    virtual ~zdb_database();

    zce_smartptr<zdb_connection> get_connection();

    ERV_DATABASE database_type() const { return database_; }

private:
    const ERV_DATABASE    database_;
    zce_smartptr<zdb_database_impl> pimpl_ptr_;
};


#endif //__ZDB_RDB__H__
