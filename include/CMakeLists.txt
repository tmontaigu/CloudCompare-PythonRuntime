target_sources(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/AboutDialog.h
          ${CMAKE_CURRENT_LIST_DIR}/PrivateRuntime.h
          ${CMAKE_CURRENT_LIST_DIR}/PythonPlugin.h
          ${CMAKE_CURRENT_LIST_DIR}/PythonInterpreter.h
          ${CMAKE_CURRENT_LIST_DIR}/QPythonRepl.h
          ${CMAKE_CURRENT_LIST_DIR}/PythonHighlighter.h
          ${CMAKE_CURRENT_LIST_DIR}/ColorScheme.h
          ${CMAKE_CURRENT_LIST_DIR}/PythonHighlighterSettings.h
          ${CMAKE_CURRENT_LIST_DIR}/Consoles.h
          ${CMAKE_CURRENT_LIST_DIR}/ccGuiPythonInstance.h
          ${CMAKE_CURRENT_LIST_DIR}/FileRunner.h
          ${CMAKE_CURRENT_LIST_DIR}/PackageManager.h
          ${CMAKE_CURRENT_LIST_DIR}/Utilities.h
          ${CMAKE_CURRENT_LIST_DIR}/PythonConfig.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
