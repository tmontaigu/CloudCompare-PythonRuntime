# PYTHON_PREFIX & PYTHON_LIBRARY_SUFFIX come from pybind11 cmake

# This function sets a global PYTHON_BASE_PREFIX cache variable
function(getset_python_base_prefix)
  execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" "-c"
            "import sys;print(sys.base_prefix.replace('\\\\', '/'), end='')"
    OUTPUT_VARIABLE PYTHON_BASE_PREFIX
  )
  set(PYTHON_BASE_PREFIX
      "${PYTHON_BASE_PREFIX}"
      PARENT_SCOPE
  )
endfunction()

function(copy_python_venv INSTALL_DIR)
  message(
    DEBUG
    "Venv copy:
        PYTHON_BASE_PREFIX: ${PYTHON_BASE_PREFIX}
        PYTHON_PREFIX:      ${PYTHON_PREFIX}
        INSTALL_DIR:        ${INSTALL_DIR}"
  )
  message(DEBUG "COPYING venv from ${PYTHON_BASE_PREFIX} to ${INSTALL_DIR}")
  install(DIRECTORY "${PYTHON_BASE_PREFIX}/" DESTINATION "${INSTALL_DIR}")

  if(NOT "${PYTHON_BASE_PREFIX}" STREQUAL "${PYTHON_PREFIX}")
    install(DIRECTORY "${PYTHON_PREFIX}/Lib/site-packages/"
            DESTINATION "${INSTALL_DIR}/Lib/site-packages/"
    )
  endif()
endfunction()

function(copy_python_dll)
  message(
    DEBUG
    "Python DLL: = ${PYTHON_BASE_PREFIX}/python${PYTHON_LIBRARY_SUFFIX}.dll"
  )
  install(
    FILES "${PYTHON_BASE_PREFIX}/python${PYTHON_LIBRARY_SUFFIX}.dll"
          # install the python3 base dll as well because some libs will try to
          # find it (PySide and PyQT for example)
          "${PYTHON_BASE_PREFIX}/python3.dll"
    DESTINATION ${CLOUDCOMPARE_DEST_FOLDER}
  )
endfunction()

function(manage_windows_install)

  getset_python_base_prefix()

  if(PLUGIN_PYTHON_COPY_ENV)
    copy_python_venv(${CC_PYTHON_INSTALL_DIR})
  endif()

  copy_python_dll()

  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/docs/stubfiles/pycc.pyi"
                "${CMAKE_CURRENT_SOURCE_DIR}/docs/stubfiles/cccorelib.pyi"
          DESTINATION "${CC_PYTHON_INSTALL_DIR}/Lib/site-packages"
  )

  if(NOT PLUGIN_PYTHON_USE_EMBEDDED_MODULES)
    install(TARGETS pycc cccorelib
            DESTINATION "${CC_PYTHON_INSTALL_DIR}/Lib/site-packages"
    )
  endif()
endfunction()

function(run_windeployqt TARGET_NAME FILE_PATH)
  # Force finding Qt5 to have the Qt5::qmake thing later
  find_package(
    Qt5
    COMPONENTS Core
    REQUIRED
  )

  get_target_property(QMAKE_EXE Qt5::qmake IMPORTED_LOCATION)
  get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)

  find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")

  add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory deployqt
    COMMAND "${WINDEPLOYQT_EXECUTABLE}" "${FILE_PATH}" --dir
            "${CMAKE_CURRENT_BINARY_DIR}/deployqt"
  )
endfunction()
