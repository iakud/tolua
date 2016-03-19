cmake_minimum_required(VERSION 2.6)

set(CXX_FLAGS
	-g
	-DCHECK_PTHREAD_RETURN_VALUE
	-D_FILE_OFFSET_BITS=64
	-Wall
	-Wextra
	-Werror
	-Wconversion
	-Wno-unused-parameter
	-Wold-style-cast
	-Woverloaded-virtual
	-Wpointer-arith
	-Wshadow
	-Wwrite-strings
	-march=native
	-rdynamic
	#-std=c++0x
	)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCE_FILES)

add_library(tolua ${SOURCE_FILES})

# add_executable(test ${SOURCE_FILES})

if(NOT CMAKE_BUILD_NO_EXAMPLES)
	add_subdirectory(example)
endif()