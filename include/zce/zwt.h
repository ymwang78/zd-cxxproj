#pragma once
#include <string>
#include <zce/zce_types.h>
#include <zce/zce_dblock.h>

int zwt_tron_wallet_from_privatekey(zce_dblock& wallet_dblock, const char* private_key_hex, int private_key_hex_len);

int zwt_tron_signhash(zce_dblock& dblock, const zce_byte* raw_hash, int raw_hash_len, const char* private_key_hex, int private_key_hex_len);