add_executable(example LuaStack.cpp example.cpp)

target_link_libraries(example tolua)

if(CMAKE_BUILD_LUA_PC)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(PKGS REQUIRED ${CMAKE_BUILD_LUA_PC})
    target_link_directories(example PUBLIC ${PKGS_LIBRARY_DIRS})
    target_link_libraries(example ${PKGS_LIBRARIES})
else()
    target_link_libraries(example lua)
endif()

if(CMAKE_BUILD_PLAT_MINGW)
    target_link_libraries(example dl)
endif()