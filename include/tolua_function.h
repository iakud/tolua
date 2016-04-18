#ifndef _TOLUA_FUNCTION_H_
#define _TOLUA_FUNCTION_H_

#include <luajit-2.0/lua.hpp>

struct tolua_function_ref;

tolua_function_ref* tolua_ref_function(lua_State* L, int index);
void tolua_push_function_by_ref(lua_State* L, tolua_function_ref* func);
void tolua_remove_function_by_ref(lua_State* L, tolua_function_ref* func);

#endif // _TOLUA_FUNCTION_H_