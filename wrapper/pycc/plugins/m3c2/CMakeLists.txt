pybind11_add_module(pym3c2)
set_target_properties(pym3c2 PROPERTIES DEBUG_POSTFIX "")

if(WIN32 AND MSVC)
  # On Windows with MSVC, symbols in a shared lib are not exported by default,
  # to export them we usualy ned to mark them DLL_EXPORT (and DLL_IMPORT). To
  # avoid having to change the sources of plugin we change the default to export
  # all symbols.
  set_property(TARGET QM3C2_PLUGIN PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

target_link_libraries(pym3c2 PRIVATE QM3C2_PLUGIN)
target_include_directories(
  pym3c2 PRIVATE "${STANDARD_PLUGINS_ROOT_DIR}/qM3C2/include"
)

find_package(Qt5 COMPONENTS Widgets)

set_target_properties(
  pym3c2 PROPERTIES AUTOUIC ON AUTOUIC_SEARCH_PATHS
                               "${STANDARD_PLUGINS_ROOT_DIR}/qM3C2/ui"
)

target_sources(
  pym3c2
  PRIVATE m3c2.cpp "${STANDARD_PLUGINS_ROOT_DIR}/qM3C2/ui/qM3C2Dialog.ui"
          "${STANDARD_PLUGINS_ROOT_DIR}/qM3C2/include/qM3C2Dialog.h"
)

addpluginwrapper(NAME pym3c2 WRAPPED QM3C2_PLUGIN)
