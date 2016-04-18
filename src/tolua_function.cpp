#include "tolua_function.h"

struct tolua_function_ref {
	//void* p;
};

tolua_function_ref* tolua_tofunction(lua_State* L, int index) {
	if (!lua_isfunction(L, index))
		return NULL;

	lua_pushvalue(L, index);
	lua_getfield(L, LUA_REGISTRYINDEX, "tolua_function_ref");
	tolua_function_ref* function_ref = new tolua_function_ref();
	lua_pushlightuserdata(L, function_ref);
	lua_insert(L, -3);
	lua_insert(L, -3);
	lua_rawset(L, -3);
	lua_pop(L, 1);
	return function_ref;
}

void tolua_pushfunction(lua_State* L, tolua_function_ref* function_ref) {
	if (function_ref == NULL) {
		lua_pushnil(L);
		return;
	}
	lua_getfield(L, LUA_REGISTRYINDEX, "tolua_function_ref");
	lua_pushlightuserdata(L, function_ref);
	lua_rawget(L, -2);
	lua_remove(L, -2);
}

void tolua_removefunction(lua_State* L, tolua_function_ref* function_ref) {
	if (function_ref == NULL)
		return;
	lua_getfield(L, LUA_REGISTRYINDEX, "tolua_function_ref");
	lua_pushlightuserdata(L, function_ref);
	lua_pushnil(L);
	lua_rawset(L, -3);
	lua_pop(L, 1);
	delete function_ref;
}