include(helpers.cmake)

option(PLUGIN_PYTHON "Install example plugin" OFF)

if (PLUGIN_PYTHON)
    project(PythonPlugin)

    AddPlugin(NAME ${PROJECT_NAME})

    if (WIN32)
        option(PLUGIN_PYTHON_COPY_ENV "Should the content of the current python venv be copied on install" ON)
        mark_as_advanced(PLUGIN_PYTHON_COPY_ENV)
    endif()

    # If we detect that we are running inside a virtualenv
    # we try to find pybind11 in the site-packages
    # because it bundles the .cmake files the
    # find_package command needs
    if (DEFINED ENV{VIRTUAL_ENV})
        set(PYTHON_SITE_PACKAGES "")
        get_python_sites_packages(PYTHON_SITE_PACKAGES)
        set(PYBIND11_CMAKE_MODULES_PATH "${PYTHON_SITE_PACKAGES}/pybind11/share/cmake/pybind11")
        if (NOT EXISTS "${PYBIND11_CMAKE_MODULES_PATH}")
            message(STATUS "pybind11 cmake modules not found at ${PYBIND11_CMAKE_MODULES_PATH}, is pybind11 installed ?")
        else ()
            message(STATUS "${PYBIND11_CMAKE_MODULES_PATH} added to CMAKE_PREFIX_PATH")
        endif ()

        list(APPEND CMAKE_PREFIX_PATH ${PYBIND11_CMAKE_MODULES_PATH})
    endif ()

    set(USE_EMBEDDED_MODULES ON)
    target_compile_definitions(PythonPlugin PRIVATE -DUSE_EMBEDDED_MODULES)
    include(wrapper/cccorelib/CMakeLists.txt)
    include(wrapper/pycc/CMakeLists.txt)

    add_subdirectory(include)
    add_subdirectory(src)
    add_subdirectory(ui)

    embed_cccorelib_in(${PROJECT_NAME})
    embed_pycc_in(${PROJECT_NAME})

    add_subdirectory(QCodeEditor)

    find_package(pybind11 CONFIG REQUIRED)
    find_package(Qt5 COMPONENTS WebEngineWidgets REQUIRED)

    target_link_libraries(PythonPlugin pybind11::embed Qt5::WebEngineWidgets)

    if (WIN32)
        set(CC_PYTHON_ENV_NAME "Python")
        set(CC_PLUGIN_INSTALL_DIR ${CLOUDCOMPARE_DEST_FOLDER}/plugins)
        set(CC_PYTHON_INSTALL_DIR "${CC_PLUGIN_INSTALL_DIR}/${CC_PYTHON_ENV_NAME}")

        deploy_qt_copy(${PROJECT_NAME} ${CLOUDCOMPARE_DEST_FOLDER})
        if (PLUGIN_PYTHON_COPY_ENV)
            copy_python_venv(${CC_PYTHON_INSTALL_DIR})
        endif()

        set(PYTHON_DLL "${PYTHON_PREFIX}/python${PYTHON_LIBRARY_SUFFIX}.dll")
        if (EXISTS "${PYTHON_DLL}")
            install(FILES ${PYTHON_DLL} DESTINATION ${CLOUDCOMPARE_DEST_FOLDER})
        endif()

        # If the docs have been built, we copy them in the install folder
        set(DOCUMENTATION_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/docs/_build")
        if (EXISTS "${DOCUMENTATION_FOLDER}/index.html")
            install(
                    DIRECTORY ${DOCUMENTATION_FOLDER}
                    DESTINATION ${CC_PYTHON_INSTALL_DIR}/docs
            )
        endif()
    endif ()
endif ()


