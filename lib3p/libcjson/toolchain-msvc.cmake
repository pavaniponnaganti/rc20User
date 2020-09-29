INCLUDE(CMakeForceCompiler)

SET(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_CXX_FLAGS "-m32")
SET(CMAKE_C_COMPILER_ID "MSVC")
# search for programs in the build host directories
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(BUILD_STATIC_TARGET true)
