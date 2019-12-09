#include "tolua_function.h"
#include <stdlib.h>

struct tolua_FunctionRef {
};

struct tolua_FunctionRef* tolua_tofunction_ref(lua_State* L, int index) {
	if (!lua_isfunction(L, index))
		return NULL;

	lua_pushvalue(L, index);
	lua_getfield(L, LUA_REGISTRYINDEX, "tolua_function_ref");
	struct tolua_FunctionRef* func = (struct tolua_FunctionRef*)malloc(sizeof(struct tolua_FunctionRef));
	lua_pushlightuserdata(L, func);
	lua_insert(L, -3);
	lua_insert(L, -3);
	lua_rawset(L, -3);
	lua_pop(L, 1);
	return func;
}

void tolua_pushfunction_ref(lua_State* L, struct tolua_FunctionRef* func) {
	if (func == NULL) {
		lua_pushnil(L);
		return;
	}
	lua_getfield(L, LUA_REGISTRYINDEX, "tolua_function_ref");
	lua_pushlightuserdata(L, func);
	lua_rawget(L, -2);
	lua_remove(L, -2);
}

void tolua_removefunction_ref(lua_State* L, struct tolua_FunctionRef* func) {
	if (func == NULL)
		return;
	lua_getfield(L, LUA_REGISTRYINDEX, "tolua_function_ref");
	lua_pushlightuserdata(L, func);
	lua_pushnil(L);
	lua_rawset(L, -3);
	lua_pop(L, 1);
	free(func);
}
