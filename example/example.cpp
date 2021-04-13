#include "LuaStack.h"

#include <iostream>

class Book : public std::enable_shared_from_this<Book> {
public:
	Book(std::string& name) : m_name(name) {}

	const char* name() { return m_name.c_str(); }

	void setSellCallback(tolua_FunctionRef* callback) {
		m_sellCallback = callback;
	}

	tolua_FunctionRef* getSellCallback() {
		return m_sellCallback;
	}

private:
	std::string m_name;
	tolua_FunctionRef* m_sellCallback;
};

static int book_create(lua_State* L) {
	if (!tolua_isusertable(L, 1, "Book")) {
		luaL_error(L, "Book error in function '%s'", __FUNCTION__);
		return 0;
	}
	int argc = lua_gettop(L) - 1;
	if (argc == 1) {
		std::string name = lua_tostring(L, 2);
		std::shared_ptr<Book> b(new Book(name));
		tolua_pushsharedusertype(L, b, "Book");
		return 1;
	}
	luaL_error(L, "'%s' has wrong number of arguments: %d, was expecting %d \n", __FUNCTION__, argc, 1);
	return 0;
}

static int book_name(lua_State* L) {
	std::shared_ptr<Book> b = std::static_pointer_cast<Book>(tolua_tosharedusertype(L, 1, "Book"));
	if (!b) {
		luaL_error(L, "invalid 'obj' in function '%s'", __FUNCTION__);
		return 0;
	}
	int argc = lua_gettop(L) - 1;
	if (argc == 0) {
		lua_pushstring(L, b->name());
		return 1;
	}
	luaL_error(L, "'%s' has wrong number of arguments: %d, was expecting %d \n", __FUNCTION__, argc, 1);
	return 0;
}

static int book_setSellCallback(lua_State* L) {
	std::shared_ptr<Book> b = std::static_pointer_cast<Book>(tolua_tosharedusertype(L, 1, "Book"));
	if (!b) {
		luaL_error(L, "invalid 'obj' in function '%s'", __FUNCTION__);
		return 0;
	}
	int argc = lua_gettop(L) - 1;
	if (argc == 1) {
		tolua_FunctionRef* func = tolua_tofunction_ref(L, 2);
		b->setSellCallback(func);
		lua_settop(L, 1);
		return 1;
	}
	luaL_error(L, "'%s' has wrong number of arguments: %d, was expecting %d \n", __FUNCTION__, argc, 1);
	return 0;
}

void register_book(lua_State* L) {
	tolua_beginmodule(L, NULL);
	tolua_usertype(L, "Book", tolua_sharedcol);
	tolua_class(L, "Book", "Book", NULL);
	tolua_beginusertype(L, "Book");
	{
		tolua_function(L, "create", book_create);
		tolua_function(L, "name", book_name);
		tolua_function(L, "setSellCallback", book_setSellCallback);
	}
	tolua_endusertype(L);
	tolua_endmodule(L);
}

int main() {
	LuaStack* luaStack = LuaStack::create();
	if (luaStack == NULL) {
		return 1;
	}
	luaStack->addSearchPath(".");
	// register c++obj to lua
	register_book(luaStack->getLuaState());
	// do
	try {
		luaStack->load("test");
		// create book
		luaStack->pushString("Programming in Lua");
		luaStack->executeGlobalFunction("create_book", 1);
		std::shared_ptr<Book> b = std::static_pointer_cast<Book>(luaStack->toSharedUserType(-1, "Book"));
		luaStack->clean();
		std::cout<<"call create_book, name: "<<b->name()<<std::endl;

		luaStack->pushSharedUserType(b, "Book");
		luaStack->pushString("Roberto Ierusalimschy");
		luaStack->executeGlobalFunction("store_author", 2);
		luaStack->clean();
		std::cout<<"call store_author"<<std::endl;

		luaStack->pushSharedUserType(b, "Book");
		luaStack->executeGlobalFunction("load_author", 1, 1);
		std::string author = luaStack->toString(-1);
		luaStack->clean();
		std::cout<<"call load_author, author: "<<author<<std::endl;

		tolua_FunctionRef* func = b->getSellCallback();
		luaStack->pushInt(6);
		luaStack->executeFunction(func, 1);
		luaStack->clean();
		std::cout<<"callback sell"<<std::endl;

		std::cout<<"call test_error before"<<std::endl;
		luaStack->pushSharedUserType(b, "Book");
		luaStack->executeGlobalFunction("test_error", 1);
		luaStack->clean();
	} catch (std::exception& ex) {
		std::cout<<ex.what()<<std::endl;
	}
	LuaStack::destroy(luaStack);
	std::cout<<"finish"<<std::endl;
	return 0;
}