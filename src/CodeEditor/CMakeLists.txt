target_sources(
  ${PROJECT_NAME}
  PRIVATE # cmake-format: sortable
          ${CMAKE_CURRENT_SOURCE_DIR}/CodeEditor.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/CodeEditor.h
          ${CMAKE_CURRENT_SOURCE_DIR}/EditorSettings.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/EditorSettings.h
          ${CMAKE_CURRENT_SOURCE_DIR}/ProjectView.h
          ${CMAKE_CURRENT_SOURCE_DIR}/ProjectViewContextMenu.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/ProjectViewContextMenu.h
          ${CMAKE_CURRENT_SOURCE_DIR}/PythonEditor.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/PythonEditor.h
)
