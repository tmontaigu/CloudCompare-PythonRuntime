target_sources(
  ${PROJECT_NAME}
  PRIVATE # cmake-format: sortable
          ${CMAKE_CURRENT_LIST_DIR}/ccCommandLine.cpp
          ${CMAKE_CURRENT_LIST_DIR}/ccGuiPythonInstance.cpp
          ${CMAKE_CURRENT_LIST_DIR}/ccGuiPythonInstance.h
          ${CMAKE_CURRENT_LIST_DIR}/Consoles.h
          ${CMAKE_CURRENT_LIST_DIR}/Runtime.cpp
          ${CMAKE_CURRENT_LIST_DIR}/Runtime.h
)
