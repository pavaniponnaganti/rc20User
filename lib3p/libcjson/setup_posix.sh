#!/bin/bash
echo "----------------------------------------------------------------------------"
echo " Automatically set up development environment for POSIX-platform"
echo "----------------------------------------------------------------------------"
echo ""
echo " Includes 64bit-datatypes, float-datatypes, Ethernet-Interface,"
echo " ASN1-encoding, ..."
echo ""
echo " To include tests set directories for boost-test-framework and "
echo " set FORTE_TESTS-option to 'ON'"
echo ""
echo "----------------------------------------------------------------------------"

export forte_bin_dir="bin/posix"

#set to boost-include directory
export forte_boost_test_inc_dirs=""
#set to boost-library directory
export forte_boost_test_lib_dirs=""

RC10_PATH="`pwd`/../"
echo "RC10 include path $RC10_PATH"

if [ ! -d "$forte_bin_dir" ]; then
  mkdir -p "$forte_bin_dir"
fi

if [ -d "$forte_bin_dir" ]; then
  echo "For building forte go to $forte_bin_dir and execute \"make\""
  echo "forte can be found at ${forte_bin_dir}/src"
  echo "forte_tests can be found at ${forte_bin_dir}/tests"
  
  cd "./$forte_bin_dir"
  
  cmake -G "Unix Makefiles" -DENDIAN=Little -DRC10_PATH=$RC10_PATH -DRC10_LIB_SUBPATH=lib-host -DFORTE_ARCHITECTURE=Posix -DFORTE_COM_ETH=ON -DFORTE_COM_FBDK=ON -DFORTE_COM_LOCAL=ON -DFORTE_TESTS=OFF -DFORTE_MODULE_CONVERT=ON -DFORTE_MODULE_MATH=ON -DFORTE_MODULE_IEC61131=ON -DFORTE_MODULE_OSCAT=OFF -DFORTE_MODULE_Test=ON -DFORTE_MODULE_UTILS=ON -DFORTE_SUPPORT_BOOT_FILE=ON -DFORTE_MODULE_NOJA=ON ../../

else
  echo "unable to create ${forte_bin_dir}"
  exit 1
fi
