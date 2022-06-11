cmake_minimum_required(VERSION 3.14)
project(pycccorelib LANGUAGES CXX)

# MASTER_PROJECT means that this CMakeLists is the top level CMakeLists which in
# our case means that this project is not built as a part of the PythonPlugin
# but as a standalone wheel
if(CMAKE_PROJECT_NAME STREQUAL pycccorelib)
  set(CCCORELIB_PYTHON_IS_MASTER_PROJECT ON)
else()
  set(CCCORELIB_PYTHON_IS_MASTER_PROJECT OFF)
endif()

add_subdirectory(src)

if(CCCORELIB_PYTHON_IS_MASTER_PROJECT)
  if(WIN32)
    if(NOT PYTHON_EXECUTABLE)
      if(DEFINED ENV{CONDA_PREFIX})
        list(INSERT CMAKE_PREFIX_PATH 0
             "$ENV{CONDA_PREFIX}/Library/share/cmake"
        )
        set(PYTHON_EXECUTABLE "$ENV{CONDA_PREFIX}/python.exe")
      elseif(DEFINED ENV{VIRTUAL_ENV})
        set(PYTHON_EXECUTABLE "$ENV{VENV_PREFIX}/Scripts/python.exe")
      endif()
    endif()
  endif()

  if(PYTHON_EXECUTABLE)
    execute_process(
      COMMAND "${PYTHON_EXECUTABLE}" "-m" "pybind11" "--cmake"
      RESULT_VARIABLE _PYTHON_SUCCESS
      OUTPUT_VARIABLE PYBIND11_CMAKE_MODULES_PATH
    )

    if(_PYTHON_SUCCESS MATCHES 0)
      string(REGEX REPLACE "\n" "" PYBIND11_CMAKE_MODULES_PATH
                           ${PYBIND11_CMAKE_MODULES_PATH}
      )
      list(INSERT CMAKE_PREFIX_PATH 0 "${PYBIND11_CMAKE_MODULES_PATH}")
    else()
      message(
        WARNING "Failed to get pybind11 cmake prefix path ${_PYTHON_SUCCESS}"
      )
    endif()
  endif()

  include(FetchContent)

  set(FETCHCONTENT_QUIET OFF)

  FetchContent_Declare(
    CCCoreLib
    GIT_REPOSITORY https://github.com/CloudCompare/CCCoreLib
    GIT_PROGRESS ON
    GIT_SHALLOW OFF
    GIT_TAG f0d54507ff3ac7e2f579dba8b07cc2bdbb392163
  )

  FetchContent_GetProperties(CCCoreLib)
  string(TOLOWER "CCCoreLib" lcName)
  if(NOT CCCoreLib_POPULATED)
    FetchContent_Populate(CCCoreLib)
    # Non-capitalized names are important
    add_subdirectory(
      ${${lcName}_SOURCE_DIR} ${${lcName}_BINARY_DIR} EXCLUDE_FROM_ALL
    )
  endif()
endif()

if(CCCORELIB_PYTHON_IS_MASTER_PROJECT OR NOT PLUGIN_PYTHON_USE_EMBEDDED_MODULES)
  find_package(pybind11 CONFIG REQUIRED)
  pybind11_add_module(cccorelib ${cccorelib_sources})
  target_link_libraries(cccorelib PRIVATE CCCoreLib)

  if(CCCORELIB_PYTHON_IS_MASTER_PROJECT)

    if(WIN32)
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
        TARGET cccorelib
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory deployqt
        COMMAND "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:CCCoreLib>" --dir
                "${CMAKE_CURRENT_BINARY_DIR}/deployqt"
      )

      install(TARGETS CCCoreLib RUNTIME DESTINATION cccorelib)
      install(
        DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/deployqt/"
        DESTINATION cccorelib
        PATTERN "*.lib" EXCLUDE
        PATTERN "*.exe" EXCLUDE
      )
    elseif(APPLE)
      install(TARGETS CCCoreLib)
      set_property(
        TARGET cccorelib
        PROPERTY INSTALL_RPATH
                 "@loader_path/../../.." # or "${PYTHON_SITE_PACKAGES}/../..""
      )
    else()
      install(TARGETS CCCoreLib)
      set_property(
        TARGET cccorelib
        PROPERTY INSTALL_RPATH
                 "$ORIGIN/../../.." # or "${PYTHON_SITE_PACKAGES}/../..""
      )
    endif()
    install(TARGETS cccorelib LIBRARY DESTINATION cccorelib)
  endif()
endif()

function(embed_cccorelib_in target)
  target_sources(${target} PRIVATE ${cccorelib_sources})
  target_link_libraries(${target} CCCoreLib)
endfunction(embed_cccorelib_in)

# Cache variables so embed_cccorelib_in can be used in parent projects
set(cccorelib_sources
    ${cccorelib_sources}
    CACHE INTERNAL ""
)
