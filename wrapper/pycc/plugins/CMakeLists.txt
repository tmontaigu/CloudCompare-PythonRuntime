
# Depending of if pycc is being built as a stand-alone python module
# or a CloudCompare's plugin, the path to the C++ plugins is not the same
# since in one case CloudCompare's sources are somewere in the build dir (cmake handles that)
# or pycc's sources are within CloudCompare's sources.
if (PYCC_IS_MASTER_PROJECT)
    get_filename_component(STANDARD_PLUGINS_ROOT_DIR "${CLOUDCOMPARE_SOURCES_ROOT}/plugins/core/Standard" ABSOLUTE BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
    include(GNUInstallDirs)
    set(LINUX_INSTALL_SHARED_DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/cloudcompare")
else()
    get_filename_component(STANDARD_PLUGINS_ROOT_DIR "../../../../../core/Standard" ABSOLUTE BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()

set(LINUX_INSTALL_PLUGINS "${LINUX_INSTALL_SHARED_DESTINATION}/plugins")
set(LINUX_INSTALL_PLUGIN_WRAPPERS "${LINUX_INSTALL_SHARED_DESTINATION}/plugins-python")

set(PYCC_PLUGIN_WRAPPER_LIST "" CACHE INTERNAL "Internal plugin wrapper list")


# Function to add a Python plugin that wraps a CloudComapare's C++ plugin.
#
# It handles where to install the plugin for supported platforms
# as well properly setting the RPATH on platforms that needs it.
#
# It needs two named argument:
#   - NAME: name of the cmake target that is the Python wrapper
#   - WRAPPED: name of the cmake target of the C++ plugin being wrapped
#
# Example: `AddPluginWrapper(NAME pym3c2 WRAPPED QM3C2_PLUGIN)`
function(AddPluginWrapper)
    # https://cmake.org/cmake/help/latest/command/cmake_parse_arguments.html
    set(options "")
    set(oneValueArgs "NAME;WRAPPED")
    set(multiValueArgs "")
    cmake_parse_arguments(PLUGIN_WRAPPER "${options}" "${oneValueArgs}"
            "${multiValueArgs}" ${ARGN})

    if (NOT PLUGIN_WRAPPER_NAME)
        message(FATAL_ERROR "AddPluginWrapper missing 'NAME' parameter")
    endif()

    if (NOT PLUGIN_WRAPPER_WRAPPED)
        message(FATAL_ERROR "Plugin Wrapper named ${PLUGIN_WRAPPER_NAME} does not wrap any plugin")
    endif()

    set(PYCC_PLUGIN_WRAPPER_LIST "${PYCC_PLUGIN_WRAPPER_LIST};${PLUGIN_WRAPPER_NAME}" CACHE INTERNAL "Internal plugin wrapper list")

    # Install the wrapper
    install(
            TARGETS ${PLUGIN_WRAPPER_NAME}
            LIBRARY DESTINATION "${LINUX_INSTALL_PLUGIN_WRAPPERS}"
    )

    # Install the plugin itself if stand alone package
    # and take care of RPATH
    if(PYCC_IS_MASTER_PROJECT)
        install(
                TARGETS ${PLUGIN_WRAPPER_WRAPPED}
                LIBRARY DESTINATION ${LINUX_INSTALL_PLUGINS}
        )

        # Change the RPATH
        # The C++ plugin is in $envPrefix/libdir/cloudcompare/plugins
        # and is linked to libs in 
        # - $envPrefix/libdir (where Qt / other CloudCompare stuff is)
        set_property(
            TARGET ${PLUGIN_WRAPPER_WRAPPED}
            PROPERTY INSTALL_RPATH
            "$ORIGIN/../.."
        )

        # Change the RPATH
        # The Python plugin wrapper is in $envPrefix/libdir/cloudcompare/plugins-python
        # and is linked to libs in 
        # - $envPrefix/libdir/cloudcompare/plugins (where the plugin it wraps is)
        # - $envPrefix/libdir (where Qt / other CloudCompare stuff is)
        set_property(
                TARGET ${PLUGIN_WRAPPER_NAME}
                PROPERTY INSTALL_RPATH
                "$ORIGIN/../.."
                "$ORIGIN/../plugins"
        )
    endif()
endfunction()


if (PLUGIN_STANDARD_QM3C2)
    add_subdirectory(m3c2)
endif()

# In standalone mode, some libs have to be installed
# by us.
if(PYCC_IS_MASTER_PROJECT AND PYCC_PLUGIN_WRAPPER_LIST)
    install(
            TARGETS CCPluginAPI
            LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
    )
endif()