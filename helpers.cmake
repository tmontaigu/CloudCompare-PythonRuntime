function(copy_python_venv install_dir)
    if (CONDA_PREFIX)
        INSTALL(
                DIRECTORY "${CONDA_PREFIX}/"
                DESTINATION "${install_dir}"
                PATTERN "tests/*" EXCLUDE
        )
    elseif (VENV_PREFIX)
        execute_process(
            COMMAND 
                "${PYTHON_EXECUTABLE}" 
                "-c" 
                "from distutils import sysconfig as s;print(s.BASE_PREFIX, end='');"
            RESULT_VARIABLE _PYTHON_SUCCESS
            OUTPUT_VARIABLE PYTHON_BASE_PREFIX
        )
        INSTALL(
            DIRECTORY "${PYTHON_BASE_PREFIX}/"
            DESTINATION "${install_dir}"
            PATTERN "*test*" EXCLUDE
        )
        INSTALL(
            DIRECTORY  "${VENV_PREFIX}/Lib/" 
            DESTINATION "${install_dir}/Lib"
            PATTERN "*test*" EXCLUDE
        )
    else ()
        message(FATAL_ERROR "Please use a virtual env (venv or conda)")
    endif ()
endfunction()


function (copy_python_dll)
    if (CONDA_PREFIX)
        set(PYTHON_DLL "${PYTHON_PREFIX}/python${PYTHON_LIBRARY_SUFFIX}.dll")
        install(FILES ${PYTHON_DLL} DESTINATION ${CLOUDCOMPARE_DEST_FOLDER})
    elseif (DEFINED ENV{VIRTUAL_ENV})
        execute_process(
            COMMAND 
                "${PYTHON_EXECUTABLE}" 
                "-c" 
                "from distutils import sysconfig as s;print(s.BASE_PREFIX, end='');"
            RESULT_VARIABLE _PYTHON_SUCCESS
            OUTPUT_VARIABLE PYTHON_BASE_PREFIX
        )
        install(FILES "${PYTHON_BASE_PREFIX}/python${PYTHON_LIBRARY_SUFFIX}.dll" DESTINATION ${CLOUDCOMPARE_DEST_FOLDER})
    endif()
endfunction()


function(get_python_sites_packages PYTHON_SITE_PACKAGES)
    execute_process(
        COMMAND 
            "${PYTHON_EXECUTABLE}" 
            "-c"   
            "from distutils import sysconfig as s;print(s.get_python_lib(plat_specific=True));"
        RESULT_VARIABLE _PYTHON_SUCCESS
        OUTPUT_VARIABLE _PYTHON_VALUES
        ERROR_VARIABLE _PYTHON_ERROR_VALUE
    )

    if (NOT _PYTHON_SUCCESS MATCHES 0)
        message("${_PYTHON_SUCCESS}")
        return()
    endif ()

    # Convert the process output into a list
    if (WIN32)
        string(REGEX REPLACE "\\\\" "/" _PYTHON_VALUES ${_PYTHON_VALUES})
    endif ()

    string(REGEX REPLACE "\n" "" _PYTHON_VALUES ${_PYTHON_VALUES})

    set(${PYTHON_SITE_PACKAGES} ${_PYTHON_VALUES} PARENT_SCOPE)
endfunction()

function(manage_windows_install)
    set(CC_PYTHON_ENV_NAME "Python")
    set(CC_PLUGIN_INSTALL_DIR ${CLOUDCOMPARE_DEST_FOLDER}/plugins)
    set(CC_PYTHON_INSTALL_DIR "${CC_PLUGIN_INSTALL_DIR}/${CC_PYTHON_ENV_NAME}")

    if (PLUGIN_PYTHON_COPY_ENV)
        copy_python_venv(${CC_PYTHON_INSTALL_DIR})
    endif()

    copy_python_dll()

    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/docs/stubfiles/pycc.pyi"
                  "${CMAKE_CURRENT_SOURCE_DIR}/docs/stubfiles/cccorelib.pyi"
            DESTINATION "${CC_PYTHON_INSTALL_DIR}/Lib/site-packages")

    if (NOT PLUGIN_PYTHON_USE_EMBEDDED_MODULES)
        install(TARGETS pycc cccorelib
                DESTINATION "${CC_PYTHON_INSTALL_DIR}/Lib/site-packages")
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