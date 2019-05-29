#include "tolua.h"

#include "LuaStack.h"

#include <iostream>

class ClassA : public std::enable_shared_from_this<ClassA> {
public:
const char* getMessage() { return "call getMessage"; }

	void setCallback(tolua_function_ref* callback) {
		m_callback = callback;
	}

	tolua_function_ref* getCallback() {
		return m_callback;
	}

private:
	tolua_function_ref* m_callback;
};

static int lua_ClassA_create(lua_State* L) {
	if (!tolua_isusertable(L, 1, "ClassA")) {
		luaL_error(L, "ClassA error in function '%s'", __FUNCTION__);
		return 0;
	}
	int argc = lua_gettop(L) - 1;
	if (argc == 0) {
		std::shared_ptr<ClassA> a(new ClassA());
		tolua_pushsharedusertype(L, a, "ClassA");
		return 1;
	}
	luaL_error(L, "'%s' has wrong number of arguments: %d, was expecting %d \n", __FUNCTION__, argc, 1);
	return 0;
}

static int lua_ClassA_getMessage(lua_State* L) {
	std::shared_ptr<ClassA> a = std::static_pointer_cast<ClassA>(tolua_tosharedusertype(L, 1, "ClassA"));
	if (!a) {
		luaL_error(L, "invalid 'obj' in function '%s'", __FUNCTION__);
		return 0;
	}
	int argc = lua_gettop(L) - 1;
	if (argc == 0) {
		const char* message = a->getMessage();
		lua_pushstring(L, message);
		return 1;
	}
	luaL_error(L, "'%s' has wrong number of arguments: %d, was expecting %d \n", __FUNCTION__, argc, 1);
	return 0;
}

static int lua_ClassA_setCallback(lua_State* L) {
	std::shared_ptr<ClassA> a = std::static_pointer_cast<ClassA>(tolua_tosharedusertype(L, 1, "ClassA"));
	if (!a) {
		luaL_error(L, "invalid 'obj' in function '%s'", __FUNCTION__);
		return 0;
	}
	int argc = lua_gettop(L) - 1;
	if (argc == 1) {
		tolua_function_ref* func = tolua_ref_function(L, 2);
		a->setCallback(func);
		lua_settop(L, 1);
		return 1;
	}
	luaL_error(L, "'%s' has wrong number of arguments: %d, was expecting %d \n", __FUNCTION__, argc, 1);
	return 0;
}

void lua_register_class(lua_State* L) {
	tolua_beginmodule(L, NULL);
	tolua_usertype(L, "ClassA", tolua_sharedcol);
	tolua_class(L, "ClassA", "ClassA", NULL);
	tolua_beginusertype(L, "ClassA");
	{
		tolua_function(L, "create", lua_ClassA_create);
		tolua_function(L, "getMessage", lua_ClassA_getMessage);
		tolua_function(L, "setCallback", lua_ClassA_setCallback);
	}
	tolua_endusertype(L);
	tolua_endmodule(L);
}

int main() {
	LuaStack* luaStack = LuaStack::create();
	if (luaStack == NULL) {
		return 1;
	}
	luaStack->addSearchPath("script");
	// register c++obj to lua
	lua_register_class(luaStack->getLuaState());
	// do
	std::shared_ptr<ClassA> a(new ClassA());
	std::shared_ptr<ClassA> clone_a = a->shared_from_this();
	try {
		luaStack->load("example");
		luaStack->pushSharedUserType(a, "ClassA");
		luaStack->executeGlobalFunction("setfunc", 1);
		std::cout<<"call setfunc"<<std::endl;
		luaStack->clean();
		luaStack->pushSharedUserType(a, "ClassA");
		luaStack->executeGlobalFunction("getfunc", 1, 2);
		int luanumber = luaStack->toInt(-2);
		std::string luastring = luaStack->toString(-1);
		std::cout<<"call getfunc"<<",number="<<luanumber<<",string="<<luastring<<std::endl;
		luaStack->clean();
		luaStack->pushSharedUserType(clone_a, "ClassA");
		luaStack->executeGlobalFunction("checkfunc", 1);
		std::cout<<"call checkfunc"<<std::endl;
		luaStack->clean();

		luaStack->executeGlobalFunction("createfunc", 0);
		std::cout<<"call createfunc"<<std::endl;
		std::shared_ptr<ClassA> a_lua = std::static_pointer_cast<ClassA>(luaStack->toSharedUserType(-1, "ClassA"));
		luaStack->clean();
		tolua_function_ref* func = a_lua->getCallback();
		luaStack->pushString("callback message");
		luaStack->executeFunction(func, 1);
		luaStack->clean();
	} catch (std::exception& ex) {
		std::cout<<ex.what()<<std::endl;
	}
	LuaStack::destroy(luaStack);
	return 0;
}