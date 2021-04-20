cmake_minimum_required(VERSION 2.8.12)

set(SOURCE_FILES
	tolua.c
	tolua_event.c
	tolua_function.c
    tolua_call.c
)
if(NOT CMAKE_BUILD_NO_SHARED)
	set(SOURCE_FILES ${SOURCE_FILES} tolua_shared.cpp)
endif()
#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES)

add_library(tolua ${SOURCE_FILES})

if(NOT CMAKE_BUILD_NO_LUAJIT)
	find_package(PkgConfig REQUIRED)
	pkg_search_module(PKGS REQUIRED luajit)
	target_include_directories(tolua PUBLIC ${PKGS_INCLUDE_DIRS})
endif()

install(TARGETS tolua DESTINATION lib)

set(HEADERS
	tolua.h
	tolua_function.h
    tolua_call.h
	)
if(NOT CMAKE_BUILD_NO_SHARED)
	set(HEADERS ${HEADERS} tolua_shared.h)
endif()

install(FILES ${HEADERS} DESTINATION include/tolua)

configure_file(tolua.pc.in
	${CMAKE_CURRENT_BINARY_DIR}/tolua.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tolua.pc DESTINATION lib/pkgconfig)

# add_executable(test ${SOURCE_FILES})
