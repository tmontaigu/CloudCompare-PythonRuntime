
target_sources(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ActionA.h
        ${CMAKE_CURRENT_LIST_DIR}/PythonPlugin.h
        ${CMAKE_CURRENT_LIST_DIR}/QPythonREPL.h
        ${CMAKE_CURRENT_LIST_DIR}/PythonHighlighter.h
        ${CMAKE_CURRENT_LIST_DIR}/Consoles.h
        )

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )
