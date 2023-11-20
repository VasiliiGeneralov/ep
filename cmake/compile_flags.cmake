add_library(
  CompileFlags INTERFACE
  )

target_compile_features(
  CompileFlags INTERFACE cxx_std_17
  )

target_compile_options(
  CompileFlags INTERFACE
  $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
    -Wall
    -Werror
  >
  $<$<CXX_COMPILER_ID:MSVC>:
    /Wall
    /WX
  >
  )
