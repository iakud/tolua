cmake_minimum_required(VERSION 2.8.12)

project(tolua)

set(CXX_FLAGS
	-g
	-DCHECK_PTHREAD_RETURN_VALUE
	-D_FILE_OFFSET_BITS=64
	-Wall
	-Wextra
	-Werror
	-Wconversion
	-Wno-unused-parameter
	-Wno-unused-result
	-Wold-style-cast
	-Woverloaded-virtual
	-Wpointer-arith
	-Wshadow
	-Wwrite-strings
	-march=native
	-std=c++11
	#-rdynamic
	)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR})

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)

message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " "
	${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

add_subdirectory(tolua)

if(NOT CMAKE_BUILD_NO_SHARED)
	if(NOT CMAKE_BUILD_NO_EXAMPLES)
		add_subdirectory(example)
	endif()
endif()
