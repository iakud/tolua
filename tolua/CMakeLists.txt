cmake_minimum_required(VERSION 2.6)

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

# add_executable(test ${SOURCE_FILES})
