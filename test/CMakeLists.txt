find_package(
  GTest CONFIG REQUIRED
  )

add_executable(
  ep_unit_tests test_main.cpp
  )

target_link_libraries(
  ep_unit_tests

  PRIVATE
  GTest::gtest
  GTest::gtest_main
  )

add_custom_target(
  check ALL COMMAND ep_unit_tests
  )
