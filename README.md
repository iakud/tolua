# tolua

* 支持shared_ptr包装c++对象传递到lua
* 支持lua中直接对userdata get/set

###### C++
```c++
tolua_usertype(L, "ClassA", tolua_sharedcol);
tolua_class(L, "ClassA", "ClassA", NULL);
tolua_beginusertype(L, "ClassA");
{
    tolua_function(L, "create", lua_ClassA_create);
    tolua_function(L, "getMessage", lua_ClassA_getMessage);
}
tolua_endusertype(L);
```
###### Lua
```lua
local a = ClassA:create()
print(a:getMessage())

a.number = 1993
a.string = "tolua string"
print(a.number)
print(a.string)
```
