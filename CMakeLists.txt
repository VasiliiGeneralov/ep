cmake_minimum_required(VERSION 3.22)

project(ep VERSION 0.0 LANGUAGES CXX)

if(NOT CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

option(EP_UT "Build and run unit tests" ON)
option(SKIP_DEPS "Skip dependency fetch step" OFF)

include(cmake/compile_flags.cmake)

if (NOT SKIP_DEPS)
  include(cmake/vcpkg.cmake)

  # in case qt builds from source once again without:
  #   /usr/bin/ld: fcxml.c:(...):      undefined reference to `XML_...'
  #   /usr/bin/ld: fcfreetype.c:(...): undefined reference to `FT_....'
  #
  # remove qtdeclarative once migrated to widgets. qtbase is self sufficient
  # for widget-based apps
  add_vcpkg_deps("gtest" "qtbase" "qtdeclarative")
endif()

add_subdirectory(core)

if(NOT EP_UT)
  message(STATUS "Unit tests are disabled")
else()
  add_subdirectory(test)
endif()
