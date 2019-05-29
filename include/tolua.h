#ifndef _TOLUA_H_
#define _TOLUA_H_

#include <luajit-2.1/lua.hpp>

void tolua_open(lua_State* L);

void tolua_module(lua_State* L, const char* name);
void tolua_beginmodule(lua_State* L, const char* name);
void tolua_endmodule(lua_State* L);
void tolua_function(lua_State* L, const char* name, lua_CFunction func);

void tolua_usertype(lua_State* L, const char* name, lua_CFunction col = 0);
void tolua_class(lua_State* L, const char* lname, const char* name, const char* base);

void tolua_beginusertype(lua_State* L, const char* name);
void tolua_endusertype(lua_State* L);

bool tolua_isusertable(lua_State* L, int index, const char* name);
void tolua_pushusertype(lua_State* L, void* p, const char* name);
void* tolua_tousertype(lua_State* L, int index, const char* name);
void tolua_removeusertype(lua_State* L, void* p, const char* name);

#endif // _TOLUA_H_