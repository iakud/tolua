cmake_minimum_required(VERSION 2.6)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES)
add_library(tolua ${SOURCE_FILES})

install(TARGETS tolua DESTINATION lib)

set(HEADERS
	tolua.h
	tolua_function.h
	tolua_shared.h
	)
install(FILES ${HEADERS} DESTINATION include/tolua)

# add_executable(test ${SOURCE_FILES})