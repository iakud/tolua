#include "tolua_event.h"

static int class_index_event(lua_State* L) {
	int type = lua_type(L, 1);
	if (type == LUA_TUSERDATA) {
		lua_getfield(L, LUA_REGISTRYINDEX, "tolua_peers");
		lua_pushvalue(L, 1);
		lua_rawget(L, -2);
		if (lua_istable(L, -1)) {
			lua_pushvalue(L, 2);
			lua_rawget(L, -2);
			if (!lua_isnil(L, -1)) {
				return 1;
			}
		}

		lua_settop(L, 2);
		/* Try metatables */
		lua_pushvalue(L, 1);
		while (lua_getmetatable(L, -1)) {
			lua_remove(L, -2);
			lua_pushvalue(L, 2);
			lua_rawget(L, -2);
			if (!lua_isnil(L, -1)) {
				return 1;
			}
			lua_pop(L, 1);
		}
		lua_pushnil(L);
		return 1;
	} else if (type == LUA_TTABLE) {
		lua_pushvalue(L, 1);
		while (lua_getmetatable(L, -1)) {
			lua_remove(L, -2);
			lua_pushvalue(L, 2);
			lua_rawget(L, -2);
			if (!lua_isnil(L, -1)) {
				return 1;
			}
			lua_pop(L, 1);
		}
		lua_pushnil(L);
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

static int class_newindex_event(lua_State* L) {
	int type = lua_type(L,1);
	if (type == LUA_TUSERDATA) {
		lua_getfield(L, LUA_REGISTRYINDEX, "tolua_peers");
		lua_pushvalue(L, 1);
		lua_rawget(L, -2);
		if (!lua_istable(L, -1)) {
			lua_pop(L, 1);
			lua_newtable(L);
			lua_pushvalue(L, 1);
			lua_pushvalue(L, -2);
			lua_rawset(L, -4);
		}
		lua_replace(L, 1);
		lua_pop(L, 1);
		lua_rawset(L, 1);
	} else if (type == LUA_TTABLE) {
		lua_getmetatable(L, 1);
		lua_replace(L, 1);
		lua_rawset(L, 1);
	}
	return 0;
}

void tolua_classevents(lua_State* L) {
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, class_index_event);
	lua_rawset(L, -3);
	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, class_newindex_event);
	lua_rawset(L, -3);
}

void tolua_collector(lua_State* L, lua_CFunction col) {
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, col);
	lua_rawset(L, -3);
}