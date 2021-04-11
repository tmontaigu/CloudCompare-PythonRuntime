function(getset_python_base_prefix)
    execute_process(
        COMMAND 
            "${PYTHON_EXECUTABLE}" 
            "-c" 
            "import sys;print(sys.base_prefix, end='');"
        OUTPUT_VARIABLE PYTHON_BASE_PREFIX
    )
    set(PYTHON_BASE_PREFIX "${PYTHON_BASE_PREFIX}" PARENT_SCOPE)
endfunction()

function(copy_python_venv install_dir)
    install(
        DIRECTORY "${PYTHON_BASE_PREFIX}/"
        DESTINATION "${install_dir}"
        PATTERN "*tests*" EXCLUDE
    )

    if (NOT "${PYTHON_BASE_PREFIX}" STREQUAL "${PYTHON_PREFIX}")
        install(
            DIRECTORY "${PYTHON_PREFIX}/Lib/site-packages/"
            DESTINATION "${install_dir}/Lib/site-packages/"
            PATTERN "*tests*" EXCLUDE
        )
    endif()
endfunction()


function (copy_python_dll)
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

    # If the docs have been built, we copy them in the install folder
    set(DOCUMENTATION_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/docs/_build")
    if (EXISTS "${DOCUMENTATION_FOLDER}/index.html")
        install(
            DIRECTORY "${DOCUMENTATION_FOLDER}/"
            DESTINATION ${CC_PYTHON_INSTALL_DIR}/docs
        )
    endif()
endfunction()