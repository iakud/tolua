#include "luafix.h"

#define LUAFIX_USERTYPE "luafix_usertype"
#define LUAFIX_USERTABLE "luafix_usertable"

#define LUAFIX_SHAREDUSERTYPE "luafix_sharedusertype"
#define LUAFIX_SHAREDUSERTABLE "luafix_sharedusertable"

#define LUAFIX_SUPER "luafix_super"

#define LUAFIX_USERTYPE_MAPPING "luafix_usertype_mapping"
#define LUAFIX_FUNCTION_MAPPING "luafix_function_mapping"

void luafix_open(lua_State* L) {
	// usertype
	lua_newtable(L);
	lua_setfield(L, LUA_REGISTRYINDEX, LUAFIX_USERTYPE);
	lua_newtable(L);
	lua_setfield(L, LUA_REGISTRYINDEX, LUAFIX_USERTABLE);
	// sharedusertype
	lua_newtable(L);
	lua_setfield(L, LUA_REGISTRYINDEX, LUAFIX_SHAREDUSERTYPE);
	lua_newtable(L);
	lua_setfield(L, LUA_REGISTRYINDEX, LUAFIX_SHAREDUSERTABLE);
	// super
	lua_newtable(L);
	lua_setfield(L, LUA_REGISTRYINDEX, LUAFIX_SUPER);
	// usertype mapping
	lua_newtable(L);
	lua_createtable(L, 0, 1);
	lua_pushstring(L, "v");
	lua_setfield(L, -2, "__mode");
	lua_setmetatable(L, -2);
	lua_setfield(L, LUA_REGISTRYINDEX, LUAFIX_USERTYPE_MAPPING);
	// function mapping
	lua_newtable(L);
	lua_setfield(L, LUA_REGISTRYINDEX, LUAFIX_FUNCTION_MAPPING);
}

void luafix_module(lua_State* L, const char* name) {
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

void luafix_beginmodule(lua_State* L, const char* name) {
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

void luafix_endmodule(lua_State* L) {
	lua_pop(L, 1);
}

void luafix_function(lua_State* L, const char* name, lua_CFunction func) {
	lua_pushstring(L,name);
	lua_pushcfunction(L,func);
	lua_rawset(L,-3);
}

void luafix_usertype(lua_State* L, const char* type, const char* name) {
	lua_getfield(L, LUA_REGISTRYINDEX, type);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}
	lua_getfield(L, -1, name);
	if (!lua_isnil(L, -1)) {
		lua_pop(L, 2);
		return;
	}
	lua_pop(L, 1);
	lua_newtable(L);
	lua_newtable(L);
	lua_createtable(L, 0, 1);
	lua_pushstring(L, "v");
	lua_setfield(L, -2, "__mode");
	lua_setmetatable(L, -2);
	lua_setfield(L, -2, LUAFIX_USERTYPE_MAPPING);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_setfield(L, -2, name);
	lua_pop(L, 1);
}

void luafix_getusertype(lua_State* L, const char* type, const char* name) {
	lua_getfield(L, LUA_REGISTRYINDEX, type);
	if (lua_isnil(L, -1)) {
		return;
	}
	lua_getfield(L, -1, name);
	lua_remove(L, -2);
}

void luafix_collector(lua_State* L, const char* type, const char* name, lua_CFunction col) {
	if (col) {
		luafix_getusertype(L, type, name);
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			return;
		}
		lua_pushcfunction(L, col);
		lua_setfield(L, -2, "__gc");
		lua_pop(L, 1);
	}
}

void luafix_inheritance(lua_State* L, const char* type, const char* name, const char* base) {
	luafix_getusertype(L, type, name);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}
	luafix_getusertype(L, type, base);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 2);
		return;
	}
	lua_setmetatable(L, -2);
	lua_pop(L, 1);
}

void luafix_super(lua_State* L, const char* type, const char* name, const char* base) {
	lua_getfield(L, LUA_REGISTRYINDEX, LUAFIX_SUPER);
	luafix_getusertype(L, type, base);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 2);
		return;
	}
	lua_rawget(L, -2);
	luafix_getusertype(L, type, name);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 3);
		return;
	}
	lua_rawget(L, -3);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
		luafix_getusertype(L, type, name);
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

void luafix_extends(lua_State* L, const char* type, const char* name, const char* base) {
	luafix_inheritance(L, type, name, base);
	luafix_super(L, type, name, base);
}

// usertype
void luafix_usertype(lua_State* L, const char* type, lua_CFunction col) {
	luafix_usertype(L, LUAFIX_USERTYPE, type);
	luafix_collector(L, LUAFIX_USERTYPE, type, col);
}

void luafix_beginusertype(lua_State* L, const char* type) {
	luafix_getusertype(L, LUAFIX_USERTYPE, type);
}

void luafix_endusertype(lua_State* L) {
	lua_pop(L, 1);
}

void luafix_extends(lua_State* L, const char* type, const char* base) {
	luafix_extends(L, LUAFIX_USERTYPE, type, base);
}

void luafix_usertable(lua_State* L, const char* name, const char* type) {
	lua_newtable(L);
	luafix_getusertype(L, LUAFIX_USERTYPE, type);
	lua_setmetatable(L, -2);
	lua_pushstring(L, name);
	lua_pushvalue(L, -2);
	lua_rawset(L, -4);

	lua_getfield(L, LUA_REGISTRYINDEX, LUAFIX_USERTABLE);
	lua_insert(L, -2);
	lua_pushboolean(L, 1);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

bool luafix_isusertable(lua_State* L, int index, const char* type) {
	lua_pushvalue(L, index);
	lua_getfield(L, LUA_REGISTRYINDEX, LUAFIX_USERTABLE);
	lua_insert(L, -2);
	lua_rawget(L, -2);
	if (!lua_isnil(L, -1)) {
		lua_pop(L, 2);
		if (lua_getmetatable(L, index)) {
			luafix_getusertype(L, LUAFIX_USERTYPE, type);
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

void luafix_pushusertype(lua_State* L, void* p, const char* type) {
	if (p == NULL) {
		lua_pushnil(L);
		return;
	}

	luafix_getusertype(L, LUAFIX_USERTYPE, type);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}

	lua_pushstring(L, LUAFIX_USERTYPE_MAPPING);
	lua_rawget(L,-2);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_getfield(L, LUA_REGISTRYINDEX, LUAFIX_USERTYPE_MAPPING);
	}

	lua_pushlightuserdata(L, p);
	lua_rawget(L, -2);

	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_pushlightuserdata(L, p);
		*(void**)lua_newuserdata(L, sizeof(void*)) = p;
		lua_pushvalue(L, -1);
		lua_insert(L, -5);
		lua_rawset(L, -3);
		lua_pop(L, 1);

		lua_setmetatable(L, -2);
	} else {
		lua_insert(L, -3);
		lua_pop(L, 1);
		lua_getfield(L, LUA_REGISTRYINDEX, LUAFIX_SUPER);
		lua_getmetatable(L, -3);
		lua_rawget(L, -2);
		if (lua_istable(L, -1)) {
			lua_pushstring(L, type);
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

void* luafix_tousertype(lua_State* L, int index, const char* type) {
	if (!lua_isuserdata(L, index)) {
		return NULL;
	}

	if (lua_getmetatable(L, index)) {
		luafix_getusertype(L, LUAFIX_USERTYPE, type);
		if (lua_isnil(L, -1)) {
			lua_pop(L, 2);
			return NULL;
		}
		if (lua_rawequal(L, -1, -2)) {
			lua_pop(L, 2);
			return *(void**)lua_touserdata(L, index);
		}
		lua_pop(L, 1);
		lua_getfield(L, LUA_REGISTRYINDEX, LUAFIX_SUPER);
		lua_insert(L, -2);
		lua_rawget(L, -2);
		if (lua_istable(L,-1)) {
			lua_pushstring(L, type);
			lua_rawget(L, -2);
			if (lua_toboolean(L, -1)) {
				lua_pop(L, 3);
				return *(void**)lua_touserdata(L, index);
			}
			lua_pop(L, 1);
		}
		lua_pop(L, 2);
	}
	return NULL;
}

void luafix_removeusertype(lua_State* L, void* p, const char* type) {
	if (p == NULL) {
		return;
	}

	luafix_getusertype(L, LUAFIX_USERTYPE, type);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}
	lua_pushstring(L, LUAFIX_USERTYPE_MAPPING);
	lua_rawget(L, -2);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_getfield(L, LUA_REGISTRYINDEX, LUAFIX_USERTYPE_MAPPING);
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

// shared usertype
int luafix_sharedcol(lua_State* L) {
	if (!lua_isuserdata(L, -1)) {
		return 0;
	}
	std::tr1::shared_ptr<void>* p = *(std::tr1::shared_ptr<void>**)lua_touserdata(L, -1);
	if (p) {
		delete p;
	}
	return 0;
}

void luafix_sharedusertype(lua_State* L, const char* type) {
	luafix_usertype(L, LUAFIX_SHAREDUSERTYPE, type);
	luafix_collector(L, LUAFIX_SHAREDUSERTYPE, type, luafix_sharedcol);
}

void luafix_beginsharedusertype(lua_State* L, const char* type) {
	luafix_getusertype(L, LUAFIX_SHAREDUSERTYPE, type);
}

void luafix_endsharedusertype(lua_State* L) {
	lua_pop(L, 1);
}

void luafix_sharedextends(lua_State* L, const char* type, const char* base) {
	luafix_extends(L, LUAFIX_SHAREDUSERTYPE, type, base);
}

void luafix_sharedusertable(lua_State* L, const char* name, const char* type) {
	lua_newtable(L);
	luafix_getusertype(L, LUAFIX_SHAREDUSERTYPE, type);
	lua_setmetatable(L, -2);
	lua_pushstring(L, name);
	lua_pushvalue(L, -2);
	lua_rawset(L, -4);

	lua_getfield(L, LUA_REGISTRYINDEX, LUAFIX_SHAREDUSERTABLE);
	lua_insert(L, -2);
	lua_pushboolean(L, 1);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

bool luafix_issharedusertable(lua_State* L, int index, const char* type) {
	lua_pushvalue(L, index);
	lua_getfield(L, LUA_REGISTRYINDEX, LUAFIX_SHAREDUSERTABLE);
	lua_insert(L, -2);
	lua_rawget(L, -2);
	if (!lua_isnil(L, -1)) {
		lua_pop(L, 2);
		if (lua_getmetatable(L, index)) {
			luafix_getusertype(L, LUAFIX_SHAREDUSERTYPE, type);
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

void luafix_pushsharedusertype(lua_State* L, const std::tr1::shared_ptr<void>& ptr, const char* type) {
	if (ptr == NULL) {
		lua_pushnil(L);
		return;
	}

	luafix_getusertype(L, LUAFIX_SHAREDUSERTYPE, type);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}
	lua_pushstring(L, LUAFIX_USERTYPE_MAPPING);
	lua_rawget(L,-2);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_getfield(L, LUA_REGISTRYINDEX, LUAFIX_USERTYPE_MAPPING);
	}

	lua_pushlightuserdata(L, ptr.get());
	lua_rawget(L, -2);

	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_pushlightuserdata(L, ptr.get());
		*(void**)lua_newuserdata(L,sizeof(void*)) = new std::tr1::shared_ptr<void>(ptr);
		lua_pushvalue(L, -1);
		lua_insert(L, -5);
		lua_rawset(L, -3);
		lua_pop(L, 1);

		lua_setmetatable(L, -2);
	}
	else
	{
		lua_insert(L, -3);
		lua_pop(L, 1);
		lua_getfield(L, LUA_REGISTRYINDEX, LUAFIX_SUPER);
		lua_getmetatable(L, -3);
		lua_rawget(L, -2);
		if (lua_istable(L, -1)) {
			lua_pushstring(L, type);
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

std::tr1::shared_ptr<void> luafix_tosharedusertype(lua_State* L, int index, const char* type) {
	if (!lua_isuserdata(L, index)) {
		return std::tr1::shared_ptr<void>();
	}

	if (lua_getmetatable(L, index)) {
		luafix_getusertype(L, LUAFIX_SHAREDUSERTYPE, type);
		if (lua_isnil(L, -1)) {
			lua_pop(L, 2);
			return std::tr1::shared_ptr<void>();
		}
		if (lua_rawequal(L, -1, -2)) {
			lua_pop(L, 2);
			std::tr1::shared_ptr<void>* p = *(std::tr1::shared_ptr<void>**)lua_touserdata(L, index);
			if (p) {
				return *p;
			}
			return std::tr1::shared_ptr<void>();
		}
		lua_pop(L, 1);
		lua_getfield(L, LUA_REGISTRYINDEX, LUAFIX_SUPER);
		lua_insert(L, -2);
		lua_rawget(L, -2);
		if (lua_istable(L,-1)) {
			lua_pushstring(L, type);
			lua_rawget(L, -2);
			if (lua_toboolean(L, -1)) {
				lua_pop(L, 3);
				std::tr1::shared_ptr<void>* p = *(std::tr1::shared_ptr<void>**)lua_touserdata(L, index);
				if (p) {
					return *p;
				}
				return std::tr1::shared_ptr<void>();
			}
			lua_pop(L, 1);
		}
		lua_pop(L, 2);
	}
	return std::tr1::shared_ptr<void>();
}

void luafix_removesharedusertype(lua_State* L, const std::tr1::shared_ptr<void>& ptr, const char* type) {
	if (ptr == NULL) {
		return;
	}

	luafix_getusertype(L, LUAFIX_SHAREDUSERTYPE, type);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}
	lua_pushstring(L, LUAFIX_USERTYPE_MAPPING);
	lua_rawget(L,-2);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_getfield(L, LUA_REGISTRYINDEX, LUAFIX_USERTYPE_MAPPING);
	}

	lua_pushlightuserdata(L, ptr.get());
	lua_rawget(L, -2);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 3);
		return;
	}

	lua_pop(L, 1);
	lua_pushlightuserdata(L, ptr.get());
	lua_pushnil(L);
	lua_rawset(L, -3);
	lua_pop(L, 2);
}