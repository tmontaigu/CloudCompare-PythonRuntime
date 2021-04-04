include(helpers.cmake)

option(PLUGIN_PYTHON "Install example plugin" OFF)

if (PLUGIN_PYTHON)
    project(PythonPlugin)

    AddPlugin(NAME ${PROJECT_NAME})

    option(PLUGIN_PYTHON_USE_EMBEDDED_MODULES "Should th Python wrapper libs be embedded in the cc plugin" ON)
    mark_as_advanced(PLUGIN_PYTHON_USE_EMBEDDED_MODULES)

    if (WIN32)
        option(PLUGIN_PYTHON_COPY_ENV "Should the content of the current python venv be copied on install" ON)
        mark_as_advanced(PLUGIN_PYTHON_COPY_ENV)

        set(CONDA_PREFIX $ENV{CONDA_PREFIX} CACHE PATH "Path to the root of the conda env")
        string(REPLACE "\\" "/" CONDA_PREFIX ${CONDA_PREFIX})

        # To avoid having to launch cmake the ide from an activated env
        # if a conda env is already activated, it is prioritized
        if (NOT CONDA_PREFIX AND NOT DEFINED ENV{CONDA_PREFIX})
            message(DEBUG "Not using conda")
        elseif (CONDA_PREFIX AND NOT DEFINED ENV{CONDA_PREFIX})
            list(APPEND CMAKE_PREFIX_PATH "${CONDA_PREFIX}/Library/share/cmake")
            set(PYTHON_EXECUTABLE "${CONDA_PREFIX}/python.exe")
        endif()
    endif()

    if (DEFINED ENV{VIRTUAL_ENV} OR CONDA_PREFIX)
        try_append_pybind11_cmake_module_to_modules_path()
    endif()

    add_subdirectory(wrapper)
    add_subdirectory(include)
    add_subdirectory(src)
    add_subdirectory(ui)
    add_subdirectory(QCodeEditor)

    if (PLUGIN_PYTHON_USE_EMBEDDED_MODULES)
        target_compile_definitions(PythonPlugin PRIVATE -DUSE_EMBEDDED_MODULES)

        embed_cccorelib_in(${PROJECT_NAME})
        embed_pycc_in(${PROJECT_NAME})
    endif()

    find_package(pybind11 CONFIG REQUIRED)

    target_link_libraries(PythonPlugin pybind11::embed)

    if (WIN32)
        manage_windows_install()
    elseif(UNIX AND NOT APPLE)
        if (NOT PLUGIN_PYTHON_USE_EMBEDDED_MODULES)
            InstallSharedLibrary(TARGET cccorelib)
            InstallSharedLibrary(TARGET pycc)
        endif()
    endif()
endif ()


