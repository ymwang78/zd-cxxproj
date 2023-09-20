#pragma once

#include <zce/text_stream.h>
class zce_dblock;
class zdb_stmt;

typedef struct lua_State lua_State;
struct lua_Debug;

#define LUA_TTABLE_SUBTYPE_ARRAY 0
#define LUA_TTABLE_SUBTYPE_TABLE 1

#define LUA_TNUMBER_SUBTYPE_BYTE 0
#define LUA_TNUMBER_SUBTYPE_SHORT 1
#define LUA_TNUMBER_SUBTYPE_INT 2
#define LUA_TNUMBER_SUBTYPE_BIGINT 3
#define LUA_TNUMBER_SUBTYPE_DOUBLE 4

std::string ZCE_API luas_dumpstack(lua_State *L, const char* fname);

std::string ZCE_API luas_funcname(lua_State *L, lua_Debug *ar);

//unlike lua_tolstring, this api won't modify data on stack, so safe in lua_next api
//if keywrap, integer will wrap like [1]
std::string ZCE_API luas_tostring(lua_State *L, int index, bool keywrap);

int ZCE_API luas_calclen(lua_State *L, int index);

int ZCE_API luas_pack(zce_byte* buf, zce_int32 size, lua_State *L, int index);

int ZCE_API luas_unpack(lua_State *L, const zce_byte* buf, zce_int32 size);

int ZCE_API luas_push_table(zce_dblock& dblock, bool isarray, int items);

int ZCE_API luas_pop_lstring(zce_dblock& dblock, const char*& str, size_t& len);

int ZCE_API luas_push_lstring(zce_dblock& dblock, const char*, zce_int32 len);

int ZCE_API luas_push_string(zce_dblock& dblock, const char*);

int ZCE_API luas_push_integer(zce_dblock& dblock, zce_int64 v);

int ZCE_API luas_push_bool(zce_dblock& dblock, bool v);

int ZCE_API luas_push_lightuserdata(zce_dblock& dblock, const void* v);

int ZCE_API luas_push_double(zce_dblock& dblock, double v);

int ZCE_API luas_push_nil(zce_dblock& dblock);

int ZCE_API luas_push_stringmap(zce_dblock& dblock, const std::map<std::string, std::string>& dict);

int ZCE_API luas_fix_table_items(zce_dblock& dblock, int wrpos, int items);

int ZCE_API luas_pack_stmt(zce_dblock& dblock, const zce_smartptr<zdb_stmt>& stmt_ptr, bool asarray, bool isempty);

int ZCE_API luas_to_dblock(zce_dblock& dblock, lua_State* L, int from, int to);

int ZCE_API luas_to_json(std::stringstream& ss, lua_State* L, int index, const char* name, int depth, int childidx);

int ZCE_API luas_to_stringmap(std::map<std::string, std::string>& dict, lua_State* L, int index);

class ZCE_API luas_ensure_stack
{
    lua_State* L_;

    int top_;

public:

    luas_ensure_stack(lua_State* L, int add = 0);

    ~luas_ensure_stack();

    inline void inc_top(int n) {
        top_ += n;
    }
};
