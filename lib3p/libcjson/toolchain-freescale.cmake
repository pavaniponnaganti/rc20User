INCLUDE(CMakeForceCompiler)

# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# cortexa7hf-neon-poky-linux-gnueabi
# specify the cross compiler
# Use default CC, pocky script will take care of this
# SET(CMAKE_C_COMPILER    /opt/poky/2.2.2/sysroots/i686-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc)
# SET(CMAKE_CXX_COMPILER  /opt/poky/2.2.2/sysroots/i686-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++)


# specify the OPENSSL link and build information for cross-compiler version isntead of calling the stupid find_package(OpenSSL, required) function in CMakelist.

# SET(OPENSSL_FOUND TRUE)
# SET(OPENSSL_INCLUDE_DIR /home/dev/Code/bsp-morty/relay20/tmp/sysroots/ls1021atwr/usr/include)
# SET(OPENSSL_CRYPTO_LIBRARY /home/dev/Code/bsp-morty/relay20/tmp/sysroots/ls1021atwr/usr/lib/libcrypto.so)
# SET(OPENSSL_SSL_LIBRARY /home/dev/Code/bsp-morty/relay20/tmp/sysroots/ls1021atwr/usr/lib/libssl.so)
# SET(OPENSSL_LIBRARIES /home/dev/Code/bsp-morty/relay20/tmp/sysroots/ls1021atwr/usr/lib/libssl.so; /home/dev/Code/bsp-morty/relay20/tmp/sysroots/ls1021atwr/usr/lib/libcrypto.so)
# SET(OPENSSL_VERSION 1.0.2g)
SET(__ARM_PCS_VFP 1)

 # SET(OPENSSL_EXECUTABLE /usr/bin/openssl)
# Compiler flags
SET(CMAKE_C_FLAGS  "-O2 -pipe -fno-common -fno-builtin -Wall -Dlinux -D__linux__ -Dunix -DDEBUG  -I../include -I../ -Iinclude -Wno-unused-variable")
SET(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

# Forte build with poky toolset produces a huge number of warnings. These should be harmless but fill up the log file:
# -Wno-conversion           (Disables "warning: conversion to ‘x’ from ‘y’ may alter its value")
# -Wno-sized-deallocation   (Disables "warning: the program should also define ‘x’")
# add_definitions("-Wno-conversion -Wno-sized-deallocation")

# search for programs in the build host directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(BUILD_STATIC_TARGET true)

# Modify CMakeLists.txt as follows:
# SET(FORTE_BootfileLocation ${NOJA_BOOTFILE_LOCATION} CACHE STRING "Path to the bootfile, if same directory as forte executable leave empty, include trailing '/'!")

# Need to change the filename in RCBindings.h as well as config.h.in
SET(NOJA_BOOTFILE_LOCATION "/var/nand/sga/" CACHE STRING "Path to forte.fboot file")
# Set the ticks per second to 100 to decrease CPU usage by the timer. Previously 1000 (1ms tick).
SET(NOJA_TICKS_PER_SECOND "100" CACHE STRING "100 ticks per second means 1 tick every 10ms on RC10")
SET(NOJA_TIMEBASE_UNITS_PER_SECOND "1000" CACHE STRING "Timebase of 1000 units per second means 1ms for RC10")

# Remove the -DFORTE_LITTLE_ENDIAN from src/arch/posix/CMakeLists.txt
add_definitions("-DCJSON_LITTLE_ENDIAN")
