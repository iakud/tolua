#ifndef _LUAFIX_H_
#define _LUAFIX_H_

#include <luajit-2.0/lua.hpp>

#include <tr1/memory>

void luafix_open(lua_State* L);

void luafix_module(lua_State* L, const char* name);
void luafix_beginmodule(lua_State* L, const char* name);
void luafix_endmodule(lua_State* L);
void luafix_function(lua_State* L, const char* name, lua_CFunction func);
// usertype
void luafix_usertype(lua_State* L, const char* type, lua_CFunction col = 0);
void luafix_beginusertype(lua_State* L, const char* type);
void luafix_endusertype(lua_State* L);
void luafix_extends(lua_State* L, const char* type, const char* base);
void luafix_usertable(lua_State* L, const char* name, const char* type);
bool luafix_isusertable(lua_State* L, int index, const char* type);

void luafix_pushusertype(lua_State* L, void* p, const char* type);
void* luafix_tousertype(lua_State* L, int index, const char* type);
void luafix_removeusertype(lua_State* L, void* p, const char* type);
// shared usertype
void luafix_sharedusertype(lua_State* L, const char* type);
void luafix_beginsharedusertype(lua_State* L, const char* type);
void luafix_endsharedusertype(lua_State* L);
void luafix_sharedextends(lua_State* L, const char* type, const char* base);
void luafix_sharedusertable(lua_State* L, const char* name, const char* type);
bool luafix_issharedusertable(lua_State* L, int index, const char* type);

void luafix_pushsharedusertype(lua_State* L, const std::tr1::shared_ptr<void>& ptr, const char* type);
std::tr1::shared_ptr<void> luafix_tosharedusertype(lua_State* L, int index, const char* type);
void luafix_removesharedusertype(lua_State* L, const std::tr1::shared_ptr<void>& ptr, const char* type);

template<typename T>
void luafix_pushsharedusertype(lua_State* L, const std::tr1::shared_ptr<T>& ptr, const char* type) {
	luafix_pushsharedusertype(L, std::tr1::static_pointer_cast<void>(ptr), type);
}

template<typename T>
std::tr1::shared_ptr<T> luafix_tosharedusertype(lua_State* L, int index, const char* type) {
	return std::tr1::static_pointer_cast<T>(luafix_tosharedusertype(L, index, type));
}

template<typename T>
void luafix_removesharedusertype(lua_State* L, const std::tr1::shared_ptr<T>& ptr, const char* type) {
	luafix_removesharedusertype(L, std::tr1::static_pointer_cast<void>(ptr), type);
}

#endif // _LUAFIX_H_