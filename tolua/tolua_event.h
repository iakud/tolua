#ifndef _TOLUA_EVENT_H_
#define _TOLUA_EVENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>

#ifdef __cplusplus
}
#endif

void tolua_classevents(lua_State* L);
void tolua_collector(lua_State* L, lua_CFunction col);

#endif // _TOLUA_EVENT_H_