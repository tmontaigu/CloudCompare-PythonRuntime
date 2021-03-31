project(pycc LANGUAGES CXX)

add_subdirectory(src)

message("${pycc_sources}")

if (NOT USE_EMBEDDED_MODULES)
    find_package(pybind11 CONFIG REQUIRED)

    pybind11_add_module(pycc ${pycc_sources})
    target_link_libraries(pycc PRIVATE QCC_DB_LIB PythonPlugin)
    target_include_directories(pycc PRIVATE ../../include)
endif()

function(embed_pycc_in target)
    target_sources(${target} PRIVATE ${pycc_sources})
    target_link_libraries(${target} QCC_DB_LIB)
endfunction(embed_pycc_in)

# Cache variables so embed_pycc_in can be used in parent projects
set(pycc_sources ${pycc_sources} CACHE INTERNAL "")
