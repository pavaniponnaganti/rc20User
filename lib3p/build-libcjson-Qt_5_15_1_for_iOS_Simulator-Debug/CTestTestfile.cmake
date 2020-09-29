# CMake generated Testfile for 
# Source directory: /Users/naveenpareek/Downloads/rc20-user/lib3p/libcjson
# Build directory: /Users/naveenpareek/Downloads/rc20-user/lib3p/build-libcjson-Qt_5_15_1_for_iOS_Simulator-Debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(cJSON_test "/Users/naveenpareek/Downloads/rc20-user/lib3p/build-libcjson-Qt_5_15_1_for_iOS_Simulator-Debug/cJSON_test")
set_tests_properties(cJSON_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/naveenpareek/Downloads/rc20-user/lib3p/libcjson/CMakeLists.txt;227;add_test;/Users/naveenpareek/Downloads/rc20-user/lib3p/libcjson/CMakeLists.txt;0;")
subdirs("tests")
subdirs("fuzzing")
