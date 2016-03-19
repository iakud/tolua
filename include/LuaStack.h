#ifndef _LUA_STACK_H_
#define _LUA_STACK_H_

#include <luajit-2.0/lua.hpp>

#include <string>

// luaStack帮助类
class LuaStack
{
public:
	static LuaStack* create();
	static void destroy(LuaStack* luastack);

	lua_State* getLuaState() { return L; }

	operator lua_State*() { return L; }

	void addSearchPath(const char* path);
	
	void load(const char* modname);
	void reload(const char* modname);
	void unload(const char* modname);

	//
	// push value
	//
	void pushBool(bool boolValue);
	void pushInt(int intValue);
	void pushLong(long longValue);
	void pushFloat(float floatValue);
	void pushDouble(double doubleValue);
	void pushString(const char* stringValue);
	void pushString(const char* stringValue, size_t length);
	void pushString(const std::string& stringValue);
	void pushUserType(void* p, const char* type);
	void pushUserType(void* p, const std::string& type);

	//
	// to value
	//
	bool toBool(int index);
	int toInt(int index);
	long toLong(int index);
	float toFloat(int index);
	double toDouble(int index);
	const char* toString(int index);
	const char* toString(int index, size_t& length);
	void* toUserType(int index, const char* type);
	void* toUserType(int index, const std::string& type);

	//
	void clean(void);

	//
	// excute
	//
	void executeGlobalFunction(const char* functionName, int nargs, int nresults = LUA_MULTRET);
	void executeGlobalFunction(const std::string& functionName, int nargs, int nresults = LUA_MULTRET);
	void executeString(const char* codes);

protected:
	bool init();
	void execute(int nargs, int nresults = LUA_MULTRET);

protected:
	LuaStack();
	virtual ~LuaStack();

protected:
	lua_State* L;
};

#endif // _LUA_STACK_H_