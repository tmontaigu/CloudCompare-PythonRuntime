include(helpers.cmake)

option(PLUGIN_PYTHON "Install example plugin" OFF)

if (PLUGIN_PYTHON)
    project(PythonPlugin)

    AddPlugin(NAME ${PROJECT_NAME})

    option(USE_EMBEDDED_MODULES "Should the Python wrapper libs be embedded in the cc plugin" ON)
    mark_as_advanced(USE_EMBEDDED_MODULES)

    if (WIN32)
        option(PLUGIN_PYTHON_COPY_ENV "Should the content of the current python venv be copied on install" ON)
        mark_as_advanced(PLUGIN_PYTHON_COPY_ENV)
    endif()

    if (DEFINED ENV{VIRTUAL_ENV})
        try_append_pybind11_cmake_module_to_modules_path()
    endif()

    add_subdirectory(wrapper)
    add_subdirectory(include)
    add_subdirectory(src)
    add_subdirectory(ui)
    add_subdirectory(QCodeEditor)

    if (USE_EMBEDDED_MODULES)
        target_compile_definitions(PythonPlugin PRIVATE -DUSE_EMBEDDED_MODULES)

        embed_cccorelib_in(${PROJECT_NAME})
        embed_pycc_in(${PROJECT_NAME})
    endif()

    find_package(pybind11 CONFIG REQUIRED)

    target_link_libraries(PythonPlugin pybind11::embed)

    if (WIN32)
        manage_windows_install()
    elseif(UNIX AND NOT APPLE)
        if (NOT USE_EMBEDDED_MODULES)
            InstallSharedLibrary(TARGET cccorelib)
            InstallSharedLibrary(TARGET pycc)
        endif()
    endif()
endif ()


