#ifndef _LUA_ENGINE_H_
#define _LUA_ENGINE_H_

#include "LuaStack.h"

#include "util/util_singleton.h"

#include <stdint.h>

class LuaEngine: public mfw::CSingleton<LuaEngine>
{
public:
	LuaEngine();
	virtual ~LuaEngine();
	
	int32_t init();
	void destroy();

	LuaStack* getLuaStack() { return m_luaStack; }

private:
	LuaStack* m_luaStack;
};

#endif // _LUA_ENGINE_H_