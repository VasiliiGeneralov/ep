add_library(
  main_window

  main_window.cpp

  # Include the headers in a library/executable if classes use Q_OBJECT macro,
  # it's necessary for Qt's `moc`
  ${CMAKE_SOURCE_DIR}/core/include/main_window/main_window.hpp
  )

target_link_libraries(
  main_window

  PRIVATE

  CompileFlags
  tab_content
  Qt6::Widgets
  Qt6::Gui
  )

target_include_directories(
  main_window

  PUBLIC

  ${CMAKE_SOURCE_DIR}/core/include/
  )
