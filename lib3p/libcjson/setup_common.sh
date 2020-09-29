#!/bin/bash
# Change directory and setup environment variables for initialisation execute
# of cmake
#
# This script requires the following variable to be set, example:

# export forte_bin_dir="bin/host"
if [ -z "$websockets_bin_dir" ]
then
  echo "Required variable 'websockets_bin_dir' has not been set"
  exit 1
fi

if [ ! -d "$websockets_bin_dir" ]; then
  mkdir -p "$websockets_bin_dir"
  if [ ! -d "$websockets_bin_dir" ]; then
    echo "unable to create ${websockets_bin_dir}"
    exit 1
  fi
else # we should never get here, the parent setup script needs to test if the directory exists
  echo "directory already exists, so nothing to do"
  exit 0
fi

NAME=libwebsockets

echo "----------------------------------------------------------------------------"
echo " Automatically set up $NAME development environment"
echo "----------------------------------------------------------------------------"
echo ""
echo "For building $NAME go to $websockets_bin_dir and execute \"make\""
echo "lib$NAME.so can be found at ${websockets_bin_dir}"
echo "$NAME test files can be found at ${websockets_bin_dir}/bin"
echo ""
echo "----------------------------------------------------------------------------"

#set to boost-include directory
export websockets_boost_test_inc_dirs=""
#set to boost-library directory
export websockets_boost_test_lib_dirs=""
# RC10 include path
RC10_PATH="`pwd`/../"
cd "./$websockets_bin_dir"
NOJA_CMAKE_VARS="-DRC10_PATH=$RC10_PATH"
