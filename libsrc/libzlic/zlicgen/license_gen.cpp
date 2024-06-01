#include <zce/zce_config.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <zce/zce_dblock.h>
#include <zce/zce_rsa.h>
#include <zce/zce_string.h>
#include <zce/zce_mbpool.h>
#include <zce/zce_filesystem.h>
#include <zce/zce_hash.h>
#include "license.h"
#include "license_proto.h"
#include "license_pack.h"

#ifdef _WIN32
#   include <windows.h>
#   include <direct.h>
#else
#	include <unistd.h>
#	include <dirent.h>
#   define MAX_PATH 256
#   define TRUE  1
#   define FALSE 0
#endif

const char _lib_privkey[] = "-----BEGIN RSA PRIVATE KEY-----\n"
"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\n"
"-----END RSA PRIVATE KEY-----";

static char _lib_rsapub[] = "-----BEGIN RSA PUBLIC KEY-----\n"
"MIIBigKCAYEAzYi25hUYlS5eMgQXa5tMZj5Nc8Z5RBcr1Fc0sFWQkS5HKP4DJYhN\n"
"281fBEMR3GZMUeWbmS4NYI9bzRMXO31Fjq9dNmBwyVCtIhG+P0l8n1RbyI+IQyln\n"
"jWzAJ5oPg7FfrHRLxh0QJCrbZa8AdKaJGkLwebHOc70TZUb5/ECVS0m/DPUSM7iM\n"
"KTBS3e6V8jaKY0c3vlhwQYTQTPT9fRNPpEtP6KX+a7u3MBgf7v5FXcidn4Xr6MOh\n"
"CietDZK8zrUNEIWNg4etLf7ujY0kxtr56qgiuXSgKAr2QQKsti8XAqbULnCZ4g29\n"
"sAM/P1wLCpUAN8sdWC1fG0n6Fjkbl1ka+fRkgQumbIaHfcDbk8zhk7u8VCFgDO85\n"
"Edu5aDhI8OQ7H+JSaCkDBTIqxV8Q4EHr5XMBK9Qndgt196NeAnnN8vKZbxTzn9Ch\n"
"wu2hhJap9KpubOOzsmEXr3OkRcm1ZTKrFhoHctSFvOhlCEhS2BmGjGdC3Ay1l3GA\n"
"vKUVLAX7jGuXAgMBAAE=\n"
"-----END RSA PUBLIC KEY-----";

static const char _self_pubkey[] = "-----BEGIN RSA PUBLIC KEY-----\n"
"MIIBigKCAYEAp075J1NpkDSNv7EXy/ikP+bGP4cWTg6K2ub45y04Bl9PdSVxC+bn\n"
"cRyeL15l+L8+GrRpu/uur3mgg9WszRuHOJ2LIlTY+LMYmUPhtUpJPpDBI+Q6Yqhk\n"
"TMoHVr/RgXwy7XEZMdUOzHvfFGGj8VizTFPmzQmAxFBhbzk7563FDVWdKDo3E14U\n"
"wjKq9r37bOe1FxeVvYCK2J3JZ40hXwwmZyb4ug5PrPQLJsCGBvdD4PGF/SFxEu27\n"
"QRphwK/+VUejKwX+PP0PJAhakFJzXE2xzjCc/iavvIAxRN5IT3Nshc4fEJE3hSsV\n"
"s+MTYiIyAggdGMplsWnYXYy9HpLj6/oGnkb1bizrrfJEJM8S96jm0oZsN+3t3DfZ\n"
"kqSOU4K6sutbuCtnMB4bXlnoj/ktQIyu3PpPI7Za4njJMQ/v3FkQ3FwUVdoUGfgC\n"
"28pj4gMv4iqFBk4rivSA/+6uto4DQrok4jLfusnSZCXxQZ6Y1G2LyMGR1qJW4emd\n"
"Z4rkZ8jjX54NAgMBAAE=\n"
"-----END RSA PUBLIC KEY-----";

//license 信息
static zlicense::license_t g_lic;

//机器本身的信息
std::set<std::string> g_setmac;
std::set<std::string> g_sethd;

static bool machine_check(const zlicense::license_t& lic)
{
    int mac_match = 0, harddrive_match = 0;
    for (auto& mac : lic.vecmac) {
        if (g_setmac.find(mac) != g_setmac.end()) {
            ++mac_match;
        }
    };

    for (auto& hdrive : lic.vechd) {
        if (g_sethd.find(hdrive) != g_sethd.end()) {
            ++harddrive_match;
        }
    }
    if (mac_match != 0 && mac_match == lic.vecmac.size())
        return true;
    if (harddrive_match != 0 && harddrive_match == lic.vechd.size())
        return true;
    if (mac_match * 2 >= lic.vecmac.size() && harddrive_match * 2 >= lic.vechd.size())
        return true;
    return false;
}

static int _lic_from_base64(zlicense::license_t& lic, const zce_dblock& b64, const char* priv, int privlen)
{
    zce_dblock dblock, mb;
    ZCE_MBACQUIRE(mb, b64.length() * 2);
    int ret = zce_base64_decode(mb.rd_ptr(), b64.rd_ptr(), b64.length());
    ZCE_ASSERT_RETURN(ret >= 0, ZLIC_ERROR_CORRUPT);
    mb.wr_ptr(ret);
    ret = zce_rsa_private_decrypt(dblock, mb.rd_ptr(), mb.length(), priv, privlen);
    ZCE_ASSERT_RETURN(ret >= 0, ZLIC_ERROR_CORRUPT);
    ret = zdp::zds_unpack(lic, dblock.rd_ptr(), dblock.length(), nullptr, false);
    ZCE_ASSERT_RETURN(ret >= 0, ZLIC_ERROR_CORRUPT);
    return ret;
}

static int _lic_ok(const char* appname, const zlicense::license_t& lic)
{
    if (lic.app != appname) {
        return ZLIC_ERROR_PRODUCT;
    }
    if (lic.expire < time(nullptr)) {
        return ZLIC_ERROR_EXPIRED;
    }
    if (!machine_check(g_lic)) {
        return ZLIC_ERROR_MACHINE;
    }
    return 0;
}

static const char _self_privkey[] = "-----BEGIN RSA PRIVATE KEY-----\n"
"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\n"
"-----END RSA PRIVATE KEY-----";

const char version[] = "Standard Edition"; //trival
const char appname[] = "RealGoDB";

const  std::string zlicgen_createlicense(const char* appname, const zce_dblock& mb)
{
    std::string s_license_gen;
    int ret = 0;
    zlicense::license_t lic{};
    ret = _lic_from_base64(lic, mb, _self_privkey, sizeof(_self_privkey) - 1);
    ZCE_ASSERT_RETURN(ret >= 0, "");
    lic.expire = ~0; //永不过期
    lic.nv.clear();
    lic.nv.push_back({ "version", std::vector<zce_byte>() });
    lic.nv.back().value.resize(sizeof(version) - 1);
    memcpy(lic.nv.back().value.data(), version, sizeof(version) - 1);

    ret = zdp::zds_pack(0, 0, lic, 0, false);
    ZCE_ASSERT_RETURN(ret >= 0, s_license_gen);

    std::unique_ptr<zce_byte> buf(new zce_byte[ret]);
    ret = zdp::zds_pack(buf.get(), ret, lic, 0, false);
    ZCE_ASSERT_RETURN(ret >= 0, s_license_gen);

    //rsa encrypt
    zce_dblock dblock, output;
    ret = zce_rsa_public_encrypt(dblock, buf.get(), ret, _lib_rsapub, sizeof(_lib_rsapub) - 1);
    ZCE_ASSERT_RETURN(ret >= 0, s_license_gen.c_str());
    ZCE_MBACQUIRE(output, dblock.length() * 2);
    ret = zce_base64_encode(output.rd_ptr(), dblock.rd_ptr(), dblock.length());
    s_license_gen = std::string((const char*)output.rd_ptr(), ret);
    return s_license_gen;
}

#pragma comment(lib, "libzce.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "UserEnv.lib")
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "dbghelp.lib")
#include <zce/zce_filesystem.h>

int main(int argc, const char* argv[])
{
    zce_dblock dblock;
    zce_dblock_from_file(dblock, "RealGoDB.req", 0);
    auto str = zlicgen_createlicense("RealGoDB", dblock);

    handle_t hd = zce_open("RealGoDB.zlic", ZCE_OPEN_CRWFLAG, 0, 0);
    ZCE_ASSERT_RETURN(hd != ZCE_INVALID_HANDLE, 0);
    auto size = zce_write(hd, str.c_str(), str.length());
    zce_close(hd);
    ZCE_ASSERT_RETURN(size == str.length(), 0);
    return 0;
}