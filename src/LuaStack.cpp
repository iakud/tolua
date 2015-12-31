#include "LuaStack.h"

#include "util/util_log.h"

#include "luafix.h"

#include <string.h>

LuaStack::LuaStack()
	: L(NULL)
{

}

LuaStack::~LuaStack()
{
	if(L)
	{
		lua_close(L);
		L = NULL;
	}
}

LuaStack* LuaStack::create()
{
	LuaStack* luastack = new (std::nothrow) LuaStack();
	if (luastack && luastack->init())
	{
		return luastack;
	}

	if (luastack)
	{
		delete luastack;
		luastack = NULL;
	}
	return NULL;
}

void LuaStack::destroy(LuaStack* luastack)
{
	if (luastack)
	{
		delete luastack;
		luastack = NULL;
	}
}

bool LuaStack::init()
{
	do
	{
		L = luaL_newstate();
		if (L == NULL)
			break;
		luaL_openlibs(L);
		luafix_open(L);
		// FIXME: reg func

		return true;
	}while(false);

	return false;
}

void LuaStack::addSearchPath(const char* path)
{
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "path");
    const char* cur_path =  lua_tostring(L, -1);
    lua_pushfstring(L, "%s;%s/?.lua", cur_path, path);
    lua_setfield(L, -3, "path");
    lua_pop(L, 2);
}

void LuaStack::load(const char* modname)
{
	if (modname == NULL || strlen(modname) == 0)
    {
        return;
    }

    std::string name = modname;
    std::string require = "require \'" + name + "\'";
    return executeString(require.c_str());
}

void LuaStack::reload(const char* modname)
{
	if (modname == NULL || strlen(modname) == 0)
    {
        return;
    }

    lua_getglobal(L, "package");
    lua_getfield(L, -1, "loaded");
    lua_pushstring(L, modname);
    lua_gettable(L, -2);
    if (!lua_isnil(L, -1))
    {
        lua_pushstring(L, modname);
        lua_pushnil(L);
        lua_settable(L, -4);
    }
    lua_pop(L, 3);

	std::string name = modname;
    std::string require = "require \'" + name + "\'";
    return executeString(require.c_str());
}

void LuaStack::unload(const char* modname)
{
	if (modname == NULL || strlen(modname) == 0)
    {
        return;
    }

    lua_getglobal(L, "package");
    lua_getfield(L, -1, "loaded");
    lua_pushstring(L, modname);
    lua_gettable(L, -2);
    if (!lua_isnil(L, -1))
    {
        lua_pushstring(L, modname);
        lua_pushnil(L);
        lua_settable(L, -4);
    }
    lua_pop(L, 3);
}

//
// push value
//
void LuaStack::pushBool(bool boolValue)
{
	lua_pushboolean(L, boolValue);
}

void LuaStack::pushInt(int intValue)
{
	lua_pushinteger(L, intValue);
}

void LuaStack::pushLong(long longValue)
{
	lua_pushnumber(L, longValue);
}

void LuaStack::pushFloat(float floatValue)
{
	lua_pushnumber(L, floatValue);
}

void LuaStack::pushDouble(double doubleValue)
{
	lua_pushnumber(L, doubleValue);
}

void LuaStack::pushString(const char* stringValue)
{
	lua_pushstring(L, stringValue);
}

void LuaStack::pushString(const char* stringValue, size_t length)
{
	lua_pushlstring(L, stringValue, length);
}

void LuaStack::pushString(const std::string& stringValue)
{
	lua_pushstring(L, stringValue.c_str());
}

void LuaStack::pushUserType(void* p, const char* type)
{
	luafix_pushusertype(L, p, type);
}

void LuaStack::pushUserType(void* p, const std::string& type)
{
	luafix_pushusertype(L, p, type.c_str());
}

//
// to value
//
bool LuaStack::toBool(int index)
{
	return lua_toboolean(L, index);
}

int LuaStack::toInt(int index)
{
	return lua_tointeger(L, index);
}

long LuaStack::toLong(int index)
{
	return lua_tonumber(L, index);
}

float LuaStack::toFloat(int index)
{
	return lua_tonumber(L, index);
}

double LuaStack::toDouble(int index)
{
	return lua_tonumber(L, index);
}

const char* LuaStack::toString(int index)
{
	return lua_tostring(L, index);
}

const char* LuaStack::toString(int index, size_t& length)
{
	return lua_tolstring(L, index, &length);
}

void* LuaStack::toUserType(int index, const char* type)
{
	return luafix_tousertype(L, index, type);
}

void* LuaStack::toUserType(int index, const std::string& type)
{
	return luafix_tousertype(L, index, type.c_str());
}

//
void LuaStack::clean(void)
{
	lua_settop(L, 0);
}

//
// execute
//
void LuaStack::pushGlobalFunction(const char* functionName)
{
	lua_getglobal(L, functionName);
}

void LuaStack::pushGlobalFunction(const std::string& functionName)
{
	lua_getglobal(L, functionName.c_str());
}

void LuaStack::execute(int nargs, int nresults)
{
	if(lua_pcall(L, nargs, nresults, 0) != 0)
	{
		std::string error = lua_tostring(L, -1);
		lua_pop(L, 1);
		throw std::runtime_error(error);
	}
}

void LuaStack::executeGlobalFunction(const char* functionName, int nargs, int nresults)
{
	lua_getglobal(L, functionName);
	lua_insert(L, -(nargs + 1));
	if(lua_pcall(L, nargs, nresults, 0) != 0)
	{
		std::string error = lua_tostring(L, -1);
		lua_pop(L, 1);
		throw std::runtime_error(error);
	}
}

void LuaStack::executeGlobalFunction(const std::string& functionName, int nargs, int nresults)
{
	lua_getglobal(L, functionName.c_str());
	lua_insert(L, -(nargs + 1));
	if(lua_pcall(L, nargs, nresults, 0) != 0)
	{
		std::string error = lua_tostring(L, -1);
		lua_pop(L, 1);
		throw std::runtime_error(error);
	}
}

void LuaStack::executeString(const char* codes)
{
	luaL_loadstring(L, codes);
    execute(0, 0);
}