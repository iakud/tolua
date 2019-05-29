#include "tolua_function.h"

struct tolua_function_ref {
};

tolua_function_ref* tolua_ref_function(lua_State* L, int index) {
	if (!lua_isfunction(L, index))
		return NULL;

	lua_pushvalue(L, index);
	lua_getfield(L, LUA_REGISTRYINDEX, "tolua_function_ref");
	tolua_function_ref* func = new tolua_function_ref();
	lua_pushlightuserdata(L, func);
	lua_insert(L, -3);
	lua_insert(L, -3);
	lua_rawset(L, -3);
	lua_pop(L, 1);
	return func;
}

void tolua_push_function_by_ref(lua_State* L, tolua_function_ref* func) {
	if (func == NULL) {
		lua_pushnil(L);
		return;
	}
	lua_getfield(L, LUA_REGISTRYINDEX, "tolua_function_ref");
	lua_pushlightuserdata(L, func);
	lua_rawget(L, -2);
	lua_remove(L, -2);
}

void tolua_remove_function_by_ref(lua_State* L, tolua_function_ref* func) {
	if (func == NULL)
		return;
	lua_getfield(L, LUA_REGISTRYINDEX, "tolua_function_ref");
	lua_pushlightuserdata(L, func);
	lua_pushnil(L);
	lua_rawset(L, -3);
	lua_pop(L, 1);
	delete func;
}