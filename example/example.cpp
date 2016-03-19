#include "tolua.h"

#include "LuaStack.h"
#include "tolua.h"
#include "tolua_shared.h"

#include <iostream>

class ClassA {
public:
	const char* getMessage() { return "call getMessage"; }
};

static int lua_ClassA_getMessage(lua_State* L) {
	std::tr1::shared_ptr<ClassA> a = std::tr1::static_pointer_cast<ClassA>(tolua_tosharedusertype(L, 1, "ClassA"));
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

static int lua_ClassA_create(lua_State* L) {
    if (!tolua_isusertable(L, 1, "ClassA")) {
        luaL_error(L, "ClassA error in function '%s'", __FUNCTION__);
        return 0;
    }
    int argc = lua_gettop(L) - 1;
    if (argc == 0) {
        std::tr1::shared_ptr<ClassA> a(new ClassA());
        tolua_pushsharedusertype(L, a, "ClassA");    
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
	try {
		luaStack->load("example");
		luaStack->executeGlobalFunction("testfunc", 0);
	} catch (std::exception& ex) {
		std::cout<<ex.what()<<std::endl;
	}
	LuaStack::destroy(luaStack);
	return 0;
}