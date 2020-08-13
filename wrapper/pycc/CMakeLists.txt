cmake_minimum_required(VERSION 3.12)
project(pycc)

set(CMAKE_CXX_STANDARD 11)

find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(
        pycc
        src/pycc.cpp
        src/ccGUIPythonInstance.cpp
        src/ccCommandLine.cpp
        src/casters.h
        src/ccDrawableObject.cpp
        src/ccScalarField.cpp
        src/ccObject.cpp
        src/ccGenericPointCloud.cpp
        src/ccPointCloud.cpp
)
target_link_libraries(pycc PRIVATE QCC_DB_LIB PythonPlugin)
target_include_directories(pycc PRIVATE ../../include)