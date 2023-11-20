cmake_minimum_required(VERSION 3.22)

project(ep VERSION 0.0 LANGUAGES CXX)

if(NOT CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

option(EP_UT "Build and run unit tests" ON)
option(SKIP_DEPS "Skip dependency fetch step" OFF)

include(cmake/compile_flags.cmake)
include(cmake/vcpkg.cmake)

if (NOT SKIP_DEPS)
  add_vcpkg_deps("gtest")
endif()

add_subdirectory(core)

if(NOT EP_UT)
  message(STATUS "Unit tests are disabled")
else()
  add_subdirectory(test)
endif()
