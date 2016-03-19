#ifndef _TOLUA_EVENT_H_
#define _TOLUA_EVENT_H_

#include <luajit-2.0/lua.hpp>

void tolua_classevents(lua_State* L);
void tolua_collector(lua_State* L, lua_CFunction col);

#endif // _TOLUA_EVENT_H_