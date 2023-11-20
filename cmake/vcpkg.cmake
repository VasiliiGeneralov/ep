set(VCPKG_ROOT "")
if (NOT DEFINED ENV{VCPKG_ROOT})
  message(FATAL_ERROR "Please set VCPKG_ROOT environment variable")
else()
  set(VCPKG_ROOT $ENV{VCPKG_ROOT})
endif()

set(VCPKG_EXEC "")
if (WIN32)
  set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg.exe)
elseif(LINUX)
  set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg)
endif()

if (NOT EXISTS ${VCPKG_EXEC})
  message(FATAL_ERROR "Please add vcpkg executable to PATH")
endif()

set(VCPKG_DEPS_PREFIX ${VCPKG_ROOT}/installed)
if (WIN32)
  #list(APPEND CMAKE_MODULE_PATH ";${VCPKG_DEPS_PREFIX}/x64-windows")
  list(APPEND CMAKE_PREFIX_PATH ";${VCPKG_DEPS_PREFIX}/x64-windows")
elseif(LINUX)
  #list(APPEND CMAKE_MODULE_PATH ";${VCPKG_DEPS_PREFIX}/x64-linux")
  list(APPEND CMAKE_PREFIX_PATH ";${VCPKG_DEPS_PREFIX}/x64-linux")
endif()

function(add_vcpkg_deps)
    execute_process(
      COMMAND ${VCPKG_EXEC} install ${ARGV}
      WORKING_DIRECTORY ${VCPKG_ROOT}
      )
endfunction()
