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

    set(COPY_ENV TRUE)

    set(CC_PYTHON_ENV_NAME "Python")
    SET(CC_PLUGIN_INSTALL_DIR ${CLOUDCOMPARE_DEST_FOLDER}/plugins)
    set(CC_PYTHON_INSTALL_DIR "${CC_PLUGIN_INSTALL_DIR}/${CC_PYTHON_ENV_NAME}")

    if (WIN32)
        if (COPY_ENV)
            if (DEFINED ENV{CONDA_PREFIX})
                message(STATUS "Conda environment found")
                string(REPLACE "\\" "/" CONDA_PREFIX $ENV{CONDA_PREFIX})
                INSTALL(
                        DIRECTORY "${CONDA_PREFIX}/"
                        DESTINATION "${CC_PYTHON_INSTALL_DIR}"
                        PATTERN "tests/*" EXCLUDE
                )
            elseif(DEFINED ENV{VIRTUAL_ENV})
                message(STATUS "Python virtualenv found")
                execute_process(
                        COMMAND "${PYTHON_EXECUTABLE}" -c "import sys; print(';'.join(p for p in sys.path if p.endswith('DLLs') or p.endswith('lib')), end='')"
                        RESUlT_VARIABLE PYTHON_RES
                        OUTPUT_VARIABLE PYTHON_OUT
                )
                string(REPLACE "\\" "/" VIRTUAL_ENV $ENV{VIRTUAL_ENV})
                INSTALL(DIRECTORY "$ENV{VIRTUAL_ENV}/" DESTINATION "${CC_PYTHON_INSTALL_DIR}")
                foreach(path ${PYTHON_OUT})
                    INSTALL(
                            DIRECTORY "${path}"
                            DESTINATION "${CC_PYTHON_INSTALL_DIR}"
                            PATTERN "tests/*" EXCLUDE
                    )
                endforeach()
            else ()
                message(WARNING "No Python Home found")
            endif ()
        endif()
        INSTALL(
                TARGETS pycc cccorelib
                DESTINATION "${CC_PYTHON_INSTALL_DIR}/Lib/site-packages"
        )
    endif()
endif ()
