include(cmake/helpers.cmake)
include(cmake/CompilerWarnings.cmake)

option(PLUGIN_PYTHON "Install Python Plugin" OFF)

if (PLUGIN_PYTHON)
    project(PythonPlugin)

    AddPlugin(NAME ${PROJECT_NAME})

    # set_project_warnings(${PROJECT_NAME})

    option(PLUGIN_PYTHON_USE_EMBEDDED_MODULES "Should the Python wrapper libs be embedded in the plugin" ON)
    mark_as_advanced(PLUGIN_PYTHON_USE_EMBEDDED_MODULES)

    if (WIN32)
        option(PLUGIN_PYTHON_COPY_ENV "Should the content of the current python venv be copied on install" ON)
        mark_as_advanced(PLUGIN_PYTHON_COPY_ENV)

        if (NOT PYTHON_EXECUTABLE)
            if (DEFINED ENV{CONDA_PREFIX})
                list(INSERT CMAKE_PREFIX_PATH 0 "$ENV{CONDA_PREFIX}/Library/share/cmake")
                set(PYTHON_EXECUTABLE "$ENV{CONDA_PREFIX}/python.exe")
            elseif (DEFINED ENV{VIRTUAL_ENV})
                set(PYTHON_EXECUTABLE "$ENV{VENV_PREFIX}/Scripts/python.exe")
            endif()
        endif()
    endif()

    if (PYTHON_EXECUTABLE)
        execute_process(
            COMMAND "${PYTHON_EXECUTABLE}" "-m" "pybind11" "--cmake"
            RESULT_VARIABLE _PYTHON_SUCCESS
            OUTPUT_VARIABLE PYBIND11_CMAKE_MODULES_PATH
        )
        
        if (_PYTHON_SUCCESS MATCHES 0)
            string(REGEX REPLACE "\n" "" PYBIND11_CMAKE_MODULES_PATH ${PYBIND11_CMAKE_MODULES_PATH})
            list(INSERT CMAKE_PREFIX_PATH 0 "${PYBIND11_CMAKE_MODULES_PATH}")
        endif()
    endif()

    find_package(pybind11 CONFIG REQUIRED)

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


    target_link_libraries(PythonPlugin pybind11::embed)
    set_target_properties(PythonPlugin PROPERTIES CXX_VISIBILITY_PRESET hidden)

    if (WIN32)
        manage_windows_install()
    elseif(UNIX AND NOT APPLE)
        if (NOT PLUGIN_PYTHON_USE_EMBEDDED_MODULES)
            InstallSharedLibrary(TARGET cccorelib)
            InstallSharedLibrary(TARGET pycc)
        endif()
    endif()
endif ()


