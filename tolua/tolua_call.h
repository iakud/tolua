#ifndef _TOLUA_CALL_H_
#define _TOLUA_CALL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>

#ifdef __cplusplus
}
#endif

int tolua_docall(lua_State* L, int nargs, int nresults);

#endif // _TOLUA_CALL_H_