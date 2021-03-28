set(pycc_sources
    ${CMAKE_CURRENT_LIST_DIR}/src/casters.h
    ${CMAKE_CURRENT_LIST_DIR}/src/pycc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccGUIPythonInstance.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccCommandLine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccGenericGLDisplay.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccGLWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccDrawableObject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccScalarField.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccObject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccGenericPointCloud.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccPointCloud.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccSphere.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccPlane.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccTorus.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccBox.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccDish.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccCone.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccCylinder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccGLMatrix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccGenericMesh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccGenericPrimitive.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccMesh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccProgressDialog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccLog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ccInteractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/cc2DLabel.cpp
)

if (NOT USE_EMBEDDED_MODULES)
    cmake_minimum_required(VERSION 3.12)
    project(pycc)

    set(CMAKE_CXX_STANDARD 14)

    find_package(pybind11 CONFIG REQUIRED)

    pybind11_add_module(pycc ${pycc_sources})
    target_link_libraries(pycc PRIVATE QCC_DB_LIB PythonPlugin)
    target_include_directories(pycc PRIVATE ../../include)
else()
    function(embed_pycc_in target)
        target_sources(${target} PRIVATE ${pycc_sources})
        target_link_libraries(${target} QCC_DB_LIB)
    endfunction(embed_pycc_in)
endif()


