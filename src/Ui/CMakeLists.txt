target_sources(
  ${PROJECT_NAME}
  PRIVATE # cmake-format: sortable
          ${CMAKE_CURRENT_LIST_DIR}/AboutDialog.ui
          ${CMAKE_CURRENT_LIST_DIR}/EditorSettings.ui
          ${CMAKE_CURRENT_LIST_DIR}/InstallDialog.ui
          ${CMAKE_CURRENT_LIST_DIR}/PackageManager.ui
          ${CMAKE_CURRENT_LIST_DIR}/PathVariableEditor.ui
          ${CMAKE_CURRENT_LIST_DIR}/PythonEditor.ui
        ${CMAKE_CURRENT_LIST_DIR}/PythonRuntimeSettings.ui
          ${CMAKE_CURRENT_LIST_DIR}/PythonREPL.ui
)

set_target_properties(
  ${PROJECT_NAME} PROPERTIES AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_LIST_DIR}
)
