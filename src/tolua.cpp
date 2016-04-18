#include "tolua.h"

#include "tolua_event.h"

void tolua_open(lua_State* L) {
	// usertype
	lua_newtable(L);
	lua_setfield(L, LUA_REGISTRYINDEX, "tolua_usertable");
	// super
	lua_newtable(L);
	lua_setfield(L, LUA_REGISTRYINDEX, "tolua_super");
	// tolua_peers
	lua_newtable(L);
	lua_createtable(L, 0, 1);
	lua_pushstring(L, "k");
	lua_setfield(L, -2, "__mode");
	lua_setmetatable(L, -2);
	lua_setfield(L, LUA_REGISTRYINDEX, "tolua_peers");
	// usertype mapping
	lua_newtable(L);
	lua_createtable(L, 0, 1);
	lua_pushstring(L, "v");
	lua_setfield(L, -2, "__mode");
	lua_setmetatable(L, -2);
	lua_setfield(L, LUA_REGISTRYINDEX, "tolua_usertype_mapping");
	// function mapping
	lua_newtable(L);
	lua_setfield(L, LUA_REGISTRYINDEX, "tolua_function_ref");
}

void tolua_module(lua_State* L, const char* name) {
	if (name) {
		lua_pushstring(L, name);
		lua_rawget(L, -2);
		if (!lua_istable(L, -1)) {
			lua_pop(L, 1);
			lua_newtable(L);
			lua_pushstring(L, name);
			lua_pushvalue(L, -2);
			lua_rawset(L, -4);
		}
		lua_pop(L, 1);
	}
}

void tolua_beginmodule(lua_State* L, const char* name) {
	if (name) {
		lua_pushstring(L, name);
		lua_rawget(L, -2);
		if (lua_istable(L, -1)) {
			return;
		}
		lua_pop(L, 1);
	}
	lua_pushvalue(L, LUA_GLOBALSINDEX);
}

void tolua_endmodule(lua_State* L) {
	lua_pop(L, 1);
}

void tolua_function(lua_State* L, const char* name, lua_CFunction func) {
	lua_pushstring(L, name);
	lua_pushcfunction(L, func);
	lua_rawset(L, -3);
}

void tolua_usertype(lua_State* L, const char* name, lua_CFunction col/* = 0*/) {
	if (luaL_newmetatable(L,name)) {
		tolua_classevents(L);
		if (col) {
			tolua_collector(L, col);
		}
	}
	lua_pop(L, 1);
}

void tolua_inheritance(lua_State* L, const char* name, const char* base) {
	luaL_getmetatable(L, name);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}
	luaL_getmetatable(L, base);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 2);
		return;
	}
	lua_setmetatable(L, -2);
	lua_pop(L, 1);
}

void tolua_super(lua_State* L, const char* name, const char* base) {
	lua_getfield(L, LUA_REGISTRYINDEX, "tolua_super");
	luaL_getmetatable(L, base);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 2);
		return;
	}
	lua_rawget(L, -2);
	luaL_getmetatable(L, name);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 3);
		return;
	}
	lua_rawget(L, -3);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
		luaL_getmetatable(L, name);
		lua_pushvalue(L, -2);
		lua_rawset(L, -5);
	}
	lua_pushstring(L, base);
	lua_pushboolean(L, 1);
	lua_rawset(L, -3);
	lua_replace(L, -3);
	if (lua_istable(L, -1)) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			lua_pushvalue(L, -2);
			lua_insert(L, -2);
			lua_rawset(L, -5);
		}
	}
	lua_pop(L, 2);
}

void tolua_mapping(lua_State* L, const char* name) {
	luaL_getmetatable(L, name);
	lua_newtable(L);
	lua_createtable(L, 0, 1);
	lua_pushstring(L, "v");
	lua_setfield(L, -2, "__mode");
	lua_setmetatable(L, -2);
	lua_setfield(L, -2, "tolua_usertype_mapping");
	lua_pop(L, 1);
}

void tolua_usertable(lua_State* L, const char* lname, const char* name) {
	lua_newtable(L);
	luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);
	lua_pushstring(L, lname);
	lua_pushvalue(L, -2);
	lua_rawset(L, -4);

	lua_getfield(L, LUA_REGISTRYINDEX, "tolua_usertable");
	lua_insert(L, -2);
	lua_pushboolean(L, 1);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

void tolua_class(lua_State* L, const char* lname, const char* name, const char* base) {
	if (base && *base) {
		tolua_inheritance(L, name, base);
		tolua_super(L, name, base);
	}
	tolua_mapping(L, name);
	tolua_usertable(L, lname, name);
}

void tolua_beginusertype(lua_State* L, const char* name) {
	luaL_getmetatable(L, name);
}

void tolua_endusertype(lua_State* L) {
	lua_pop(L, 1);
}

bool tolua_isusertable(lua_State* L, int index, const char* name) {
	lua_pushvalue(L, index);
	lua_getfield(L, LUA_REGISTRYINDEX, "tolua_usertable");
	lua_insert(L, -2);
	lua_rawget(L, -2);
	if (!lua_isnil(L, -1)) {
		lua_pop(L, 2);
		if (lua_getmetatable(L, index)) {
			luaL_getmetatable(L, name);
			if (lua_rawequal(L, -1, -2)) {
				lua_pop(L, 2);
				return true;
			}
			lua_pop(L, 2);
		}
	} else {
		lua_pop(L, 2);
	}
	return false;
}

void tolua_pushusertype(lua_State* L, void* p, const char* name) {
	if (p == NULL) {
		lua_pushnil(L);
		return;
	}

	luaL_getmetatable(L, name);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}

	lua_pushstring(L, "tolua_usertype_mapping");
	lua_rawget(L,-2);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_getfield(L, LUA_REGISTRYINDEX, "tolua_usertype_mapping");
	}

	lua_pushlightuserdata(L, p);
	lua_rawget(L, -2);

	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_pushlightuserdata(L, p);
		*reinterpret_cast<void**>(lua_newuserdata(L, sizeof(void*))) = p;
		lua_pushvalue(L, -1);
		lua_insert(L, -5);
		lua_rawset(L, -3);
		lua_pop(L, 1);

		lua_setmetatable(L, -2);
	} else {
		lua_insert(L, -3);
		lua_pop(L, 1);
		lua_getfield(L, LUA_REGISTRYINDEX, "tolua_super");
		lua_getmetatable(L, -3);
		lua_rawget(L, -2);
		if (lua_istable(L, -1)) {
			lua_pushstring(L, name);
			lua_rawget(L, -2);
			if (lua_toboolean(L, -1) == 1) {
				lua_pop(L, 4);
				return;
			}
			lua_pop(L, 1);
		}
		lua_pop(L, 2);

		lua_setmetatable(L, -2);
	}
}

void* tolua_tousertype(lua_State* L, int index, const char* name) {
	if (!lua_isuserdata(L, index)) {
		return NULL;
	}

	if (lua_getmetatable(L, index)) {
		luaL_getmetatable(L, name);
		if (lua_isnil(L, -1)) {
			lua_pop(L, 2);
			return NULL;
		}
		if (lua_rawequal(L, -1, -2)) {
			lua_pop(L, 2);
			return *reinterpret_cast<void**>(lua_touserdata(L, index));
		}
		lua_pop(L, 1);
		lua_getfield(L, LUA_REGISTRYINDEX, "tolua_super");
		lua_insert(L, -2);
		lua_rawget(L, -2);
		if (lua_istable(L,-1)) {
			lua_pushstring(L, name);
			lua_rawget(L, -2);
			if (lua_toboolean(L, -1)) {
				lua_pop(L, 3);
				return *reinterpret_cast<void**>(lua_touserdata(L, index));
			}
			lua_pop(L, 1);
		}
		lua_pop(L, 2);
	}
	return NULL;
}

void tolua_removeusertype(lua_State* L, void* p, const char* name) {
	if (p == NULL) {
		return;
	}

	luaL_getmetatable(L, name);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}
	lua_pushstring(L, "tolua_usertype_mapping");
	lua_rawget(L, -2);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_getfield(L, LUA_REGISTRYINDEX, "tolua_usertype_mapping");
	}

	lua_pushlightuserdata(L, p);
	lua_rawget(L, -2);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 3);
		return;
	}

	lua_pop(L, 1);
	lua_pushlightuserdata(L, p);
	lua_pushnil(L);
	lua_rawset(L, -3);
	lua_pop(L, 2);
}