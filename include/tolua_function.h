#ifndef _TOLUA_FUNCTION_H_
#define _TOLUA_FUNCTION_H_

#include <luajit-2.0/lua.hpp>

struct tolua_function_ref;

tolua_function_ref* tolua_tofunction(lua_State* L, int index);
void tolua_pushfunction(lua_State* L, const tolua_function_ref* function_ref);
void tolua_removefunction(lua_State* L, tolua_function_ref* function_ref);

#endif // _TOLUA_FUNCTION_H_