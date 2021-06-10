cmake_minimum_required(VERSION 3.14)
project(pycc LANGUAGES CXX)

# MASTER_PROJECT means that this CMakeLists is the top level CMakeLists which in
# our case means that this project is not built as a part of the PythonPlugin
# but as a standalone wheel
if(CMAKE_PROJECT_NAME
   STREQUAL
   pycc
)
  set(PYCC_IS_MASTER_PROJECT ON)
  set(DEFINE_PYCC_RUNTIME OFF)
else()
  set(PYCC_IS_MASTER_PROJECT OFF)
  option(
    DEFINE_PYCC_RUNTIME
    "Should the runtime functions be included"
    ON
  )
endif()

add_subdirectory(src)

if(PYCC_IS_MASTER_PROJECT)
  if(WIN32)
    if(NOT PYTHON_EXECUTABLE)
      if(DEFINED ENV{CONDA_PREFIX})
        list(
          INSERT
          CMAKE_PREFIX_PATH
          0
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

    if(_PYTHON_SUCCESS
       MATCHES
       0
    )
      string(
        REGEX
        REPLACE "\n"
                ""
                PYBIND11_CMAKE_MODULES_PATH
                ${PYBIND11_CMAKE_MODULES_PATH}
      )
      list(
        INSERT
        CMAKE_PREFIX_PATH
        0
        "${PYBIND11_CMAKE_MODULES_PATH}"
      )
    else()
      message(
        WARNING "Failed to get pybind11 cmake prefix path ${_PYTHON_SUCCESS}"
      )
    endif()
  endif()

  include(FetchContent)

  set(FETCHCONTENT_QUIET OFF)

  FetchContent_Declare(
    CloudCompare
    GIT_REPOSITORY https://github.com/CloudCompare/CloudCompare
    GIT_SUBMODULES "libs/qCC_db/extern/CCCoreLib"
    GIT_PROGRESS ON
    GIT_TAG a4ebd61e903c986e18d468df67320cce6378e78f
    GIT_SHALLOW OFF
  )

  FetchContent_MakeAvailable(CloudCompare)
endif()

if(PYCC_IS_MASTER_PROJECT OR NOT PLUGIN_PYTHON_USE_EMBEDDED_MODULES)
  function(
    run_windeployqt
    TARGET_NAME
    FILE_PATH
  )
    add_custom_command(
      TARGET ${TARGET_NAME}
      POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E make_directory deployqt
      COMMAND "${WINDEPLOYQT_EXECUTABLE}" "${FILE_PATH}" --dir
              "${CMAKE_CURRENT_BINARY_DIR}/deployqt"
    )
  endfunction()

  find_package(
    pybind11
    CONFIG
    REQUIRED
  )

  pybind11_add_module(pycc ${pycc_sources})
  target_link_libraries(
    pycc
    PRIVATE QCC_DB_LIB
            QCC_GL_LIB
            QCC_IO_LIB
  )

  target_include_directories(pycc PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../cccorelib/src")
  message("LOL: ${CMAKE_CURRENT_SOURCE_DIR}/../cccorelib/src")
  
  if(DEFINE_PYCC_RUNTIME)
    target_include_directories(pycc PRIVATE ../../include)
    target_link_libraries(pycc PRIVATE PythonPlugin)
    target_compile_definitions(pycc PRIVATE DEFINE_PYCC_RUNTIME)
  endif()

  if(PYCC_IS_MASTER_PROJECT)
    if(WIN32)
      install(
        TARGETS QCC_DB_LIB
                QCC_GL_LIB
                QCC_IO_LIB
                CCCoreLib
                CC_FBO_LIB
        RUNTIME DESTINATION pycc
      )
      find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
      # Just to be sure we have everything
      run_windeployqt(pycc "$<TARGET_FILE:CCCoreLib>")
      run_windeployqt(pycc "$<TARGET_FILE:QCC_DB_LIB>")
      run_windeployqt(pycc "$<TARGET_FILE:QCC_GL_LIB>")
      run_windeployqt(pycc "$<TARGET_FILE:QCC_IO_LIB>")
      run_windeployqt(pycc "$<TARGET_FILE:pycc>")
      install(
        DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/deployqt/"
        DESTINATION pycc
        PATTERN "*.lib" EXCLUDE
        PATTERN "*.exe" EXCLUDE
      )
    elseif(APPLE)
      set_property(
        TARGET pycc
        PROPERTY INSTALL_RPATH
                 "@loader_path/../../.." # or "${PYTHON_SITE_PACKAGES}/../..""
      )
    else()
      # Its a bit hacky but its the way we have to make it work as the
      # CMakeLists from CloudCompare sets the RPATH to a absolute path that does
      # not work in our case
      foreach(
        target_name IN
        ITEMS QCC_DB_LIB
              QCC_GL_LIB
              QCC_IO_LIB
              CC_FBO_LIB
      )
        set_property(TARGET "${target_name}" PROPERTY INSTALL_RPATH "$ORIGIN")
      endforeach()
      set_property(
        TARGET pycc
        PROPERTY INSTALL_RPATH
                 "$ORIGIN/../../.." # or "${PYTHON_SITE_PACKAGES}/../..""
      )
    endif()
    install(TARGETS pycc LIBRARY DESTINATION pycc)
  endif()
endif()

function(embed_pycc_in target)
  target_sources(${target} PRIVATE ${pycc_sources})
  target_include_directories(${target} PRIVATE ${cccorelib_inc})
  if(DEFINE_PYCC_RUNTIME)
    target_compile_definitions(${target} PRIVATE DEFINE_PYCC_RUNTIME)
  endif()
endfunction(embed_pycc_in)

# Cache variables so embed_pycc_in can be used in parent projects
set(pycc_sources
    ${pycc_sources}
    CACHE INTERNAL ""
)

set(cccorelib_inc
  "${CMAKE_CURRENT_SOURCE_DIR}/../cccorelib/src"
  CACHE INTERNAL ""
)
