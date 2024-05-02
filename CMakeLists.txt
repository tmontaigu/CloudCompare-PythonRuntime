cmake_minimum_required(VERSION 3.14..3.27)

include(cmake/Helpers.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/PythonEnvHelper.cmake)
include(cmake/CloudCompareVariables.cmake)

option(PLUGIN_PYTHON "Install Python Plugin" OFF)

if(PLUGIN_PYTHON)
  project(PythonRuntime)

  addplugin(NAME ${PROJECT_NAME})

  option(PLUGIN_PYTHON_USE_EMBEDDED_MODULES
         "Should the Python wrapper libs be embedded in the plugin" ON
  )
  mark_as_advanced(PLUGIN_PYTHON_USE_EMBEDDED_MODULES)

  if(WIN32)
    option(PLUGIN_PYTHON_COPY_ENV
           "Should the content of the current python venv be copied on install"
           ON
    )
    mark_as_advanced(PLUGIN_PYTHON_COPY_ENV)
  endif()

  if(PYTHON_EXECUTABLE)
    ensure_pybind11_cmake_module_is_in_path()
  endif()

  find_package(Python COMPONENTS Interpreter Development)
  find_package(pybind11 CONFIG)

  add_subdirectory(wrapper)
  add_subdirectory(src)

  if(PLUGIN_PYTHON_USE_EMBEDDED_MODULES)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_EMBEDDED_MODULES)

    embed_cccorelib_in(${PROJECT_NAME})
    embed_pycc_in(${PROJECT_NAME})
  endif()

  target_link_libraries(${PROJECT_NAME} pybind11::embed)
  set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON CXX_VISIBILITY_PRESET hidden)

  add_subdirectory(tests)
  add_subdirectory(docs)
  add_subdirectory(installer)

  if(WIN32)
    manage_windows_install()
  elseif(UNIX AND NOT APPLE)
    if(NOT PLUGIN_PYTHON_USE_EMBEDDED_MODULES)
      installsharedlibrary(TARGET cccorelib)
      installsharedlibrary(TARGET pycc)
    endif()
  endif()
endif()
