﻿#pragma once
// ***************************************************************
//  zdb_rdb   version:  1.0   -  date: 2015/01/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __ZDB_RDB__H__
#define __ZDB_RDB__H__

#include <zce/zce_object.h>
#include <zce/zce_object_counter.h>

int ZCE_API zdb_connstr_parser(const std::string& filename,
	std::string& dbuser,
	std::string& dbpass,
	std::string& dbhost,
	std::string& dbport,
	std::string& dbname);

class zdb_stmt : public zce_object
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
	static none_ignoreidx _none_ignoreidx;

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
	#ifndef WIN32
	virtual zdb_stmt& operator << (long long v) {
		return operator << ((zce_int64)v);
	}
	#endif

	virtual zdb_stmt& operator >> (double& v) = 0;
    virtual zdb_stmt& operator >> (zce_int64& v) = 0;
    virtual zdb_stmt& operator >> (int& v) = 0;
    virtual zdb_stmt& operator >> (short& v) = 0;
    virtual zdb_stmt& operator >> (unsigned char& v) = 0;
    virtual zdb_stmt& operator >> (std::string& v) = 0;
    virtual zdb_stmt& operator >> (struct timespec& v) = 0;
    virtual zdb_stmt& operator >> (none&) = 0;
    virtual zdb_stmt& operator >> (none_ignoreidx&) = 0;
	#ifndef WIN32
	virtual zdb_stmt& operator >> (long long& v) {
		return operator >> ((zce_int64&)v);
	}
	#endif

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

class ZCE_API zdb_connection : public zce_object
{
public:

    virtual bool connetion_ok() = 0;

    virtual void close() = 0;

    virtual void begin() = 0;

    virtual void commit() = 0;

    virtual void rollback() = 0;

    virtual int create_stmt(zce_smartptr<zdb_stmt>& stmt_ptr, const char* sql, bool multi) = 0;

    virtual int backup(const char* arg1, const char* arg2) { ZCE_ASSERT(false); return -1; };

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

class ZCE_API zdb_database : public zce_object
{
public:
    class zdb_database_impl : public zce_object {
    public:
        virtual zce_smartptr<zdb_connection> get_connection() = 0;
        virtual void close() = 0;
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


template <typename TKEY, typename T>
struct zdb_object
{
	template <typename TVEC, typename TARG>
	static int select_all_vec(const zce_smartptr<zdb_connection>& connptr, TVEC& vec, TARG arg)
	{
		const char* sql = T::select_all_sql(arg);
		try
		{
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			*stmtptr << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check
				T m;
				m.extract(stmtptr);
				vec.push_back(std::move(m));
			}

			return (int)vec.size();
		}
		catch (const std::string& ex)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	};

	template <typename TMAP, typename TARG>
	static int select_all(const zce_smartptr<zdb_connection>& connptr, TMAP& dict, TARG arg)
	{
		const char* sql = T::select_all_sql(arg);
		try {
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			*stmtptr << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check
				T m;
				m.extract(stmtptr);
				dict.insert(std::make_pair(m.key(), m));
			}
			return 0;
		}
		catch (const std::string& ex) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	};

	template <typename TMAP, typename TARG>
	static int select_all_ptr(const zce_smartptr<zdb_connection>& connptr, TMAP& dict, TARG arg) {
		const char* sql = T::select_all_sql(arg);
		try {
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			*stmtptr << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check
				zce_smartptr<T> m(new T);
				m->extract(stmtptr);
				dict.insert(std::make_pair(m->key(), m));
			}
			return 0;
		}
		catch (const std::string& ex) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	};

	template<typename P, typename Q>
	static int select_byprop_base(const zce_smartptr<zdb_connection>& connptr, std::vector<Q>& vec, const char* sql, const P& prop, bool isprc = false)
	{
		try
		{
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			*stmtptr << prop << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check
				Q m;
				extract(m, stmtptr);
				vec.push_back(std::move(m));
			}

			return (int)vec.size();
		}
		catch (const std::string& ex)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	};


	template<typename P, typename Q>
	static int select_byprop_base(const zce_smartptr<zdb_connection>& connptr, Q& m, const char* sql, const P& prop)
	{
		try {
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			*stmtptr << prop << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check
				extract(m, stmtptr);
				return 1;
			}
			return 0;
		}
		catch (const std::string& ex) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	};

	template<typename P>
	static int select_byprop(const zce_smartptr<zdb_connection>& connptr, std::vector<T>& vec, const char* sql, const P& prop)
	{
		try {
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			*stmtptr << prop << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check
				T m;
				m.extract(stmtptr);
				vec.push_back(std::move(m));
				return 1;
			}

			return (int)vec.size();
		}
		catch (const std::string& ex) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	};

	template<typename P>
	static int select_byprop(const zce_smartptr<zdb_connection>& connptr, T& m, const char* sql, const P& prop)
	{
		try {
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			*stmtptr << prop << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check
				m.extract(stmtptr);
				return 1;
			}

			return 0;
		}
		catch (const std::string& ex) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	};

	template <typename TARG>
	static int select_bykey(const zce_smartptr<zdb_connection>& connptr, T& m, const TKEY& key, TARG arg) {
		const char* sql = m.select_bykey_sql(arg);
		try {
			if (connptr == 0)
				return -1;
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			*stmtptr << key << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				m.extract(stmtptr);
				return 1;
			}
			return 0;
		}
		catch (const std::string& ex) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	};

	template <typename TARG>
	static int select_bykey(const zce_smartptr<zdb_connection>& connptr, std::vector<T>& mvec, const TKEY& key, TARG arg, bool isprc = false)
	{
		const char* sql = T::select_bykey_sql(arg);
		try
		{
			//@todo check isprc 
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			*stmtptr << key << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check
				mvec.push_back(T());
				mvec[mvec.size() - 1].extract(stmtptr);
			}
			return (int)mvec.size();
		}
		catch (const std::string& ex)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	};

	template<typename TARG>
	static int insert(const zce_smartptr<zdb_connection>& connptr, T& m, TARG arg)
	{
		const char* sql = m.insert_sql(arg);
		try
		{
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			m.putinsertvars(stmtptr);
			*stmtptr << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check
				*stmtptr >> zdb_stmt::_none_ignoreidx;
			}
			return 0;
		}
		catch (const std::string& ex)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	}

	template<typename TARG>
	static int replace(const zce_smartptr<zdb_connection>& connptr, T& m, TARG arg)
	{
		const char* sql = m.replace_sql(arg);
		try
		{
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			m.putreplacevars(stmtptr);
			*stmtptr << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check
				*stmtptr >> zdb_stmt::_none_ignoreidx;
			}
			return 0;
		}
		catch (const std::string& ex)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	}

	template<typename TARG>
	static int update(const zce_smartptr<zdb_connection>& connptr, T& m, TARG arg)
	{
		const char* sql = m.update_sql(arg);
		try
		{
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			m.putupdatevars(stmtptr);
			*stmtptr << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check here, zce do nothing in skip row
				//connptr->skip_row(stmtptr);
				*stmtptr >> zdb_stmt::_none_ignoreidx;
			}
			return 0;
		}
		catch (const std::string& ex)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	}

	static int remove(const zce_smartptr<zdb_connection>& connptr, T& m)
	{
		const char* sql = m.delete_sql();
		try {
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			m.putdeletevars(stmtptr);
			*stmtptr << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check here, zce do nothing in skip row
				//connptr->skip_row(stmtptr);
				*stmtptr >> zdb_stmt::_none_ignoreidx;
			}
			return 0;
		}
		catch (const std::string& ex) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	}

	static int execute(const zce_smartptr<zdb_connection>& connptr, T& m, const char* sql, bool isprc = false, const zce_any& ctx = zce_any((zce_int64)0))
	{
		try
		{
			//@todo check isprc 
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			m.putvars(stmtptr, sql, ctx);
			*stmtptr << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check
				m.extract(stmtptr);
				return 1;
			}
			return 0;
		}
		catch (const std::string& ex)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...)
		{
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	}

	template<typename TARG>
	static int execute(const zce_smartptr<zdb_connection>& connptr, std::vector<T>& mvec, TARG prop, const char* sql, bool isprc = false, const zce_any& ctx = zce_any((zce_int64)0))
	{
		try
		{
			//@todo check isprc 
			zce_smartptr<zdb_stmt> stmtptr;
			int ret = connptr->create_stmt(stmtptr, sql, false);
			if (ret < 0) return ret;
			*stmtptr << prop << zdb_stmt::endl;
			while (stmtptr->end_row() > 0) {
				//@todo check
				mvec.push_back(T());
				mvec[mvec.size() - 1].extract(stmtptr);
			}

			return (int)mvec.size();
		}
		catch (const std::string& ex) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
			connptr->close();
		}
		catch (...) {
			ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
			connptr->close();
		}
		return -1;
	}
};


template<typename record, bool enable_transaction = true>
class zdb_table : public zce_object
{
    typedef typename record::zdb_update_e record_update_type;

    std::string name_;

    std::string insert_sql_;

public:

    zdb_table(const std::string& name) :name_(name) {
        insert_sql_ = record::insert_schema(name_);
    }

    const std::string& name() const noexcept { return name_; };

    const std::string& insert_sql() const noexcept { return insert_sql_; }

    int create(zdb_connection_ptr& connection)
    {
        std::string sqltext = record::get_create_schema(name_);
        return connection->execute(sqltext.c_str(), zdb_stmt::_none, zdb_stmt::_none);
    }

    int drop(zdb_connection_ptr& connection)
    {
        std::string sqltext = record::get_drop_schema(name_);
        return connection->execute(sqltext.c_str(), zdb_stmt::_none, zdb_stmt::_none);
    }

    int query(zdb_connection_ptr& connection, record& rec, typename record::zdb_query_e query_type, bool use_transaction = true)
    {
        return query(connection, rec, rec.get_query_schema(name_, query_type), use_transaction);
    }

    int query(zdb_connection_ptr& connection, record& query_tpl, std::vector<record>& rec, typename record::zdb_query_e query_type, bool use_transaction = true)
    {
        return execute(connection, query_tpl.get_query_schema(name_, query_type), rec, use_transaction);
    }

    int execute(zdb_connection_ptr& connection, const std::string& sqltext, bool use_transaction = false)
    {
        int ret = connection->execute(sqltext.c_str(), zdb_stmt::_none, zdb_stmt::_none);
        if (ret < 0)
            return ret;
        return 0;
    }

    int insert(zdb_connection_ptr& connection, const record& rec/*, const std::string& prepsql = std::string(""), bool use_transaction = true*/)
    {
        return connection->execute(insert_sql_.c_str(), zdb_stmt::_none, rec);
    }

    int insert(zdb_connection_ptr& connection, std::vector<record>& rec/*, const std::string& prepsql = std::string(""), bool use_transaction = true*/)
    {
        if (rec.empty() )
            return 0;
        int rc = 0;
        connection->begin();
        typename std::vector<record>::iterator iter = rec.begin();
        for (; iter != rec.end(); ++iter) {
            rc = connection->execute(insert_sql_.c_str(), zdb_stmt::_none, *iter);
            ZCE_ASSERT(rc >= 0);
        }
        connection->commit();
        return 0;
    }

    int update(zdb_connection_ptr& connection, const record& rec, record_update_type update_type, bool use_transaction = true)const
    {
        std::string sqltext = rec.create_update_schema(name_, update_type);
        return connection->execute(sqltext.c_str(), zdb_stmt::_none, zdb_stmt::_none);
    }

    int update(zdb_connection_ptr& connection, std::vector<record>& rec, record_update_type update_type, bool use_transaction = true)
    {
        if (rec.size() == 0)
            return 0;
        std::string sqltext;
        typename std::vector<record>::iterator iter = rec.begin();
        for (; iter != rec.end(); ++iter)
            sqltext += (*iter).create_update_schema(name_, update_type);

        return connection->execute(sqltext.c_str(), zdb_stmt::_none, zdb_stmt::_none);
    }

    int remove(zdb_connection_ptr& connection, const record& rec, typename record::zdb_delete_e delete_type, bool use_transaction = true)const
    {
        std::string sqltext = rec.create_delete_schema(name_, delete_type);
        return connection->execute(sqltext.c_str(), zdb_stmt::_none, zdb_stmt::_none);
    }

    int remove(zdb_connection_ptr& connection, std::vector<record>& rec, typename record::zdb_delete_e delete_type, bool use_transaction = true)
    {
        if (rec.empty())
            return 0;
        std::string sqltext;
        typename std::vector<record>::iterator iter = rec.begin();
        for (; iter != rec.end(); ++iter)
            sqltext += (*iter).create_delete_schema(name_, delete_type);

        return connection->execute(sqltext.c_str(), zdb_stmt::_none, zdb_stmt::_none);
    }

    template<typename Y>
    int query(zdb_connection_ptr& connection, Y& rec, const std::string& sql, bool use_transaction = false)
    {
        return connection->execute(sql.c_str(), rec, zdb_stmt::_none);
    }

    template<typename Y>
    int query(zdb_connection_ptr& connection, Y& rec, zdb_stmt* sql, bool use_transaction = false)
    {
        return connection->execute(sql, rec, zdb_stmt::_none);
    }

    template<typename Y>
    int execute(zdb_connection_ptr& connection, const std::string& sql, std::vector<Y>& rec, bool use_transaction = false)
    {
        return connection->execute(sql.c_str(), rec, zdb_stmt::_none);
    }


};

#endif //__ZDB_RDB__H__
