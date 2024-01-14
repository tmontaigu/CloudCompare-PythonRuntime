cmake_minimum_required(VERSION 3.14)
project(pycc LANGUAGES CXX)

include("${CMAKE_CURRENT_LIST_DIR}/../../cmake/Helpers.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/../../cmake/PythonEnvHelper.cmake")

# MASTER_PROJECT means that this CMakeLists is the top level CMakeLists which in
# our case means that this project is not built as a part of the PythonRuntime
# but as a standalone wheel
if(CMAKE_PROJECT_NAME STREQUAL pycc)
  set(PYCC_IS_MASTER_PROJECT ON)
  set(DEFINE_PYCC_RUNTIME OFF)
else()
  set(PYCC_IS_MASTER_PROJECT OFF)
  option(DEFINE_PYCC_RUNTIME "Should the runtime functions be included" ON)
endif()

if(PYCC_IS_MASTER_PROJECT)
  message(DEBUG "PYTHON_EXECUTABLE: ${PYTHON_EXECUTABLE}")
  if(NOT PYTHON_EXECUTABLE)
    set_python_executable_from_current_venv()
  endif()

  message(DEBUG "PYTHON_EXECUTABLE: ${PYTHON_EXECUTABLE}")
  if(PYTHON_EXECUTABLE)
    ensure_pybind11_cmake_module_is_in_path()
  endif()

  include(FetchContent)

  set(FETCHCONTENT_QUIET OFF)

  FetchContent_Declare(
    CloudCompare
    GIT_REPOSITORY https://github.com/CloudCompare/CloudCompare
    GIT_SUBMODULES "libs/qCC_db/extern/CCCoreLib"
    GIT_PROGRESS ON
    GIT_TAG d17314ca8538e187bf12d4ebe97525d61886b735
    GIT_SHALLOW OFF
  )

  FetchContent_GetProperties(CloudCompare)
  string(TOLOWER "CloudCompare" lowerCaseName)
  if(NOT CloudCompare_POPULATED)
    FetchContent_Populate(CloudCompare)
    set(CLOUDCOMPARE_SOURCES_ROOT ${${lowerCaseName}_SOURCE_DIR})
    add_subdirectory(
      ${${lowerCaseName}_SOURCE_DIR} ${${lowerCaseName}_BINARY_DIR}
      EXCLUDE_FROM_ALL
    )
  endif()
endif()

find_package(pybind11 CONFIG REQUIRED)

add_subdirectory(src)
add_subdirectory(plugins)

if(PYCC_IS_MASTER_PROJECT OR NOT PLUGIN_PYTHON_USE_EMBEDDED_MODULES)

  pybind11_add_module(pycc ${pycc_sources})
  target_link_libraries(pycc PRIVATE QCC_DB_LIB QCC_GL_LIB QCC_IO_LIB)

  target_include_directories(
    pycc PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../cccorelib/src"
  )

  if(DEFINE_PYCC_RUNTIME)
    target_link_libraries(pycc PRIVATE PythonRuntime)
    target_compile_definitions(pycc PRIVATE DEFINE_PYCC_RUNTIME)
  endif()

  if(PYCC_IS_MASTER_PROJECT)
    target_compile_definitions(pycc PRIVATE PYCC_STAND_ALONE)

    if(WIN32)
      install(
        TARGETS QCC_DB_LIB
                QCC_GL_LIB
                QCC_IO_LIB
                CCCoreLib
                CC_FBO_LIB
        RUNTIME DESTINATION pycc
      )
      # Just to be sure we have everything
      run_windeployqt(pycc "$<TARGET_FILE:CCCoreLib>")
      run_windeployqt(pycc "$<TARGET_FILE:QCC_DB_LIB>")
      run_windeployqt(pycc "$<TARGET_FILE:QCC_GL_LIB>")
      run_windeployqt(pycc "$<TARGET_FILE:QCC_IO_LIB>")
      run_windeployqt(pycc "$<TARGET_FILE:pycc>")

      if(PYCC_PLUGIN_WRAPPER_LIST)
        run_windeployqt(pycc "$<TARGET_FILE:CCPluginAPI>")
      endif()

      install(
        DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/deployqt/"
        DESTINATION pycc
        PATTERN "*.lib" EXCLUDE
        PATTERN "*.exe" EXCLUDE
      )
    elseif(APPLE)
      install(TARGETS QCC_DB_LIB QCC_GL_LIB QCC_IO_LIB CC_FBO_LIB)
      set_property(
        TARGET pycc
        PROPERTY INSTALL_RPATH
                 "@loader_path/../../.." # or "${PYTHON_SITE_PACKAGES}/../..""
      )
    else()
      # Its a bit hacky but its the way we have to make it work as the
      # CMakeLists from CloudCompare sets the RPATH to a absolute path that does
      # not work in our case
      foreach(target_name IN ITEMS QCC_DB_LIB QCC_GL_LIB QCC_IO_LIB CC_FBO_LIB)
        set_property(TARGET "${target_name}" PROPERTY INSTALL_RPATH "$ORIGIN")
      endforeach()
      install(TARGETS QCC_DB_LIB QCC_GL_LIB QCC_IO_LIB CC_FBO_LIB)
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
