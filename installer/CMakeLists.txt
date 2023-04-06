if(WIN32 AND (CMAKE_BUILD_TYPE STREQUAL Release))
  add_custom_target(
    python_plugin_installer
    COMMENT "Building MSI installer..."
    COMMAND
      pwsh ./Create-Installer.ps1 -PLUGIN_STANDARD_QM3C2
      ${PLUGIN_STANDARD_QM3C2}
      "${CMAKE_INSTALL_PREFIX}/${CLOUDCOMPARE_DEST_FOLDER}"
    USES_TERMINAL
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
endif()
