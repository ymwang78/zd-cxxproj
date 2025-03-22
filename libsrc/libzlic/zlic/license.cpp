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
#include <zce/zce_api.h>
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


bool SetRegistryValue(HKEY hKey, LPCSTR subKey, LPCSTR valueName, LPCSTR data) {
    HKEY hSubKey;
    // 打开（或创建）一个注册表键
    if (RegCreateKeyExA(hKey, subKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hSubKey, NULL) != ERROR_SUCCESS) {
        return false;
    }

    // 设置注册表键值
    if (RegSetValueExA(hSubKey, valueName, 0, REG_SZ, (const BYTE*)data, strlen(data) + 1) != ERROR_SUCCESS) {
        RegCloseKey(hSubKey);
        return false;
    }

    RegCloseKey(hSubKey);
    return true;
}

std::string GetRegistryValue(HKEY hKey, LPCSTR subKey, LPCSTR valueName) {
    HKEY hSubKey;
    char data[1024];
    DWORD dataSize = sizeof(data);

    if (RegOpenKeyExA(hKey, subKey, 0, KEY_READ, &hSubKey) != ERROR_SUCCESS) {
        return "";
    }

    if (RegQueryValueExA(hSubKey, valueName, NULL, NULL, (LPBYTE)data, &dataSize) != ERROR_SUCCESS) {
        RegCloseKey(hSubKey);
        return "";
    }

    RegCloseKey(hSubKey);
    return std::string(data, dataSize - 1);
}
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
    int ret = zce_base64_decode(mb.rd_ptr_cow(), b64.rd_ptr(), b64.length());
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
    if ((unsigned)lic.expire < time(nullptr)) {
        return ZLIC_ERROR_EXPIRED;
    }
    if (!machine_check(g_lic)) {
        return ZLIC_ERROR_MACHINE;
    }
    return 0;
}

static void _push_machine(zlicense::license_t& lic)
{
    for (auto& mac : g_setmac) {
        if (mac != "00-00-00-00-00-00" && !zce::start_with(mac, "00"))
            lic.vecmac.push_back(mac);
    }
    if (lic.vecmac.empty()) {
        for (auto& mac : g_setmac) {
            if (mac != "00-00-00-00-00-00")
                lic.vecmac.push_back(mac);
        }
    }
    for (auto& hdrive : g_sethd) {
        lic.vechd.push_back(hdrive);
    }
}

static std::string _create_trivallicense(const char* appname, int expire_trival)
{
    std::string s_license_gen;
    int ret = 0;
    zlicense::license_t lic{};
    lic.app = appname;
    lic.expire = 24 * 3600 * (int)(1 + time(nullptr) / (24*3600)) + expire_trival;
    lic.nv.push_back({ "version", std::vector<zce_byte>()});
    lic.nv.back().value.resize(sizeof("trival") - 1);
    memcpy(lic.nv.back().value.data(), "trival", sizeof("trival") - 1);
    _push_machine(lic);

    ret = zdp::zds_pack(0, 0, lic, 0, false);
    ZCE_ASSERT_RETURN(ret >= 0, s_license_gen);

    std::unique_ptr<zce_byte> buf(new zce_byte[ret]);
    ret = zdp::zds_pack(buf.get(), ret, lic, 0, false);
    ZCE_ASSERT_RETURN(ret >= 0, s_license_gen);

    //rsa encrypt
    zce_dblock dblock, output;
    ret = zce_rsa_public_encrypt(dblock, buf.get(), ret, _lib_rsapub, sizeof(_lib_rsapub) - 1);
    ZCE_ASSERT_RETURN(ret >= 0, s_license_gen);
    ZCE_MBACQUIRE(output, dblock.length() * 2);
    ret = zce_base64_encode(output.rd_ptr_cow(), dblock.rd_ptr(), dblock.length());
    s_license_gen = std::string((const char*)output.rd_ptr(), ret);
    return s_license_gen;
}

static zce_byte* _lic_fetch(const zlicense::license_t& lic, const char* property_name, int* nlen)
{
    if (strcmp(property_name, "expire") == 0) {
        *nlen = sizeof(lic.expire);
        return (zce_byte*)&lic.expire;
    }
    for (auto& v : lic.nv) {
        if (v.name == property_name) {
            *nlen = v.value.size();
            return (zce_byte*)v.value.data();
        }
    }
    *nlen = 0;
    return nullptr;
}

static inline void _machine_init()
{
    if (g_setmac.empty() && g_sethd.empty()) {
        g_setmac = zce_get_mac();
        g_sethd = zce_get_hardrive();
    }
}

static std::string s_licreq;

const char* zlic_getreq(const char* appname)
{
    _machine_init();
    s_licreq = "";
    zlicense::license_t lic{};
    lic.app = appname;
    for (auto& mac : g_setmac) {
        if (mac != "00-00-00-00-00-00" && !zce::start_with(mac, "00"))
            lic.vecmac.push_back(mac);
    }
    if (lic.vecmac.empty()) {
        for (auto& mac : g_setmac) {
            if (mac != "00-00-00-00-00-00")
                lic.vecmac.push_back(mac);
        }
    }
    for (auto& hdrive : g_sethd) {
        lic.vechd.push_back(hdrive);
    }
    int ret = zdp::zds_pack(0, 0, lic, 0, false);
    ZCE_ASSERT_RETURN(ret >= 0, s_licreq.c_str());

    std::unique_ptr<zce_byte> buf(new zce_byte[ret]);
    ret = zdp::zds_pack(buf.get(), ret, lic, 0, false);
    ZCE_ASSERT_RETURN(ret >= 0, s_licreq.c_str());

    //rsa encrypt
    zce_dblock dblock, output;
    ret = zce_rsa_public_encrypt(dblock, buf.get(), ret, _self_pubkey, sizeof(_self_pubkey) - 1);
    ZCE_ASSERT_RETURN(ret >= 0, s_licreq.c_str());
    ZCE_MBACQUIRE(output, dblock.length() * 2);
    ret = zce_base64_encode(output.rd_ptr_cow(), dblock.rd_ptr(), dblock.length());
    s_licreq = std::string((const char*)output.rd_ptr(), ret);
    std::string subKey = std::string("SOFTWARE\\Microsoft\\UserData\\ZLIC\\") + appname;
    SetRegistryValue(HKEY_CURRENT_USER, subKey.c_str(), "Request", s_licreq.c_str());

    handle_t hd = zce_open("RealGoDB.req", ZCE_OPEN_CRWFLAG, 0, 0);
    ZCE_ASSERT_RETURN(hd != ZCE_INVALID_HANDLE, 0);
    auto size = zce_write(hd, s_licreq.c_str(), s_licreq.length());
    zce_close(hd);

    return s_licreq.c_str();
}

int zlic_setkey(const char* appname, const char* keystr)
{
    _machine_init();
    int ret = 0;
    size_t keylen = strlen(keystr);
    zce_dblock mb(keylen, (zce_byte*)keystr, (int)keylen);

    std::string subKey = std::string("SOFTWARE\\Microsoft\\UserData\\ZLIC\\") + appname;
    SetRegistryValue(HKEY_CURRENT_USER, subKey.c_str(), "Response", keystr);

    //考虑到有的机器复制文件非常麻烦，所以使用手动输入license，那就必须非常的短
    if (mb.length() < 64) {
        //std::string val = GetRegistryValue(HKEY_CURRENT_USER, subKey.c_str(), "Request");
        //zce_dblock mb(val.length(), (zce_byte*)val.c_str(), (int)val.length());
        //zlicense::license_t lic{};
        //ret = _lic_from_base64(lic, mb, _lib_privkey, sizeof(_lib_privkey) - 1);
        //ZCE_ASSERT_RETURN(ret >= 0, ret);
        return -1;
    }
    else {
        zlicense::license_t lic{};
        ret = _lic_from_base64(lic, mb, _lib_privkey, sizeof(_lib_privkey) - 1);
        ZCE_ASSERT_RETURN(ret >= 0, ret);
        ret = _lic_ok(appname, lic);
        ZCE_ASSERT_RETURN(ret >= 0, ret);
        g_lic = lic;

        handle_t hd = zce_open((std::string(appname) + ".zlic").c_str(), ZCE_OPEN_CRWFLAG, 0, 0);
        ZCE_ASSERT_RETURN(hd != ZCE_INVALID_HANDLE, 0);
        auto size = zce_write(hd, keystr, keylen);
        zce_close(hd);
        ZCE_ASSERT_RETURN(size == keylen, 0);
        return 0;
    }
}

ZLIC_API int  zlic_valid(const char* appname, int trival_expire)
{
    _machine_init();
    if (g_lic.app.empty()) {
        zce_dblock mb;
        int ret = zce_dblock_from_file(mb, (std::string(appname) + ".zlic").c_str(), 0);
        ZCE_ASSERT_RETURN(ret >= 0, ZLIC_ERROR_NOLICENSE);
        zlicense::license_t lic{};
        ret = _lic_from_base64(lic, mb, _lib_privkey, sizeof(_lib_privkey) - 1);
        ZCE_ASSERT_RETURN(ret >= 0, ret);
        g_lic = lic;
    }
    int ret = _lic_ok(appname, g_lic);
    if (ret < 0) {
        int nlen = 0;
        zce_byte* data = _lic_fetch(g_lic, "version", &nlen);
        if (nlen > 0 && data && 0 == memcmp(data, "trival", zce_min(nlen, (int)sizeof("trival") - 1))) {
            //先读取试用版license，如果没有则生成一个，但是如何防止删除试用版license，造成无限试用呢？
            std::string subKey = std::string("SOFTWARE\\Microsoft\\UserData\\ZLIC\\") + appname;
            std::string val = GetRegistryValue(HKEY_CURRENT_USER, subKey.c_str(), "Data");
            if (val.empty()) {
                val = _create_trivallicense(appname, trival_expire);
                SetRegistryValue(HKEY_CURRENT_USER, subKey.c_str(), "Data", val.c_str());
            }
            zce_dblock mb(val.length(), (zce_byte*)val.c_str(), (int)val.length());
            zlicense::license_t lic{};
            ret = _lic_from_base64(lic, mb, _lib_privkey, sizeof(_lib_privkey) - 1);
            ZCE_ASSERT_RETURN(ret >= 0, ret);
            ret = _lic_ok(appname, lic);
            if (ret < 0) {
                return ret;
            }
            g_lic = lic;
            return 0;
        }
    }
    return ret;
}

const unsigned char* zlic_fetch(const char* property_name, int* nLen)
{
    return _lic_fetch(g_lic, property_name, nLen);
}

