# PYTHON_PREFIX & PYTHON_LIBRARY_SUFFIX come from pybind11 cmake

# This function sets a global PYTHON_BASE_PREFIX cache variable
function(getset_python_base_prefix)
    execute_process(
        COMMAND 
            "${PYTHON_EXECUTABLE}" 
            "-c"
            "import sys;print(sys.base_prefix.replace('\\\\', '/'), end='')"
        OUTPUT_VARIABLE PYTHON_BASE_PREFIX
    )
    set(PYTHON_BASE_PREFIX "${PYTHON_BASE_PREFIX}" PARENT_SCOPE)
endfunction()

function(copy_python_venv INSTALL_DIR)
    message(DEBUG "Venv copy:
        PYTHON_BASE_PREFIX: ${PYTHON_BASE_PREFIX}
        PYTHON_PREFIX:      ${PYTHON_PREFIX}
        INSTALL_DIR:        ${INSTALL_DIR}")
    message(DEBUG "COPYING venv from ${PYTHON_BASE_PREFIX} to ${INSTALL_DIR}")
    install(
        DIRECTORY "${PYTHON_BASE_PREFIX}/"
        DESTINATION "${INSTALL_DIR}"
    )

    if (NOT "${PYTHON_BASE_PREFIX}" STREQUAL "${PYTHON_PREFIX}")
        install(
            DIRECTORY "${PYTHON_PREFIX}/Lib/site-packages/"
            DESTINATION "${INSTALL_DIR}/Lib/site-packages/"
        )
    endif()
endfunction()


function (copy_python_dll)
    message(DEBUG "Python DLL: = ${PYTHON_BASE_PREFIX}/python${PYTHON_LIBRARY_SUFFIX}.dll")
    install(
        FILES "${PYTHON_BASE_PREFIX}/python${PYTHON_LIBRARY_SUFFIX}.dll" 
        DESTINATION ${CLOUDCOMPARE_DEST_FOLDER}
    )
endfunction()


function(manage_windows_install)
    set(CC_PYTHON_ENV_NAME "Python")
    set(CC_PLUGIN_INSTALL_DIR ${CLOUDCOMPARE_DEST_FOLDER}/plugins)
    set(CC_PYTHON_INSTALL_DIR "${CC_PLUGIN_INSTALL_DIR}/${CC_PYTHON_ENV_NAME}")

    getset_python_base_prefix()

    if (PLUGIN_PYTHON_COPY_ENV)
        copy_python_venv(${CC_PYTHON_INSTALL_DIR})
    endif()

    copy_python_dll()

    install(
        FILES "${CMAKE_CURRENT_SOURCE_DIR}/docs/stubfiles/pycc.pyi"
                  "${CMAKE_CURRENT_SOURCE_DIR}/docs/stubfiles/cccorelib.pyi"
        DESTINATION "${CC_PYTHON_INSTALL_DIR}/Lib/site-packages"
    )

    if (NOT PLUGIN_PYTHON_USE_EMBEDDED_MODULES)
        install(
            TARGETS pycc cccorelib
            DESTINATION "${CC_PYTHON_INSTALL_DIR}/Lib/site-packages"
        )
    endif()
endfunction()