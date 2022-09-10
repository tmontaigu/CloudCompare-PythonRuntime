if(WIN32 OR APPLE)
  add_custom_target(
    pytest
    COMMAND ${PYTHON_EXECUTABLE} -m pytest --verbose --cloudcompare_exe
            ${CLOUDCOMPARE_EXECUTABLE_INSTALL_PATH} ${CMAKE_CURRENT_LIST_DIR}
  )
else()
  add_custom_target(
    pytest
    COMMAND
      ${CMAKE_COMMAND} -E env CC_PLUGIN_PATH=${CC_PLUGIN_PATH_VALUE}
      ${PYTHON_EXECUTABLE} -m pytest --verbose --cloudcompare_exe
      ${CLOUDCOMPARE_EXECUTABLE_INSTALL_PATH} ${CMAKE_CURRENT_LIST_DIR}
  )
endif()
