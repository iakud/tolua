add_executable(example LuaStack.cpp example.cpp)

target_link_libraries(example tolua)

if(NOT CMAKE_BUILD_NO_LUAJIT)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(PKGS REQUIRED luajit)
    target_link_libraries(example ${PKGS_LIBRARIES})
else()
    target_link_libraries(example lua)
endif()

if(NOT CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(example dl)
endif()