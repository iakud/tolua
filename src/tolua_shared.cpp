#include "tolua_shared.h"

int tolua_sharedcol(lua_State* L) {
	if (!lua_isuserdata(L, -1)) {
		return 0;
	}
	std::tr1::shared_ptr<void>* p = *reinterpret_cast<std::tr1::shared_ptr<void>**>(lua_touserdata(L, -1));
	if (p) {
		delete p;
	}
	return 0;
}

void tolua_pushsharedusertype(lua_State* L, const std::tr1::shared_ptr<void>& ptr, const char* name) {
	if (ptr == NULL) {
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

	lua_pushlightuserdata(L, ptr.get());
	lua_rawget(L, -2);

	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_pushlightuserdata(L, ptr.get());
		*reinterpret_cast<void**>(lua_newuserdata(L,sizeof(void*))) = new std::tr1::shared_ptr<void>(ptr);
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

std::tr1::shared_ptr<void> tolua_tosharedusertype(lua_State* L, int index, const char* name) {
	if (!lua_isuserdata(L, index)) {
		return std::tr1::shared_ptr<void>();
	}

	if (lua_getmetatable(L, index)) {
		luaL_getmetatable(L, name);
		if (lua_isnil(L, -1)) {
			lua_pop(L, 2);
			return std::tr1::shared_ptr<void>();
		}
		if (lua_rawequal(L, -1, -2)) {
			lua_pop(L, 2);
			std::tr1::shared_ptr<void>* p = *reinterpret_cast<std::tr1::shared_ptr<void>**>(lua_touserdata(L, index));
			if (p) {
				return *p;
			}
			return std::tr1::shared_ptr<void>();
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
				std::tr1::shared_ptr<void>* p = *reinterpret_cast<std::tr1::shared_ptr<void>**>(lua_touserdata(L, index));
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

void tolua_removesharedusertype(lua_State* L, const std::tr1::shared_ptr<void>& ptr, const char* name) {
	if (ptr == NULL) {
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