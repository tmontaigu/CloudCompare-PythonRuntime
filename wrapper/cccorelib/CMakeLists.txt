cmake_minimum_required(VERSION 3.15..3.27)
project(pycccorelib LANGUAGES CXX)

include("${CMAKE_CURRENT_LIST_DIR}/../../cmake/Helpers.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/../../cmake/PythonEnvHelper.cmake")

# MASTER_PROJECT means that this CMakeLists is the top level CMakeLists which in
# our case means that this project is not built as a part of the PythonRuntime
# but as a standalone wheel
if(CMAKE_PROJECT_NAME STREQUAL pycccorelib)
  set(CCCORELIB_PYTHON_IS_MASTER_PROJECT ON)
else()
  set(CCCORELIB_PYTHON_IS_MASTER_PROJECT OFF)
endif()

add_subdirectory(src)

if(CCCORELIB_PYTHON_IS_MASTER_PROJECT)
  include(FetchContent)

  set(FETCHCONTENT_QUIET OFF)

  FetchContent_Declare(
    CCCoreLib
    GIT_REPOSITORY https://github.com/CloudCompare/CCCoreLib
    GIT_PROGRESS ON
    GIT_SHALLOW OFF
    GIT_TAG 08137fc62ec246f9bcfb32c337a550d086e0be21
  )

  FetchContent_GetProperties(CCCoreLib)
  string(TOLOWER "CCCoreLib" lowerCaseName)
  if(NOT CCCoreLib_POPULATED)
    FetchContent_Populate(CCCoreLib)
    # Non-capitalized names are important
    add_subdirectory(
      ${${lowerCaseName}_SOURCE_DIR} ${${lowerCaseName}_BINARY_DIR}
      EXCLUDE_FROM_ALL
    )
  endif()
endif()

if(CCCORELIB_PYTHON_IS_MASTER_PROJECT OR NOT PLUGIN_PYTHON_USE_EMBEDDED_MODULES)
  find_package(Python 3.9 COMPONENTS Interpreter Development REQUIRED)
  ensure_pybind11_cmake_module_is_in_path()
  find_package(pybind11 CONFIG REQUIRED)

  pybind11_add_module(cccorelib ${cccorelib_sources})
  target_link_libraries(cccorelib PRIVATE CCCoreLib)

  if(CCCORELIB_PYTHON_IS_MASTER_PROJECT)

    if(WIN32)
      run_windeployqt(cccorelib "$<TARGET_FILE:CCCoreLib>")
      install(
        DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/deployqt/"
        DESTINATION cccorelib
        PATTERN "*.lib" EXCLUDE
        PATTERN "*.exe" EXCLUDE
      )
      install(TARGETS CCCoreLib RUNTIME DESTINATION cccorelib)
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
