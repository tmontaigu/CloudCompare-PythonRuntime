
target_sources(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/AboutDialog.h
        ${CMAKE_CURRENT_LIST_DIR}/PrivateRuntime.h
        ${CMAKE_CURRENT_LIST_DIR}/PythonPlugin.h
        ${CMAKE_CURRENT_LIST_DIR}/PythonInterpreter.h
        ${CMAKE_CURRENT_LIST_DIR}/QPythonREPL.h
        ${CMAKE_CURRENT_LIST_DIR}/PythonHighlighter.h
        ${CMAKE_CURRENT_LIST_DIR}/Consoles.h
        ${CMAKE_CURRENT_LIST_DIR}/ccGUIPythonInstance.h
        ${CMAKE_CURRENT_LIST_DIR}/FileRunner.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Utilities.h
        )

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )
