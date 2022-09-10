macro(ensure_pybind11_cmake_module_is_in_path)
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
endmacro()

macro(set_python_executable_from_current_venv)
  if(WIN32)
    if(DEFINED ENV{CONDA_PREFIX})
      list(INSERT CMAKE_PREFIX_PATH 0 "$ENV{CONDA_PREFIX}/Library/share/cmake")
      set(PYTHON_EXECUTABLE "$ENV{CONDA_PREFIX}/python.exe")
    elseif(DEFINED ENV{VIRTUAL_ENV})
      set(PYTHON_EXECUTABLE "$ENV{VIRTUAL_ENV}/Scripts/python.exe")
    endif()
  else()
    if(DEFINED ENV{CONDA_PREFIX})
      set(PYTHON_EXECUTABLE "$ENV{CONDA_PREFIX}/bin/python")
    elseif(DEFINED ENV{VIRTUAL_ENV})
      set(PYTHON_EXECUTABLE "$ENV{VIRTUAL_ENV}/bin/python")
    else()
      set(PYTHON_EXECUTABLE "python3")
    endif()
  endif()
endmacro()
