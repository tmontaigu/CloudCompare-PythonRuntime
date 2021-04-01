add_subdirectory(qcc_io)
add_subdirectory(qcc_db)
if (DEFINE_PYCC_RUNTIME)
    add_subdirectory(runtime)
endif()

set(pycc_sources
    ${CMAKE_CURRENT_LIST_DIR}/casters.h
    ${CMAKE_CURRENT_LIST_DIR}/pycc.cpp
    ${pycc_runtime_sources}
    ${pycc_io_sources}
    ${pycc_db_sources}
    PARENT_SCOPE
)

