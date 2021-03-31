function(copy_python_venv install_dir)
    if (DEFINED ENV{CONDA_PREFIX})
        message(STATUS "Conda environment found")
        string(REPLACE "\\" "/" CONDA_PREFIX $ENV{CONDA_PREFIX})
        INSTALL(
                DIRECTORY "${CONDA_PREFIX}/"
                DESTINATION "${install_dir}"
                PATTERN "tests/*" EXCLUDE
        )
    elseif (DEFINED ENV{VIRTUAL_ENV})
        message(STATUS "Python virtualenv found")
        execute_process(
                COMMAND "${PYTHON_EXECUTABLE}" -c "import sys; print(';'.join(p for p in sys.path if p.endswith('DLLs') or p.endswith('lib')), end='')"
                RESUlT_VARIABLE PYTHON_RES
                OUTPUT_VARIABLE PYTHON_OUT
        )
        string(REPLACE "\\" "/" VIRTUAL_ENV $ENV{VIRTUAL_ENV})
        INSTALL(DIRECTORY "$ENV{VIRTUAL_ENV}/" DESTINATION "${install_dir}")
        foreach (path ${PYTHON_OUT})
            INSTALL(
                    DIRECTORY "${path}"
                    DESTINATION "${install_dir}"
                    PATTERN "tests/*" EXCLUDE
            )
        endforeach ()
    else ()
        message(WARNING "No Python Home found")
    endif ()
endfunction()


function(get_python_sites_packages PYTHON_SITE_PACKAGES)
    execute_process(
            COMMAND "python" "-c" "from distutils import sysconfig as s;
print(s.get_python_lib(plat_specific=True));
"
    RESULT_VARIABLE _PYTHON_SUCCESS
    OUTPUT_VARIABLE _PYTHON_VALUES
    ERROR_VARIABLE _PYTHON_ERROR_VALUE)

    if (NOT _PYTHON_SUCCESS MATCHES 0)
        message("${_PYTHON_SUCCESS} zdf ${PYTHON_EXECUTABLE}")
        return()
    endif ()

    # Convert the process output into a list
    if (WIN32)
        string(REGEX REPLACE "\\\\" "/" _PYTHON_VALUES ${_PYTHON_VALUES})
    endif ()

    string(REGEX REPLACE "\n" "" _PYTHON_VALUES ${_PYTHON_VALUES})

    set(${PYTHON_SITE_PACKAGES} ${_PYTHON_VALUES} PARENT_SCOPE)
endfunction()

# Tries to find pybind11 in the site-packages
# because it bundles the .cmake files the
# find_package command needs
function(try_append_pybind11_cmake_module_to_modules_path)
    set(PYTHON_SITE_PACKAGES "")
    get_python_sites_packages(PYTHON_SITE_PACKAGES)
    set(PYBIND11_CMAKE_MODULES_PATH "${PYTHON_SITE_PACKAGES}/pybind11/share/cmake/pybind11")
    if (NOT EXISTS "${PYBIND11_CMAKE_MODULES_PATH}")
        message(STATUS "pybind11 cmake modules not found at ${PYBIND11_CMAKE_MODULES_PATH}, is pybind11 installed ?")
    else ()
        message(STATUS "${PYBIND11_CMAKE_MODULES_PATH} added to CMAKE_PREFIX_PATH")
    endif ()

    list(APPEND CMAKE_PREFIX_PATH ${PYBIND11_CMAKE_MODULES_PATH})
endfunction()


function(manage_windows_install)
    set(CC_PYTHON_ENV_NAME "Python")
    set(CC_PLUGIN_INSTALL_DIR ${CLOUDCOMPARE_DEST_FOLDER}/plugins)
    set(CC_PYTHON_INSTALL_DIR "${CC_PLUGIN_INSTALL_DIR}/${CC_PYTHON_ENV_NAME}")

    if (PLUGIN_PYTHON_COPY_ENV)
        copy_python_venv(${CC_PYTHON_INSTALL_DIR})
    endif ()

    set(PYTHON_DLL "${PYTHON_PREFIX}/python${PYTHON_LIBRARY_SUFFIX}.dll")
    if (EXISTS "${PYTHON_DLL}")
        install(FILES ${PYTHON_DLL} DESTINATION ${CLOUDCOMPARE_DEST_FOLDER}/LIB/site-packages)
    endif ()

    if (NOT USE_EMBEDDED_MODULES)
        install(TARGETS pycc cccorelib
                DESTINATION "${CC_PYTHON_INSTALL_DIR}/Lib/site-packages")
    endif ()

    # If the docs have been built, we copy them in the install folder
    set(DOCUMENTATION_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/docs/_build")
    if (EXISTS "${DOCUMENTATION_FOLDER}/index.html")
        install(
                DIRECTORY "${DOCUMENTATION_FOLDER}/"
                DESTINATION ${CC_PYTHON_INSTALL_DIR}/docs
        )
    endif ()
endfunction()