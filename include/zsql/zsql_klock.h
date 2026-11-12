#pragma once
// ***************************************************************
//  zsql_klock   version:  1.0   -  date:  2026/11/12
//  -------------------------------------------------------------
//  ZSQL_KEY_KEELLOCK keys from a KeelLock dongle (library zsql_klock)
//  -------------------------------------------------------------
//  This file is a part of project zce.
//  Copyright (C) 2026 - All Rights Reserved
// ***************************************************************

/**
 * @file zsql_klock.h
 * @brief Serve ZSQL_KEY_KEELLOCK database keys from the feature keys a KeelLock dongle releases.
 *
 * Only a program that uses KeelLock mode links zsql_klock (and with it klock); libzsql and
 * libzce do not. The program unlocks the dongle the way it already does for licensing, but with
 * the request from dataAccessRequest(), and installs the result:
 *
 * @code
 *   klock::HostGuard guard(alg, kRootPub);
 *   auto keys = guard.verifyAndUnlock(dongle, cert, license,
 *                                     zsql::keellock::dataAccessRequest(kProductId, kDataFeature, now));
 *   if (!keys || !zsql::keellock::installProvider(*keys, kDataFeature)) { ... no data access ... }
 *   // zdb: "app.db?keymode=keellock;keylabel=main"
 * @endcode
 *
 * What a database key depends on, and so what must stay the same for the data to stay readable:
 * the product's DEK, the feature bit, the cipher suite of the license (P-256 and SM2 licenses
 * give different keys) and kDataKeyContext. A renewed license, a new seq or epoch, or a
 * replacement dongle for the same product keep the keys. Changing the DEK or the suite means
 * rekeying the databases while the old keys are still available. The vendor can always
 * recompute a key from the DEK (klock::crypto::deriveFeatureKeyForContext() and
 * deriveDatabaseKey()), which is the recovery path for a lost dongle.
 */

#include <cstdint>
#include <string_view>

#include "klock/guard.h"
#include "zsql/zsql.h"

namespace zsql {
namespace keellock {

/**
 * AccessRequest::key_context of the unlock that yields database keys. It is fixed for the life
 * of the data: a key context that changes with the program version, as code protection uses,
 * would lock out every database written by the previous version.
 */
inline constexpr std::string_view kDataKeyContext = "zsql-data-v1";

/**
 * The AccessRequest for the data keys.
 * @param feature_bit the one license feature bit (0x1, 0x2, 0x4, ...) whose key encrypts
 *        databases; a license without it cannot open them.
 */
klock::AccessRequest dataAccessRequest(uint32_t product_id, uint32_t feature_bit, uint64_t now_epoch);

/**
 * Key of the database labelled @p label: HKDF-Expand (SHA-256) with the feature key as PRK and
 * "zsql-db-v1" || label as info, 32 bytes. Pinned by test vectors in libzsql's gtest: changing
 * it makes every KeelLock database unreadable.
 * @throws klock::CryptoError if @p feature_key is not 32 bytes.
 */
klock::SecureBytes deriveDatabaseKey(const klock::SecureBytes& feature_key, std::string_view label);

/**
 * Install the libzsql provider of ZSQL_KEY_KEELLOCK keys, serving deriveDatabaseKey() of the
 * key of @p feature_bit in @p keys. The key is copied; @p keys may be cleared afterwards.
 * @return false, installing nothing, when @p feature_bit is not a single bit or @p keys has no
 *         key for it (the license does not grant it).
 */
bool installProvider(const klock::FeatureKeys& keys, uint32_t feature_bit);

/** Remove the provider and wipe the copied key. Databases opened before stay usable. */
void uninstallProvider();

}  // namespace keellock
}  // namespace zsql
