#include "LuaEngine.h"

#include "luaregister/lua_register_all.h"
#include "physics/PhysicsSpace.h"
#include <iostream>

LuaEngine::LuaEngine()
	: m_luaStack(NULL)
{

}

LuaEngine::~LuaEngine()
{

}

int32_t LuaEngine::init()
{
	do
	{
		m_luaStack = LuaStack::create();
		if (m_luaStack == NULL)
			break;

		m_luaStack->addSearchPath("Script");
		m_luaStack->load("start");

		lua_register_all(m_luaStack->getLuaState());

		// test lua
		try
		{
			for (int i = 0; i < 1; ++i) {
				//m_luaStack->pushUserType((void*)0x7786, "PhysicsSpace");
				m_luaStack->executeGlobalFunction("teststack", 0);
				//std::cout << m_luaStack->toUserType(-1, "PhysicsSpace") << std::endl;
				m_luaStack->clean();
			}
		}
		catch (std::exception& ex)
		{
			std::cout<<ex.what()<<std::endl;
		}
		
		return 0;
	} while(false);

	return -1;
}

void LuaEngine::destroy()
{
	if (m_luaStack)
	{
		LuaStack::destroy(m_luaStack);
		m_luaStack = NULL;
	}
}