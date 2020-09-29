INCLUDE(CMakeForceCompiler)

# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   gcc)
SET(CMAKE_CXX_COMPILER g++)

# specify the OPENSSL link and build information  for host mode instead of calling the stupid find_package(OpenSSL, required) function in CMakelist.

# Compiler flags
# The usual SET(CMAKE_C_FLAGS "flags") and SET(CMAKE_CXX_FLAGS "flags") doesn't
# work so we go for the brutal add_definitions() to set both
add_definitions("-g -O0 -pipe -fno-common -fno-builtin -DHOST -Dlinux -D__linux__ -Dunix -DDEBUG -Wall -Wno-unused-variable")

# Linker flags
# Not required for host build:
#     CMAKE_C_LINK_FLAGS, CMAKE_CXX_LINK_FLAGS, CMAKE_EXE_LINKER_FLAGS

# where is the target environment

# search for programs in the build host directories
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(BUILD_STATIC_TARGET true)

# Modify CMakeLists.txt as follows:
# SET(FORTE_BootfileLocation ${NOJA_BOOTFILE_LOCATION} CACHE STRING "Path to the bootfile, if same directory as forte executable leave empty, include trailing '/'!")

# Remove the -DWEBSOCKETS_LITTLE_ENDIAN from src/arch/posix/CMakeLists.txt
add_definitions("-DCJSON_LITTLE_ENDIAN")
