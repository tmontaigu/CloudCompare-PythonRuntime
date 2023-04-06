# Depending of if pycc is being built as a stand-alone python module or a
# CloudCompare's plugin, the path to the C++ plugins is not the same since in
# one case CloudCompare's sources are somewere in the build dir (cmake handles
# that) or pycc's sources are within CloudCompare's sources.
if(PYCC_IS_MASTER_PROJECT)
  get_filename_component(
    STANDARD_PLUGINS_ROOT_DIR
    "${CLOUDCOMPARE_SOURCES_ROOT}/plugins/core/Standard"
    ABSOLUTE
    BASE_DIR
    ${CMAKE_CURRENT_LIST_DIR}
  )
else()
  get_filename_component(
    STANDARD_PLUGINS_ROOT_DIR
    "../../../../../core/Standard"
    ABSOLUTE
    BASE_DIR
    ${CMAKE_CURRENT_LIST_DIR}
  )
endif()

# The goal of the following if chain is to set 2 variables -
# INSTALL_PLUGINS_PATH: Where we are going to install the original C++ plugin
# (Should be the same as CC when compiling in plugin mode) -
# INSTALL_PLUGINS_WRAPPERS_PATH: Where we are going to install the python
# wrapper
if(APPLE)
  if(PYCC_IS_MASTER_PROJECT)
    include(GNUInstallDirs)
    set(INSTALL_PLUGINS_PATH
        "${CMAKE_INSTALL_FULL_LIBDIR}/cloudcompare/plugins"
    )
    set(INSTALL_PLUGINS_WRAPPERS_PATH
        "${CMAKE_INSTALL_FULL_LIBDIR}/cloudcompare/plugins-python"
    )
  else()
    set(CLOUDCOMPARE_MAC_BASE_DIR
        ${CMAKE_INSTALL_PREFIX}/${CLOUDCOMPARE_DEST_FOLDER}/CloudCompare.app
    )
    set(INSTALL_PLUGINS_PATH
        "${CLOUDCOMPARE_MAC_BASE_DIR}/Contents/PlugIns/ccPlugins"
    )
    set(INSTALL_PLUGINS_WRAPPERS_PATH
        "${CLOUDCOMPARE_MAC_BASE_DIR}/Contents/PlugIns/ccPythonPlugins"
    )
  endif()
elseif(UNIX)
  if(PYCC_IS_MASTER_PROJECT)
    include(GNUInstallDirs)
    # This is normally defined by CloudCompare's CMakeLists
    set(LINUX_INSTALL_SHARED_DESTINATION
        "${CMAKE_INSTALL_FULL_LIBDIR}/cloudcompare"
    )
  endif()

  set(INSTALL_PLUGINS_PATH "${LINUX_INSTALL_SHARED_DESTINATION}/plugins")
  set(INSTALL_PLUGINS_WRAPPERS_PATH
      "${LINUX_INSTALL_SHARED_DESTINATION}/plugins-python"
  )
elseif(WIN32)
  if(PYCC_IS_MASTER_PROJECT)
    set(INSTALL_PLUGINS_PATH "pycc/plugins-python")
    set(INSTALL_PLUGINS_WRAPPERS_PATH "pycc/plugins-python")
  else()
    set(INSTALL_PLUGINS_PATH "${CLOUDCOMPARE_DEST_FOLDER}/plugins")
    set(INSTALL_PLUGINS_WRAPPERS_PATH
        "${CLOUDCOMPARE_DEST_FOLDER}/plugins-python"
    )
  endif()
endif()

message(DEBUG "INSTALL_PLUGINS_PATH: ${INSTALL_PLUGINS_PATH}")
message(DEBUG "INSTALL_PLUGINS_WRAPPERS_PATH: ${INSTALL_PLUGINS_WRAPPERS_PATH}")

set(PYCC_PLUGIN_WRAPPER_LIST
    ""
    CACHE INTERNAL "Internal plugin wrapper list"
)

# Function to add a Python plugin that wraps a CloudComapare's C++ plugin.
#
# It handles where to install the plugin for supported platforms as well
# properly setting the RPATH on platforms that needs it.
#
# It needs two named argument: - NAME: name of the cmake target that is the
# Python wrapper - WRAPPED: name of the cmake target of the C++ plugin being
# wrapped
#
# Example: `AddPluginWrapper(NAME pym3c2 WRAPPED QM3C2_PLUGIN)`
function(AddPluginWrapper)
  # https://cmake.org/cmake/help/latest/command/cmake_parse_arguments.html
  set(options "")
  set(oneValueArgs "NAME;WRAPPED")
  set(multiValueArgs "")
  cmake_parse_arguments(
    PLUGIN_WRAPPER
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
  )

  if(NOT PLUGIN_WRAPPER_NAME)
    message(FATAL_ERROR "AddPluginWrapper missing 'NAME' parameter")
  endif()

  if(NOT PLUGIN_WRAPPER_WRAPPED)
    message(
      FATAL_ERROR
        "Plugin Wrapper named ${PLUGIN_WRAPPER_NAME} does not wrap any plugin"
    )
  endif()

  set(PYCC_PLUGIN_WRAPPER_LIST
      "${PYCC_PLUGIN_WRAPPER_LIST};${PLUGIN_WRAPPER_NAME}"
      CACHE INTERNAL "Internal plugin wrapper list"
  )

  # Install the wrapper
  install(TARGETS ${PLUGIN_WRAPPER_NAME}
          LIBRARY DESTINATION "${INSTALL_PLUGINS_WRAPPERS_PATH}"
  )

  # Install the plugin itself if stand alone package and take care of RPATH
  if(PYCC_IS_MASTER_PROJECT)
    install(
      TARGETS ${PLUGIN_WRAPPER_WRAPPED}
      LIBRARY DESTINATION ${INSTALL_PLUGINS_PATH}
      RUNTIME DESTINATION ${INSTALL_PLUGINS_PATH}
    )
    if(APPLE)
      # Change the RPATH The C++ plugin is in
      # $envPrefix/libdir/cloudcompare/plugins and is linked to libs in -
      # $envPrefix/libdir (where Qt / other CloudCompare stuff is)
      set_property(
        TARGET ${PLUGIN_WRAPPER_WRAPPED} PROPERTY INSTALL_RPATH
                                                  "@loader_path/../.."
      )

      # Change the RPATH The Python plugin wrapper is in
      # $envPrefix/libdir/cloudcompare/plugins-python and is linked to libs in -
      # $envPrefix/libdir/cloudcompare/plugins (where the plugin it wraps is) -
      # $envPrefix/libdir (where Qt / other CloudCompare stuff is)
      set_property(
        TARGET ${PLUGIN_WRAPPER_NAME}
        PROPERTY INSTALL_RPATH "@loader_path/../.." "@loader_path/../plugins"
      )
    elseif(UNIX)
      # Change the RPATH The C++ plugin is in
      # $envPrefix/libdir/cloudcompare/plugins and is linked to libs in -
      # $envPrefix/libdir (where Qt / other CloudCompare stuff is)
      set_property(
        TARGET ${PLUGIN_WRAPPER_WRAPPED} PROPERTY INSTALL_RPATH "$ORIGIN/../.."
      )

      # Change the RPATH The Python plugin wrapper is in
      # $envPrefix/libdir/cloudcompare/plugins-python and is linked to libs in -
      # $envPrefix/libdir/cloudcompare/plugins (where the plugin it wraps is) -
      # $envPrefix/libdir (where Qt / other CloudCompare stuff is)
      set_property(
        TARGET ${PLUGIN_WRAPPER_NAME} PROPERTY INSTALL_RPATH "$ORIGIN/../.."
                                               "$ORIGIN/../plugins"
      )
    endif()
  endif()
endfunction()

if(PLUGIN_STANDARD_QM3C2)
  add_subdirectory(m3c2)
endif()

# In standalone mode, some libs have to be installed by us.
if(PYCC_IS_MASTER_PROJECT AND PYCC_PLUGIN_WRAPPER_LIST)
  if(UNIX AND NOT APPLE)
    install(TARGETS CCPluginAPI
            LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
    )
  else()
    install(TARGETS CCPluginAPI RUNTIME DESTINATION ${INSTALL_PLUGINS_PATH})
  endif()
endif()
