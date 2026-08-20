#pragma once
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
#include <zce/zce_object.h>
#include <zce/zce_object_counter.h>

namespace zce {
namespace zdb {

/**
 * @brief Connection parameters supplied field by field.
 *
 * Every field is taken verbatim, so no character is reserved: a password may contain
 * ':', '@' or '/'. Prefer this over the "user:passwd@host:port/dbname" connection
 * string, which cannot express those characters -- see zdb_connstr_parser().
 *
 * For Database::ERV_DATABASE_SQLITE only @a dbname is used, as the database file path
 * (its "path?threadsafe=..;dbkey=.." suffix is still honoured); the other fields are
 * ignored.
 */
struct ConnectionParams {
    std::string host;      ///< Host name or address; backend default is used when empty.
    std::string port;      ///< Port; backend default is used when empty.
    std::string dbname;    ///< Database name (SQLite: database file path).
    std::string user;      ///< User name; backend default is used when empty.
    std::string password;  ///< Password, taken verbatim.
};

/**
 * @brief Parse a "[user[:passwd]@]host[:port][/dbname]" connection string.
 *
 * The authority ends at the first '/', the credentials at the *last* '@' before it, and
 * the port at the last ':' of what remains; an IPv6 host must be bracketed, as in
 * "[::1]:5432". A password may therefore hold '@' and ':'.
 *
 * The syntax still cannot express every value. Where an input has two readings and
 * neither is provably the intended one, it is rejected rather than silently split into
 * the wrong fields:
 * - a user, password or host holding '/', *when credentials are present* -- the leaked
 *   '/' ends the authority early, so ':', '@' or '/' turns up in what is left as the
 *   database name; reported, returns < 0. With no '@' anywhere there are no credentials
 *   and nothing to confuse, so the database name is taken verbatim: "host/db:name" and
 *   "host/db/name" parse as they always have;
 * - an unbracketed IPv6 host -- reported, returns < 0;
 * - a non-numeric port -- reported, returns < 0;
 * - a user holding ':' is indistinguishable from "user:password" and is therefore *not*
 *   detectable: the text before the first ':' always wins.
 *
 * Diagnostics never echo any part of @a connstr: on a malformed input a credential
 * fragment can land in any field.
 *
 * Use ConnectionParams whenever a field may hold one of those characters.
 *
 * @param[in]  connstr Connection string, with any "scheme://" prefix already stripped.
 * @param[out] dbuser  User name, empty when the string carries no credentials.
 * @param[out] dbpass  Password, empty when the string carries none.
 * @param[out] dbhost  Host, empty when the string starts at the port.
 * @param[out] dbport  Port, empty when the string carries none.
 * @param[out] dbname  Database name, empty when the string carries none.
 * @return 0 on success, < 0 when the string cannot be parsed unambiguously. All five
 *         outputs are cleared on entry and left cleared on failure, so a caller that
 *         skips the return code cannot pick up a half-parsed field.
 */
int ZCE_API zdb_connstr_parser(const std::string& connstr, std::string& dbuser,
                               std::string& dbpass, std::string& dbhost, std::string& dbport,
                               std::string& dbname);

class Statement : public zce::Object {
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

    struct _endl_t {};
    static _endl_t endl;

    struct none {};
    static none _none;

    struct none_ignoreidx {  // will ingore index
    };
    static none_ignoreidx _none_ignoreidx;

    template <typename T0, typename T1, typename T2 = none_ignoreidx, typename T3 = none_ignoreidx,
              typename T4 = none_ignoreidx, typename T5 = none_ignoreidx,
              typename T6 = none_ignoreidx, typename T7 = none_ignoreidx,
              typename T8 = none_ignoreidx, typename T9 = none_ignoreidx>
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

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5,
              typename T6, typename T7, typename T8, typename T9>
    Statement& operator<<(const body<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& v) {
        (*this) << v.v0 << v.v1 << v.v2 << v.v3 << v.v4 << v.v5 << v.v6 << v.v7 << v.v8 << v.v9;
        return *this;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5,
              typename T6, typename T7, typename T8, typename T9>
    Statement& operator>>(body<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& v) {
        (*this) >> v.v0 >> v.v1 >> v.v2 >> v.v3 >> v.v4 >> v.v5 >> v.v6 >> v.v7 >> v.v8 >> v.v9;
        ;
        return *this;
    }

    virtual int init() = 0;
    virtual int reset() = 0;
    virtual int go() = 0;
    virtual int end_row() = 0;  // important: <0, error; = 0, sucess, nomore row; >0 has more row
    virtual int lasterr_code() const = 0;
    virtual int lasterr_column() const = 0;
    virtual const char* get_sqlstr() = 0;
    virtual int get_column() { return 0; };
    virtual int get_rows_effected() { return 0; }
    virtual const char* get_column_name(int colidx) = 0;
    virtual field_type_e get_next_filed_type() { return FIELD_TYPE_UNKNOW; };

    virtual Statement& operator<<(double v) = 0;
    virtual Statement& operator<<(zce_int64 v) = 0;
    virtual Statement& operator<<(int v) = 0;
    virtual Statement& operator<<(short v) = 0;
    virtual Statement& operator<<(unsigned char v) = 0;
    virtual Statement& operator<<(const std::string& v) = 0;
    virtual Statement& operator<<(const struct timespec& v) = 0;
    virtual Statement& operator<<(const none&) = 0;
    virtual Statement& operator<<(const none_ignoreidx&) = 0;
#if !defined(WIN32) && !defined(__APPLE__)
    virtual Statement& operator<<(long long v) { return operator<<((zce_int64)v); }
#endif

    virtual Statement& operator>>(double& v) = 0;
    virtual Statement& operator>>(zce_int64& v) = 0;
    virtual Statement& operator>>(int& v) = 0;
    virtual Statement& operator>>(short& v) = 0;
    virtual Statement& operator>>(unsigned char& v) = 0;
    virtual Statement& operator>>(std::string& v) = 0;
    virtual Statement& operator>>(struct timespec& v) = 0;
    virtual Statement& operator>>(none&) = 0;
    virtual Statement& operator>>(none_ignoreidx&) = 0;
#if !defined(WIN32) && !defined(__APPLE__)
    virtual Statement& operator>>(long long& v) { return operator>>((zce_int64&)v); }
#endif

    virtual Statement& operator<<(zce_uint64 v) { return operator<<((zce_int64)v); }
    virtual Statement& operator<<(const unsigned v) { return operator<<((int)v); }
    virtual Statement& operator<<(unsigned short v) { return operator<<((short)v); }
    virtual Statement& operator<<(char v) { return operator<<((unsigned char)v); }
    virtual Statement& operator>>(zce_uint64& v) { return operator>>((zce_int64&)v); }
    virtual Statement& operator>>(unsigned& v) { return operator>>((int&)v); }
    virtual Statement& operator>>(unsigned short& v) { return operator>>((short&)v); }
    virtual Statement& operator>>(char& v) { return operator>>((unsigned char&)v); }

    //<0, error; = 0, sucess, no more row; >0 has more row
    int operator<<(const _endl_t&) { return go(); }

    //<0, error; = 0, sucess, no more row; >0 has more row
    int operator>>(const _endl_t&) { return end_row(); }
};

//////////////////////////////////////////////////////////////////////////

extern Statement& operator<<(Statement& stmt_ptr, const std::vector<std::string>& vecargs);

class ZCE_API Connection : public zce::Object {
  public:
    virtual bool connetion_ok() = 0;

    virtual void close() = 0;

    virtual void begin() = 0;

    virtual void commit() = 0;

    virtual void rollback() = 0;

    virtual int create_stmt(zce::SmartPtr<Statement>& stmt_ptr, const char* sql, bool multi) = 0;

    virtual int backup(const char* arg1, const char* arg2) {
        ZCE_ASSERT(false);
        return -1;
    };

    int execute(zce::SmartPtr<Statement>& stmt_ptr, const char* sql,
                const std::vector<std::string>& vecargs);

    int execute_multi(zce::SmartPtr<Statement>& stmt_ptr, const char* sql);

    template <typename OUT_TYPE>
    int execute_multi(const char* sql, OUT_TYPE& inst) {
        try {
            zce::SmartPtr<Statement> stmt_ptr;

            int rc = create_stmt(stmt_ptr, sql, true);
            if (rc < 0) return rc;

            stmt_ptr->reset();

            rc = *stmt_ptr << Statement::endl;

            if (rc > 0) {
                rc = *stmt_ptr >> inst >> Statement::endl;

                if (rc < 0) {
                    return rc;
                }

                return 1;
            }
            return rc;
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            close();
        }
        return -1;
    };

    template <typename IN_TYPE, typename OUT_TYPE>
    int execute(const char* sql, OUT_TYPE& inst, const IN_TYPE& input) {
        try {
            zce::SmartPtr<Statement> stmt_ptr;

            int rc = create_stmt(stmt_ptr, sql, false);
            if (rc < 0) return rc;

            stmt_ptr->reset();

            rc = *stmt_ptr << input << Statement::endl;

            if (rc > 0) {
                rc = *stmt_ptr >> inst >> Statement::endl;

                if (rc < 0) {
                    return rc;
                }

                return 1;
            }
            return rc;
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            close();
        }
        return -1;
    };

    template <typename IN_TYPE, typename OUT_TYPE>
    int execute(const char* sql, std::vector<OUT_TYPE>& vec, const IN_TYPE& input) {
        try {
            zce::SmartPtr<Statement> stmt_ptr;

            int rc = create_stmt(stmt_ptr, sql, false);
            if (rc < 0) return rc;

            stmt_ptr->reset();

            rc = *stmt_ptr << input << Statement::endl;
            if (rc <= 0) return rc;

            do {
                OUT_TYPE m;
                rc = *stmt_ptr >> m >> Statement::endl;
                if (rc < 0) {
                    return rc;
                }
                vec.push_back(std::move(m));
            } while (rc > 0);

            if (vec.empty()) return rc;
            return (int)vec.size();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            close();
        }
        return -1;
    };
};

typedef zce::SmartPtr<Connection> Connection_ptr;

class ZCE_API Database : public zce::Object {
  public:
    class DatabaseImpl : public zce::Object {
      public:
        virtual zce::SmartPtr<Connection> getConnection() = 0;
        virtual void close() = 0;
    };

    typedef Database::DatabaseImpl* (*pfn_DatabaseImpl_create)(
        const zce_astring& connection_str);

    enum ERV_DATABASE {
        ERV_DATABASE_UNKNOW,
        ERV_DATABASE_SQLITE,
        ERV_DATABASE_MYSQL,
        ERV_DATABASE_PGSQL,
        ERV_DATABASE_LIMIT,
    };

    /**
     * @brief Build from a "[user[:passwd]@]host[:port][/dbname]" connection string.
     *
     * A field holding ':', '@' or '/' cannot survive this form; reach for the
     * ConnectionParams overload in that case. SQLite takes the whole string as its
     * database file path.
     */
    Database(ERV_DATABASE e, const zce_astring& connection_str);

    /**
     * @brief Build from parameters given field by field, bypassing connection-string
     *        assembly and parsing entirely.
     */
    Database(ERV_DATABASE e, const ConnectionParams& params);

    virtual ~Database();

    zce::SmartPtr<Connection> getConnection();

    ERV_DATABASE database_type() const { return database_; }

  private:
    const ERV_DATABASE database_;
    zce::SmartPtr<DatabaseImpl> pimpl_ptr_;
};

template <typename TKEY, typename T>
struct DatabaseObject {
    template <typename TVEC, typename TARG>
    static int select_all_vec(const zce::SmartPtr<Connection>& connptr, TVEC& vec, TARG arg) {
        const char* sql = T::select_all_sql(arg);
        try {
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            *stmtptr << Statement::endl;
            while (stmtptr->end_row() > 0) {
                //@todo check
                T m;
                m.extract(stmtptr);
                vec.push_back(std::move(m));
            }

            return (int)vec.size();
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    };

    template <typename TMAP, typename TARG>
    static int select_all(const zce::SmartPtr<Connection>& connptr, TMAP& dict, TARG arg) {
        const char* sql = T::select_all_sql(arg);
        try {
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            *stmtptr << Statement::endl;
            while (stmtptr->end_row() > 0) {
                //@todo check
                T m;
                m.extract(stmtptr);
                dict.insert(std::make_pair(m.key(), m));
            }
            return 0;
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    };

    template <typename TMAP, typename TARG>
    static int select_all_ptr(const zce::SmartPtr<Connection>& connptr, TMAP& dict, TARG arg) {
        const char* sql = T::select_all_sql(arg);
        try {
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            *stmtptr << Statement::endl;
            while (stmtptr->end_row() > 0) {
                //@todo check
                zce::SmartPtr<T> m(new T);
                m->extract(stmtptr);
                dict.insert(std::make_pair(m->key(), m));
            }
            return 0;
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    };

    template <typename P, typename Q>
    static int select_byprop_base(const zce::SmartPtr<Connection>& connptr, std::vector<Q>& vec,
                                  const char* sql, const P& prop, bool isprc = false) {
        try {
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            *stmtptr << prop << Statement::endl;
            while (stmtptr->end_row() > 0) {
                //@todo check
                Q m;
                extract(m, stmtptr);
                vec.push_back(std::move(m));
            }

            return (int)vec.size();
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    };

    template <typename P, typename Q>
    static int select_byprop_base(const zce::SmartPtr<Connection>& connptr, Q& m,
                                  const char* sql, const P& prop) {
        try {
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            *stmtptr << prop << Statement::endl;
            while (stmtptr->end_row() > 0) {
                //@todo check
                extract(m, stmtptr);
                return 1;
            }
            return 0;
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    };

    template <typename P>
    static int select_byprop(const zce::SmartPtr<Connection>& connptr, std::vector<T>& vec,
                             const char* sql, const P& prop) {
        try {
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            *stmtptr << prop << Statement::endl;
            while (stmtptr->end_row() > 0) {
                T m;
                m.extract(stmtptr);
                vec.push_back(std::move(m));
            }

            return (int)vec.size();
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    };

    template <typename P>
    static int select_byprop(const zce::SmartPtr<Connection>& connptr, T& m, const char* sql,
                             const P& prop) {
        try {
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            *stmtptr << prop << Statement::endl;
            while (stmtptr->end_row() > 0) {
                //@todo check
                m.extract(stmtptr);
                return 1;
            }

            return 0;
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    };

    template <typename TARG>
    static int select_bykey(const zce::SmartPtr<Connection>& connptr, T& m, const TKEY& key,
                            TARG arg) {
        const char* sql = m.select_bykey_sql(arg);
        try {
            if (connptr == 0) return -1;
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            *stmtptr << key << Statement::endl;
            while (stmtptr->end_row() > 0) {
                m.extract(stmtptr);
                return 1;
            }
            return 0;
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    };

    template <typename TARG>
    static int select_bykey(const zce::SmartPtr<Connection>& connptr, std::vector<T>& mvec,
                            const TKEY& key, TARG arg, bool isprc = false) {
        const char* sql = T::select_bykey_sql(arg);
        try {
            //@todo check isprc
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            *stmtptr << key << Statement::endl;
            while (stmtptr->end_row() > 0) {
                //@todo check
                mvec.push_back(T());
                mvec[mvec.size() - 1].extract(stmtptr);
            }
            return (int)mvec.size();
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    };

    template <typename TARG>
    static int insert(const zce::SmartPtr<Connection>& connptr, T& m, TARG arg) {
        const char* sql = m.insert_sql(arg);
        try {
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            m.putinsertvars(stmtptr);
            *stmtptr << Statement::endl;
            while (stmtptr->end_row() > 0) {
                //@todo check
                *stmtptr >> Statement::_none_ignoreidx;
            }
            return 0;
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    }

    template <typename TARG>
    static int replace(const zce::SmartPtr<Connection>& connptr, T& m, TARG arg) {
        const char* sql = m.replace_sql(arg);
        try {
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            m.putreplacevars(stmtptr);
            *stmtptr << Statement::endl;
            while (stmtptr->end_row() > 0) {
                //@todo check
                *stmtptr >> Statement::_none_ignoreidx;
            }
            return 0;
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    }

    template <typename TARG>
    static int update(const zce::SmartPtr<Connection>& connptr, T& m, TARG arg) {
        const char* sql = m.update_sql(arg);
        try {
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            m.putupdatevars(stmtptr);
            *stmtptr << Statement::endl;
            while (stmtptr->end_row() > 0) {
                //@todo check here, zce do nothing in skip row
                // connptr->skip_row(stmtptr);
                *stmtptr >> Statement::_none_ignoreidx;
            }
            return 0;
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    }

    static int remove(const zce::SmartPtr<Connection>& connptr, T& m) {
        const char* sql = m.delete_sql();
        try {
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            m.putdeletevars(stmtptr);
            *stmtptr << Statement::endl;
            while (stmtptr->end_row() > 0) {
                //@todo check here, zce do nothing in skip row
                // connptr->skip_row(stmtptr);
                *stmtptr >> Statement::_none_ignoreidx;
            }
            return 0;
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    }

    static int execute(const zce::SmartPtr<Connection>& connptr, T& m, const char* sql,
                       bool isprc = false, const zce::Any& ctx = zce::Any((zce_int64)0)) {
        try {
            //@todo check isprc
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            m.putvars(stmtptr, sql, ctx);
            *stmtptr << Statement::endl;
            while (stmtptr->end_row() > 0) {
                //@todo check
                m.extract(stmtptr);
                return 1;
            }
            return 0;
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    }

    template <typename TARG>
    static int execute(const zce::SmartPtr<Connection>& connptr, std::vector<T>& mvec,
                       TARG prop, const char* sql, bool isprc = false,
                       const zce::Any& ctx = zce::Any((zce_int64)0)) {
        try {
            //@todo check isprc
            zce::SmartPtr<Statement> stmtptr;
            int ret = connptr->create_stmt(stmtptr, sql, false);
            if (ret < 0) return ret;
            *stmtptr << prop << Statement::endl;
            while (stmtptr->end_row() > 0) {
                //@todo check
                mvec.push_back(T());
                mvec[mvec.size() - 1].extract(stmtptr);
            }

            return (int)mvec.size();
        } catch (const std::string& ex) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n\t%s\n", sql, ex.c_str()));
            connptr->close();
        } catch (...) {
            ZCE_ERROR((ZLOG_ERROR, "sql error: %s\n", sql));
            connptr->close();
        }
        return -1;
    }
};

template <typename record, bool enable_transaction = true>
class zdb_table : public zce::Object {
    typedef typename record::zdb_update_e record_update_type;

    std::string name_;

    std::string insert_sql_;

  public:
    zdb_table(const std::string& name) : name_(name) { insert_sql_ = record::insert_schema(name_); }

    const std::string& name() const noexcept { return name_; };

    const std::string& insert_sql() const noexcept { return insert_sql_; }

    int create(Connection_ptr& connection) {
        std::string sqltext = record::get_create_schema(name_);
        return connection->execute(sqltext.c_str(), Statement::_none, Statement::_none);
    }

    int drop(Connection_ptr& connection) {
        std::string sqltext = record::get_drop_schema(name_);
        return connection->execute(sqltext.c_str(), Statement::_none, Statement::_none);
    }

    int query(Connection_ptr& connection, record& rec, typename record::zdb_query_e query_type,
              bool use_transaction = true) {
        return query(connection, rec, rec.get_query_schema(name_, query_type), use_transaction);
    }

    int query(Connection_ptr& connection, record& query_tpl, std::vector<record>& rec,
              typename record::zdb_query_e query_type, bool use_transaction = true) {
        return execute(connection, query_tpl.get_query_schema(name_, query_type), rec,
                       use_transaction);
    }

    int execute(Connection_ptr& connection, const std::string& sqltext,
                bool use_transaction = false) {
        int ret = connection->execute(sqltext.c_str(), Statement::_none, Statement::_none);
        if (ret < 0) return ret;
        return 0;
    }

    int insert(
        Connection_ptr& connection,
        const record&
            rec /*, const std::string& prepsql = std::string(""), bool use_transaction = true*/) {
        return connection->execute(insert_sql_.c_str(), Statement::_none, rec);
    }

    int insert(
        Connection_ptr& connection,
        std::vector<record>&
            rec /*, const std::string& prepsql = std::string(""), bool use_transaction = true*/) {
        if (rec.empty()) return 0;
        int rc = 0;
        connection->begin();
        typename std::vector<record>::iterator iter = rec.begin();
        for (; iter != rec.end(); ++iter) {
            rc = connection->execute(insert_sql_.c_str(), Statement::_none, *iter);
            ZCE_ASSERT(rc >= 0);
        }
        connection->commit();
        return 0;
    }

    int update(Connection_ptr& connection, const record& rec, record_update_type update_type,
               bool use_transaction = true) const {
        std::string sqltext = rec.create_update_schema(name_, update_type);
        return connection->execute(sqltext.c_str(), Statement::_none, Statement::_none);
    }

    int update(Connection_ptr& connection, std::vector<record>& rec,
               record_update_type update_type, bool use_transaction = true) {
        if (rec.size() == 0) return 0;
        std::string sqltext;
        typename std::vector<record>::iterator iter = rec.begin();
        for (; iter != rec.end(); ++iter)
            sqltext += (*iter).create_update_schema(name_, update_type);

        return connection->execute(sqltext.c_str(), Statement::_none, Statement::_none);
    }

    int remove(Connection_ptr& connection, const record& rec,
               typename record::zdb_delete_e delete_type, bool use_transaction = true) const {
        std::string sqltext = rec.create_delete_schema(name_, delete_type);
        return connection->execute(sqltext.c_str(), Statement::_none, Statement::_none);
    }

    int remove(Connection_ptr& connection, std::vector<record>& rec,
               typename record::zdb_delete_e delete_type, bool use_transaction = true) {
        if (rec.empty()) return 0;
        std::string sqltext;
        typename std::vector<record>::iterator iter = rec.begin();
        for (; iter != rec.end(); ++iter)
            sqltext += (*iter).create_delete_schema(name_, delete_type);

        return connection->execute(sqltext.c_str(), Statement::_none, Statement::_none);
    }

    template <typename Y>
    int query(Connection_ptr& connection, Y& rec, const std::string& sql,
              bool use_transaction = false) {
        return connection->execute(sql.c_str(), rec, Statement::_none);
    }

    template <typename Y>
    int query(Connection_ptr& connection, Y& rec, Statement* sql, bool use_transaction = false) {
        return connection->execute(sql, rec, Statement::_none);
    }

    template <typename Y>
    int execute(Connection_ptr& connection, const std::string& sql, std::vector<Y>& rec,
                bool use_transaction = false) {
        return connection->execute(sql.c_str(), rec, Statement::_none);
    }
};

}  // namespace zdb
}  // namespace zce
