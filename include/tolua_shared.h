#ifndef _TOLUA_SHARED_H_
#define _TOLUA_SHARED_H_

#include <luajit-2.0/lua.hpp>

#include <tr1/memory>

int tolua_sharedcol(lua_State* L);
void tolua_pushsharedusertype(lua_State* L, const std::tr1::shared_ptr<void>& ptr, const char* name);
std::tr1::shared_ptr<void> tolua_tosharedusertype(lua_State* L, int index, const char* name);
void tolua_removesharedusertype(lua_State* L, const std::tr1::shared_ptr<void>& ptr, const char* name);
/*
template<typename T>
void tolua_pushsharedusertype(lua_State* L, const std::tr1::shared_ptr<T>& ptr, const char* name) {
	tolua_pushsharedusertype(L, std::tr1::static_pointer_cast<void>(ptr), name);
}

template<typename T>
std::tr1::shared_ptr<T> tolua_tosharedusertype(lua_State* L, int index, const char* name) {
	return std::tr1::static_pointer_cast<T>(tolua_tosharedusertype(L, index, name));
}

template<typename T>
void tolua_removesharedusertype(lua_State* L, const std::tr1::shared_ptr<T>& ptr, const char* name) {
	tolua_removesharedusertype(L, std::tr1::static_pointer_cast<void>(ptr), name);
}
*/
#endif // _TOLUA_SHARED_H_
