
target_sources(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/AboutDialog.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Runtime.cpp
        ${CMAKE_CURRENT_LIST_DIR}/PythonPlugin.cpp
        ${CMAKE_CURRENT_LIST_DIR}/PythonInterpreter.cpp
        ${CMAKE_CURRENT_LIST_DIR}/QPythonRepl.cpp
        ${CMAKE_CURRENT_LIST_DIR}/PythonHighlighterSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/PythonHighlighter.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ColorScheme.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ccGuiPythonInstance.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileRunner.cpp
        )
