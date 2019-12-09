#ifndef _TOLUA_FUNCTION_H_
#define _TOLUA_FUNCTION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>

#ifdef __cplusplus
}
#endif

typedef struct tolua_FunctionRef tolua_FunctionRef;

struct tolua_FunctionRef* tolua_tofunction_ref(lua_State* L, int index);
void tolua_pushfunction_ref(lua_State* L, struct tolua_FunctionRef* func);
void tolua_removefunction_ref(lua_State* L, struct tolua_FunctionRef* func);

#endif // _TOLUA_FUNCTION_H_
