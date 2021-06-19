target_sources(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/AboutDialog.ui
          ${CMAKE_CURRENT_LIST_DIR}/QPythonEditor.ui
          ${CMAKE_CURRENT_LIST_DIR}/QEditorSettings.ui
          ${CMAKE_CURRENT_LIST_DIR}/QPythonREPL.ui
          ${CMAKE_CURRENT_LIST_DIR}/PackageManager.ui
)

set_target_properties(
  ${PROJECT_NAME} PROPERTIES AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_LIST_DIR}
)
