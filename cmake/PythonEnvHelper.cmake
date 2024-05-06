macro(ensure_pybind11_cmake_module_is_in_path)
  execute_process(
    COMMAND "${Python_EXECUTABLE}" "-m" "pybind11" "--cmake"
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