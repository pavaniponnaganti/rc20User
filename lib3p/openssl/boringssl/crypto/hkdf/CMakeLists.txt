include_directories(../../include)

add_library(
  hkdf

  OBJECT

  hkdf.c
)

add_executable(
  hkdf_test

  hkdf_test.cc

  $<TARGET_OBJECTS:test_support>
)

target_link_libraries(hkdf_test crypto)
add_dependencies(all_tests hkdf_test)
