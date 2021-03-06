#!/bin/bash

SOURCE_DIR=`pwd`
BUILD_DIR=build
BUILD_TYPE=debug
INSTALL_DIR=${BUILD_TYPE}-install
BUILD_NO_LUAJIT=0
BUILD_NO_SHARED=0
BUILD_NO_EXAMPLES=0

mkdir -p ${BUILD_DIR}/${BUILD_TYPE} \
	&& cd ${BUILD_DIR}/${BUILD_TYPE} \
	&& cmake \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DCMAKE_BUILD_NO_LUAJIT=${BUILD_NO_LUAJIT} \
		-DCMAKE_BUILD_NO_SHARED=${BUILD_NO_SHARED} \
		-DCMAKE_BUILD_NO_EXAMPLES=${BUILD_NO_EXAMPLES} \
		${SOURCE_DIR} \
	&& make $*
