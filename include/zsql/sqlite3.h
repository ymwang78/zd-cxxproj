#pragma once
/**
 * @file zsql/sqlite3.h
 * @brief The SQLite API of libzsql: SQLite plus the encryption API of SQLite3 Multiple Ciphers
 *        (sqlite3_key(), sqlite3_rekey(), sqlite3mc_*).
 *
 * Include this instead of any other sqlite3.h. The declarations are the upstream
 * sqlite3mc_amalgamation.h, kept byte for byte so that it can be compared with and upgraded
 * from the release; libsrc/libzsql/vendor/sqlite3mc/README.md records which one. The key modes
 * of libzsql are in zsql/zsql.h.
 */

#include "sqlite3mc_amalgamation.h"
