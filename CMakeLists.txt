cmake_minimum_required(VERSION 3.22)

project(ep VERSION 0.0 LANGUAGES CXX)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

if(NOT CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

option(UNIT_TESTS "Build and run unit tests" OFF)
option(VCPKG_DEPS "Use vcpkg to fetch dependencies" OFF)

include(cmake/compile_flags.cmake)

if (VCPKG_DEPS)
  include(cmake/vcpkg.cmake)

  # in case qt builds from source once again without:
  #   /usr/bin/ld: fcxml.c:(...):      undefined reference to `XML_...'
  #   /usr/bin/ld: fcfreetype.c:(...): undefined reference to `FT_....'
  #
  add_vcpkg_deps("gtest" "qtbase")
endif()

add_subdirectory(core)

if(UNIT_TESTS)
  add_subdirectory(test)
endif()
