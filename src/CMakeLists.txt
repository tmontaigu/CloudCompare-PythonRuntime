add_subdirectory(CodeEditor)
add_subdirectory(Runtime)
add_subdirectory(Ui)

target_sources(
  ${PROJECT_NAME}
  PRIVATE # cmake-format: sortable
          ${CMAKE_CURRENT_LIST_DIR}/AboutDialog.cpp
          ${CMAKE_CURRENT_LIST_DIR}/AboutDialog.h
          ${CMAKE_CURRENT_LIST_DIR}/ColorScheme.cpp
          ${CMAKE_CURRENT_LIST_DIR}/ColorScheme.h
          ${CMAKE_CURRENT_LIST_DIR}/FileRunner.cpp
          ${CMAKE_CURRENT_LIST_DIR}/FileRunner.h
          ${CMAKE_CURRENT_LIST_DIR}/PackageManager.cpp
          ${CMAKE_CURRENT_LIST_DIR}/PackageManager.h
          ${CMAKE_CURRENT_LIST_DIR}/PythonActionLauncher.cpp
          ${CMAKE_CURRENT_LIST_DIR}/PythonActionLauncher.h
          ${CMAKE_CURRENT_LIST_DIR}/PythonConfig.cpp
          ${CMAKE_CURRENT_LIST_DIR}/PythonConfig.h
          ${CMAKE_CURRENT_LIST_DIR}/PythonHighlighter.cpp
          ${CMAKE_CURRENT_LIST_DIR}/PythonHighlighter.h
          ${CMAKE_CURRENT_LIST_DIR}/PythonInterpreter.cpp
          ${CMAKE_CURRENT_LIST_DIR}/PythonInterpreter.h
          ${CMAKE_CURRENT_LIST_DIR}/PythonPlugin.cpp
          ${CMAKE_CURRENT_LIST_DIR}/PythonPlugin.h
          ${CMAKE_CURRENT_LIST_DIR}/PythonPluginManager.cpp
          ${CMAKE_CURRENT_LIST_DIR}/PythonPluginManager.h
        ${CMAKE_CURRENT_LIST_DIR}/PythonRuntimeSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/PythonRuntimeSettings.h
          ${CMAKE_CURRENT_LIST_DIR}/PythonRepl.cpp
          ${CMAKE_CURRENT_LIST_DIR}/PythonRepl.h
          ${CMAKE_CURRENT_LIST_DIR}/Resources.h
          ${CMAKE_CURRENT_LIST_DIR}/Utilities.h
          ${CMAKE_CURRENT_LIST_DIR}/WaitingSpinnerWidget.cpp
          ${CMAKE_CURRENT_LIST_DIR}/WaitingSpinnerWidget.h
)

target_include_directories(
  ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}
                          ${CMAKE_CURRENT_LIST_DIR}/CodeEditor
)
