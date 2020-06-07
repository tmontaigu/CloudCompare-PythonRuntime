option(PLUGIN_PYTHON "Install example plugin" OFF)

if (PLUGIN_PYTHON)
    project(PythonPlugin)

    AddPlugin(NAME ${PROJECT_NAME})

    add_subdirectory(wrapper)

    add_subdirectory(include)
    add_subdirectory(src)

    find_package(pybind11 CONFIG REQUIRED)
    target_link_libraries(PythonPlugin pybind11::embed)
endif ()
