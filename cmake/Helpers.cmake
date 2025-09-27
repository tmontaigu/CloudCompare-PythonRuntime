# This function sets a global PYTHON_BASE_PREFIX cache variable
function(getset_python_base_prefix)
  execute_process(
    COMMAND "${Python_EXECUTABLE}" "-c"
            "import sys;print(sys.base_prefix.replace('\\\\', '/'), end='')"
    OUTPUT_VARIABLE PYTHON_BASE_PREFIX
  )
  set(PYTHON_BASE_PREFIX
      "${PYTHON_BASE_PREFIX}"
      PARENT_SCOPE
  )
endfunction()

function(copy_python_env INSTALL_DIR)
  string(REPLACE "\\" "/" PYTHON_BASE_PREFIX "${PYTHON_BASE_PREFIX}")
  string(REPLACE "\\" "/" Python_SITELIB "${Python_SITELIB}")
  string(REPLACE "\\" "/" Python_STDARCH "${Python_STDARCH}")
  string(REPLACE "\\" "/" Python_STDLIB "${Python_STDLIB}")
  string(REPLACE "\\" "/" Python_RUNTIME_LIBRARY_DIRS "${Python_RUNTIME_LIBRARY_DIRS}")

  message(
    DEBUG
    "ENV copy:
        PYTHON_BASE_PREFIX: ${PYTHON_BASE_PREFIX}
        PYTHON_SITELIB:     ${Python_SITELIB}
        PYTHON_STDARCH:     ${Python_STDARCH}
        PYTHON_STDLIB:      ${Python_STDLIB}
        PYTHON_LIBRARIES:   ${Python_RUNTIME_LIBRARY_DIRS}"
  )
  message(DEBUG "COPYING venv from ${Python_SITELIB}/ to ${INSTALL_DIR}/Lib/site-packages/")
  install(DIRECTORY "${PYTHON_BASE_PREFIX}/" DESTINATION "${INSTALL_DIR}")

  install(DIRECTORY "${Python_SITELIB}/"
        DESTINATION "${INSTALL_DIR}/Lib/site-packages/"
  )
endfunction()

function(copy_python_dll)
  message(
    DEBUG
    "Python DLL: = ${Python_RUNTIME_LIBRARY_DIRS}/python${Python_VERSION_MAJOR}${Python_VERSION_MINOR}.dll"
  )
  install(
    FILES "${Python_RUNTIME_LIBRARY_DIRS}/python${Python_VERSION_MAJOR}${Python_VERSION_MINOR}.dll"
          # install the python3 base dll as well because some libs will try to
          # find it (PySide and PyQT for example)
          "${Python_RUNTIME_LIBRARY_DIRS}/python${Python_VERSION_MAJOR}.dll"
    DESTINATION ${CLOUDCOMPARE_DEST_FOLDER}
  )
endfunction()

function(manage_windows_install)

  getset_python_base_prefix()

  if(PLUGIN_PYTHON_COPY_ENV)
    copy_python_env(${CC_PYTHON_INSTALL_DIR})
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
