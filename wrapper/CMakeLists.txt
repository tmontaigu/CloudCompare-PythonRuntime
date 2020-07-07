cmake_minimum_required(VERSION 3.16)
project(pycc)

set(CMAKE_CXX_STANDARD 11)

find_package(pybind11 CONFIG REQUIRED)


pybind11_add_module(cccorelib src/cccorelib.cpp src/casters.h src/wrappers.h)
target_link_libraries(cccorelib PRIVATE CCCoreLib)

pybind11_add_module(pycc src/pycc.cpp src/wrappers.h src/casters.h)
target_link_libraries(pycc PRIVATE QCC_DB_LIB PythonPlugin)
target_include_directories(pycc PRIVATE ../include)