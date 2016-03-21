add_executable(example LuaStack.cpp example.cpp)
target_link_libraries(example tolua luajit-5.1.a dl)