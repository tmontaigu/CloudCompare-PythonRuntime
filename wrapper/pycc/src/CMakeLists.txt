add_subdirectory(qcc_io)
add_subdirectory(qcc_db)

set(pycc_sources
    ${CMAKE_CURRENT_LIST_DIR}/casters.h
    ${CMAKE_CURRENT_LIST_DIR}/pycc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/load_plugins.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PyPrintLogger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PyPrintLogger.h
    ${pycc_io_sources}
    ${pycc_db_sources}
    PARENT_SCOPE
)
