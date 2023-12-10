add_library(
  tab_content

  tab_builder.cpp
  ${CMAKE_SOURCE_DIR}/core/include/tab_content/tab_builder.hpp
  )

target_link_libraries(
  tab_content

  PRIVATE

  CompileFlags
  Qt6::Gui
  Qt6::Widgets
  )

target_include_directories(
  tab_content

  PUBLIC

  ${CMAKE_SOURCE_DIR}/core/include/
  )
