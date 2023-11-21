find_package(
  Qt6 REQUIRED

  COMPONENTS
  Core
  Gui
  Quick
  QuickControls2
  )

qt_standard_project_setup(
  REQUIRES 6.0
  )

qt_add_executable(
  ep main.cpp
  )

set_source_files_properties(
  ${PROJECT_SOURCE_DIR}/gui/main.qml

  PROPERTIES
  QT_RESOURCE_ALIAS main.qml
)

qt_add_qml_module(
  ep

  URI epqml

  VERSION 0.0

  QML_FILES
  ${PROJECT_SOURCE_DIR}/gui/main.qml
)

target_link_libraries(
  ep

  PUBLIC
  CompileFlags

  PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Quick
  Qt6::QuickControls2
  )
