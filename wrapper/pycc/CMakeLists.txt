project(pycc LANGUAGES CXX)

# TODO if runtime is not defined then, all observer_ptr<T> should becomte unique_ptr<T>s
option(DEFINE_PYCC_RUNTIME "Should the runtime functions be included" ON)

add_subdirectory(src)


if (NOT USE_EMBEDDED_MODULES)
    find_package(pybind11 CONFIG REQUIRED)

    pybind11_add_module(pycc ${pycc_sources})
    target_link_libraries(pycc PRIVATE QCC_DB_LIB QCC_GL_LIB QCC_IO_LIB)
    target_include_directories(pycc PRIVATE ../../include)
    if (DEFINE_PYCC_RUNTIME)
        target_link_libraries(pycc PRIVATE PythonPlugin)
        target_compile_definitions(pycc PRIVATE DEFINE_PYCC_RUNTIME)
    endif()
endif()

function(embed_pycc_in target)
    target_sources(${target} PRIVATE ${pycc_sources})
    if (DEFINE_PYCC_RUNTIME)
        target_compile_definitions(${target} PRIVATE DEFINE_PYCC_RUNTIME)
    endif()
endfunction(embed_pycc_in)

# Cache variables so embed_pycc_in can be used in parent projects
set(pycc_sources ${pycc_sources} CACHE INTERNAL "")
