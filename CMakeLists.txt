option(PLUGIN_PYTHON "Install example plugin" OFF)

if (PLUGIN_PYTHON)
    project(PythonPlugin)

    AddPlugin(NAME ${PROJECT_NAME})

    target_compile_definitions(PythonPlugin PRIVATE -DQT_NO_KEYWORDS)

    add_subdirectory(wrapper)

    add_subdirectory(include)
    add_subdirectory(src)
    add_subdirectory(ui)

    add_subdirectory(QCodeEditor)

    find_package(pybind11 CONFIG REQUIRED)
    target_link_libraries(PythonPlugin pybind11::embed)

    set(CC_PYTHON_ENV_NAME Python)
    SET(CC_PLUGIN_INSTALL_DIR CloudCompare/plugins)
    set(CC_PYTHON_INSTALL_DIR "${CC_PLUGIN_INSTALL_DIR}/${CC_PYTHON_ENV_NAME}")

    find_program(PYTHON_EXE python REQUIRED HINT ENV PATH)

    message(STATUS "Python: '${PYTHON_EXE}'")

    if (DEFINED ENV{CONDA_PREFIX})
        message(STATUS "Conda environment found")
        set(CC_PYTHON_HOME $ENV{CONDA_PREFIX})
        INSTALL(
                DIRECTORY "${CC_PYTHON_HOME}/"
                DESTINATION "${CC_PYTHON_INSTALL_DIR}"
                PATTERN "tests/*" EXCLUDE
        )
    elseif(DEFINED ENV{VIRTUAL_ENV})
        message(STATUS "Python virtualenv found")
        set(CC_PYTHON_HOME $ENV{VIRTUAL_ENV})
        execute_process(
                COMMAND "${PYTHON_EXE}" -c "import sys; paths = print(';'.join(p for p in sys.path if p.endswith('DLLs') or p.endswith('lib')), end='')"
                RESUlT_VARIALBLE PYTHON_RES
                OUTPUT_VARIABLE PYTHON_OUT
        )
        INSTALL(DIRECTORY "${CC_PYTHON_HOME}/" DESTINATION "${CC_PYTHON_INSTALL_DIR}")
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

    if (DEFINED CC_PYTHON_HOME)
        INSTALL(TARGETS pycc DESTINATION "${CC_PYTHON_INSTALL_DIR}/Lib/site-packages")
    endif()
endif ()
