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


# TODO: This is very much like DeployQt found in cloudcompare's cmake folder
#       however DeployQT only works for executable targets and not lib targets
#       because it adds .exe or .app, I believe DeployQt can be modified to work with
#       lib as targets, in the meantime we have our own similar thing
function(deploy_qt_copy target dest)
    get_target_property(qmake_location Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(qt5_bin_dir ${qmake_location} DIRECTORY)


    find_program(win_deploy_qt windeployqt HINTS "${qt5_bin_dir}")

    if (NOT EXISTS "${win_deploy_qt}")
        message(FATAL_ERROR "windeployqt not found in ${qt5_bin_dir}")
    endif ()

    set(temp_dir "${CMAKE_CURRENT_BINARY_DIR}/deployqt")
    add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND "${win_deploy_qt}"
            $<TARGET_FILE:${PROJECT_NAME}>
            --no-angle
            --no-opengl-sw
            --no-quick-import
            --no-system-d3d-compiler
            --concurrent
            --verbose=1
            --dir ${temp_dir}
            VERBATIM
    )

    install(
            DIRECTORY ${temp_dir}/
            DESTINATION ${dest}
    )

endfunction()