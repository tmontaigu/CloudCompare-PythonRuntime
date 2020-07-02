
target_sources(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Runtime.cpp
        ${CMAKE_CURRENT_LIST_DIR}/PythonPlugin.cpp
        ${CMAKE_CURRENT_LIST_DIR}/QPythonREPL.cpp
        ${CMAKE_CURRENT_LIST_DIR}/PythonHighlighter.cpp
        )
