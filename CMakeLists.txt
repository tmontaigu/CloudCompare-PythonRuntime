include(helpers.cmake)

option(PLUGIN_PYTHON "Install example plugin" OFF)

if (PLUGIN_PYTHON)
    project(PythonPlugin)

    AddPlugin(NAME ${PROJECT_NAME})

    # Qt's 'slots' conflicts with Python's 'slots'
    target_compile_definitions(PythonPlugin PRIVATE -DQT_NO_KEYWORDS)

    add_subdirectory(wrapper)

    add_subdirectory(include)
    add_subdirectory(src)
    add_subdirectory(ui)

    add_subdirectory(QCodeEditor)

    find_package(pybind11 CONFIG REQUIRED)
    target_link_libraries(PythonPlugin pybind11::embed)

    option(PLUGIN_PYTHON_COPY_ENV "Should the content of the current python venv be copied on install" ON)
    mark_as_advanced(PLUGIN_PYTHON_COPY_ENV)

    set(CC_PYTHON_ENV_NAME "Python")
    set(CC_PLUGIN_INSTALL_DIR ${CLOUDCOMPARE_DEST_FOLDER}/plugins)
    set(CC_PYTHON_INSTALL_DIR "${CC_PLUGIN_INSTALL_DIR}/${CC_PYTHON_ENV_NAME}")

    if (WIN32)
        if (PLUGIN_PYTHON_COPY_ENV)
            copy_python_venv(${CC_PYTHON_INSTALL_DIR})
        endif()
        INSTALL(
                TARGETS pycc cccorelib
                DESTINATION "${CC_PYTHON_INSTALL_DIR}/Lib/site-packages"
        )
    endif ()
endif ()


