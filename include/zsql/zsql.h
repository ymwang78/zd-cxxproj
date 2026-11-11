#pragma once
// ***************************************************************
//  zsql   version:  1.0   -  date:  2026/11/12
//  -------------------------------------------------------------
//  libzsql: SQLite with an encryption layer (SQLite3 Multiple Ciphers)
//  -------------------------------------------------------------
//  This file is a part of project zce.
//  Copyright (C) 2026 - All Rights Reserved
// ***************************************************************

/**
 * @file zsql.h
 * @brief How libzsql keys a database: no key, the built-in default secret, a passphrase, or
 *        a key derived from a KeelLock dongle.
 *
 * libzsql compiles SQLite3 Multiple Ciphers (SQLite3MC), so the whole SQLite API, plus
 * sqlite3_key() and the sqlite3mc_* functions, comes from "zsql/sqlite3.h", which this header
 * includes. The functions below put a fixed policy on top of it:
 *
 * - Every encrypted database uses one cipher scheme with every parameter pinned: SQLCipher v4
 *   (AES-256-CBC, HMAC-SHA512 per page, PBKDF2-HMAC-SHA512 with 256000 iterations, 4096-byte
 *   pages, SQLite3MC's "legacy=4"). A later SQLite3MC that changes its defaults does not change
 *   our file format, and SQLCipher tools (DB Browser for SQLite, sqlcipher, Python sqlcipher3)
 *   open the files with their default v4 settings. The page size is fixed at 4096: an encrypted
 *   database ignores PRAGMA page_size.
 * - Keying fails closed: an empty key, a missing provider or a key that does not decrypt the
 *   file is an error, never a plaintext database.
 * - A plaintext database opened with a key is refused (ZSQL_E_PLAINTEXT) unless the caller
 *   asks for it to be encrypted in place (zsql_key::migrate_plaintext).
 *
 * Header and library must come from the same SQLite3MC release: zsql_header_matches_library()
 * checks that.
 */

#include "sqlite3.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Version of libzsql itself (the wrapper, not SQLite). */
#define ZSQL_VERSION "1.0.0"

#if !defined(ZSQL_API)
#if defined(__GNUC__) || defined(__clang__)
#define ZSQL_API __attribute__((visibility("default")))
#else
#define ZSQL_API
#endif
#endif

#if defined(_MSC_VER) && !defined(__cplusplus)
#define ZSQL_INLINE static __inline
#else
#define ZSQL_INLINE static inline
#endif

/** Size of a raw database key, in bytes (AES-256). */
#define ZSQL_KEY_BYTES 32

/**
 * @name Result codes added by libzsql
 * They are SQLite extended result codes: `rc & 0xff` is the primary SQLite code, so code that
 * only looks at the primary code still sees an error of the right kind.
 * @{
 */
/** The file is encrypted, but not with this key, or it is not a database at all. */
#define ZSQL_E_WRONGKEY (SQLITE_NOTADB | (0x70 << 8))
/** A plaintext database was opened with a key and zsql_key::migrate_plaintext is 0. */
#define ZSQL_E_PLAINTEXT (SQLITE_NOTADB | (0x71 << 8))
/** ZSQL_KEY_KEELLOCK was asked for, but no provider is installed. */
#define ZSQL_E_NOPROVIDER (SQLITE_AUTH | (0x72 << 8))
/** The KeelLock provider could not produce the key (no dongle, feature not licensed, ...). */
#define ZSQL_E_PROVIDER (SQLITE_AUTH | (0x73 << 8))
/** A key was asked for without key material (empty passphrase, unknown mode). */
#define ZSQL_E_BADKEY (SQLITE_MISUSE | (0x74 << 8))
/** SQLite3MC refused the pinned cipher configuration. */
#define ZSQL_E_CIPHER (SQLITE_ERROR | (0x75 << 8))
/** @} */

/** Where the key of a database comes from. */
typedef enum zsql_key_mode {
    /** Plaintext: the database is not encrypted. */
    ZSQL_KEY_NONE = 0,
    /**
     * The default secret compiled into libzsql (or the one a product build put in its place,
     * see ZSQL_DEFAULT_SECRET_FILE in libzsql's CMakeLists.txt). It keeps a database from being
     * read with a stock SQLite tool; anyone who has the program can recover it.
     */
    ZSQL_KEY_DEFAULT = 1,
    /** A passphrase supplied by the caller, stretched with PBKDF2 (about 0.1 to 0.3 s per open). */
    ZSQL_KEY_PASSPHRASE = 2,
    /** A 256-bit key from the provider installed with zsql_set_keellock_provider(). */
    ZSQL_KEY_KEELLOCK = 3
} zsql_key_mode;

/** How to key one database. Zero-initialise it, then set what the mode needs. */
typedef struct zsql_key {
    zsql_key_mode mode;
    /** ZSQL_KEY_PASSPHRASE: the passphrase. Bytes of the form `x'<64 hex digits>'` or
     *  `raw:<32 bytes>` are taken as a raw key, as SQLCipher does. */
    const void* passphrase;
    /** Length of @c passphrase in bytes, or -1 if it is NUL-terminated. */
    int passphrase_len;
    /** ZSQL_KEY_KEELLOCK: which database key the provider derives. NULL or "" means "main". */
    const char* label;
    /** Non-zero: a plaintext database is encrypted in place with this key while it is opened.
     *  Zero: opening a plaintext database with a key fails with ZSQL_E_PLAINTEXT. */
    int migrate_plaintext;
} zsql_key;

/**
 * Produces the raw key of a ZSQL_KEY_KEELLOCK database.
 *
 * @param ctx   The pointer given to zsql_set_keellock_provider().
 * @param label zsql_key::label, never NULL ("main" when the caller gave none).
 * @param key   Receives ZSQL_KEY_BYTES bytes.
 * @return 0 on success; anything else makes the open fail with ZSQL_E_PROVIDER.
 *
 * It is called with libzsql's provider lock held, so it must not call back into libzsql.
 */
typedef int (*zsql_key_provider)(void* ctx, const char* label, unsigned char key[ZSQL_KEY_BYTES]);

/**
 * Install the provider of ZSQL_KEY_KEELLOCK keys, or remove it with NULL.
 *
 * It calls no SQLite function, so a program may install it before zdb configures SQLite with
 * sqlite3_config(). Once it returns, the previous provider is no longer running and will not
 * be called again, so its context may be freed.
 */
ZSQL_API void zsql_set_keellock_provider(zsql_key_provider provider, void* ctx);

/**
 * Open a database and key it.
 *
 * Works like sqlite3_open_v2(), then keys the "main" database with zsql_key_db() when @p key
 * asks for one. Unlike sqlite3_open_v2(), on failure the connection is closed and *@p db is
 * NULL; use zsql_errstr() for the message.
 *
 * @param key NULL or ZSQL_KEY_NONE opens without a key, exactly as sqlite3_open_v2() does.
 */
ZSQL_API int zsql_open(const char* filename, sqlite3** db, int flags, const char* vfs,
                       const zsql_key* key);

/**
 * Key one database of an open connection, then check that the key reads it.
 *
 * Selects the pinned SQLCipher v4 scheme for @p db, hands the key to sqlite3_key_v2() and reads
 * the schema. A file that is busy at that moment is accepted unread: SQLite3MC still writes
 * nothing in plaintext once a key is set. Does not migrate a plaintext database; zsql_open()
 * does that.
 *
 * @param schema "main", or the name of an attached database.
 * @return SQLITE_OK, ZSQL_E_WRONGKEY, ZSQL_E_PLAINTEXT, or another error.
 */
ZSQL_API int zsql_key_db(sqlite3* db, const char* schema, const zsql_key* key);

/**
 * Parse a key mode name: "none", "default", "passphrase" or "keellock" (any case).
 * @return SQLITE_OK, or ZSQL_E_BADKEY for any other name.
 */
ZSQL_API int zsql_key_mode_from_name(const char* name, zsql_key_mode* mode);

/** What a file holds, judged from its first 16 bytes only. */
typedef enum zsql_file_kind {
    ZSQL_FILE_MISSING = 0,   /**< It does not exist or cannot be read. */
    ZSQL_FILE_EMPTY = 1,     /**< It is empty, so any key will do for it. */
    ZSQL_FILE_PLAINTEXT = 2, /**< It starts with "SQLite format 3". */
    ZSQL_FILE_OTHER = 3      /**< Anything else: an encrypted database, or not a database. */
} zsql_file_kind;

/** Classify the file at @p filename (UTF-8, as SQLite takes it). */
ZSQL_API zsql_file_kind zsql_file_kind_of(const char* filename);

/** English message for any SQLite or libzsql result code. */
ZSQL_API const char* zsql_errstr(int rc);

/** "libzsql 1.0.0 (SQLite3 Multiple Ciphers x.y.z, SQLite a.b.c)" of the compiled library. */
ZSQL_API const char* zsql_version(void);

/**
 * Whether this header and the linked library come from the same SQLite3MC and SQLite release.
 * The header lives in zd-cxxproj and the amalgamation in libzsql, so the two can drift apart.
 */
ZSQL_INLINE int zsql_header_matches_library(void) {
    return strcmp(sqlite3_sourceid(), SQLITE_SOURCE_ID) == 0 &&
           strcmp(sqlite3mc_version(), SQLITE3MC_VERSION_STRING) == 0;
}

#ifdef __cplusplus
}
#endif
