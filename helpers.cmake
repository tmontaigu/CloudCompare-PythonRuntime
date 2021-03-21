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
