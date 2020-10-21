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
