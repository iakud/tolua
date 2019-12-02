#ifndef _TOLUA_H_
#define _TOLUA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#ifdef __cplusplus
}
#endif

void tolua_open(lua_State* L);

void tolua_module(lua_State* L, const char* name);
void tolua_beginmodule(lua_State* L, const char* name);
void tolua_endmodule(lua_State* L);
void tolua_function(lua_State* L, const char* name, lua_CFunction func);

void tolua_usertype(lua_State* L, const char* name, lua_CFunction col);
void tolua_class(lua_State* L, const char* lname, const char* name, const char* base);

void tolua_beginusertype(lua_State* L, const char* name);
void tolua_endusertype(lua_State* L);

int tolua_isusertable(lua_State* L, int index, const char* name);
void tolua_pushusertype(lua_State* L, void* p, const char* name);
void* tolua_tousertype(lua_State* L, int index, const char* name);
void tolua_removeusertype(lua_State* L, void* p, const char* name);

#endif // _TOLUA_H_
