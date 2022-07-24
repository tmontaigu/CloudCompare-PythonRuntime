if(WIN32 OR APPLE)
  add_custom_target(
    python_plugin_documentation
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_LIST_DIR}/__build
    COMMAND doxygen qCC_db_doxygen_file
    COMMAND doxygen ./CCCoreLib_doxygen_file
    COMMAND ${CLOUDCOMPARE_EXECUTABLE_INSTALL_PATH} -SILENT -PYTHON_SCRIPT
            build_doc.py -b html . _build
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    USES_TERMINAL
  )
else()
  add_custom_target(
    python_plugin_documentation
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_LIST_DIR}/__build
    COMMAND doxygen qCC_db_doxygen_file
    COMMAND doxygen ./CCCoreLib_doxygen_file
    COMMAND
      ${CMAKE_COMMAND} -E env CC_PLUGIN_PATH=${CC_PLUGIN_PATH_VALUE}
      ${CLOUDCOMPARE_EXECUTABLE_INSTALL_PATH} -SILENT -PYTHON_SCRIPT
      build_doc.py -b html . _build
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    USES_TERMINAL
  )
endif()
