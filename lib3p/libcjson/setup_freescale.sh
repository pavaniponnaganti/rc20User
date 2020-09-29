#!/bin/bash
export websockets_bin_dir="bin/freescale"
if [ -e "$websockets_bin_dir" ] 
then
  exit 0
fi

. ./setup_common.sh

ret=-1
count=0
while [ $ret -ne 0 ];
do
    cmake -G "Unix Makefiles" -DRC10_LIB_SUBPATH=lib -DCMAKE_TOOLCHAIN_FILE=./toolchain-freescale.cmake $NOJA_CMAKE_VARS ../../
    ret=$?
    (( count += 1 ))
    if [ $count -gt 3 ]; then
      exit -1
    fi
done
