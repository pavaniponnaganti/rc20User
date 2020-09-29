#!/bin/bash
export websockets_bin_dir="bin/host"
if [ -e "$websockets_bin_dir" ]
then
  exit 0
fi

. ./setup_common.sh
cmake -G "Unix Makefiles" -DRC10_LIB_SUBPATH=lib-host -DCMAKE_TOOLCHAIN_FILE=./toolchain-host.cmake $NOJA_CMAKE_VARS ../../
