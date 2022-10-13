if(WIN32 OR APPLE)
  add_custom_target(
    pytest
    COMMAND ${PYTHON_EXECUTABLE} -m pytest --verbose --cloudcompare_exe
            ${CLOUDCOMPARE_EXECUTABLE_INSTALL_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmdline
  )
  add_custom_target(
          python_plugin_tests
          COMMAND ${CLOUDCOMPARE_EXECUTABLE_INSTALL_PATH}
          -SILENT
          -O ${CMAKE_CURRENT_LIST_DIR}/data/a_cloud.bin
          -PYTHON_TESTS ${CMAKE_CURRENT_LIST_DIR}/scripts
  )
else()
  add_custom_target(
    pytest
    COMMAND
      ${CMAKE_COMMAND} -E env CC_PLUGIN_PATH=${CC_PLUGIN_PATH_VALUE}
      ${PYTHON_EXECUTABLE} -m pytest --verbose --cloudcompare_exe
      ${CLOUDCOMPARE_EXECUTABLE_INSTALL_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmdline
  )
  add_custom_target(
          python_plugin_tests
          COMMAND ${CLOUDCOMPARE_EXECUTABLE_INSTALL_PATH}
          -SILENT
          -O ${CMAKE_CURRENT_LIST_DIR}/data/a_cloud.bin
          -PYTHON_TESTS ${CMAKE_CURRENT_LIST_DIR}/scripts
  )
endif()
