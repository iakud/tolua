add_executable(example LuaStack.cpp example.cpp)
target_link_libraries(example tolua lua dl)