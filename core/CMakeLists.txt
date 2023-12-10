find_package(
  Qt6 REQUIRED

  COMPONENTS
  Core
  Gui
  Widgets
  )

qt_standard_project_setup()

add_subdirectory(source)

qt_add_executable(
  ep main.cpp
  )

target_link_libraries(
  ep

  PRIVATE

  CompileFlags
  main_window

  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  )
