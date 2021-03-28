set(cccorelib_sources
    ${CMAKE_CURRENT_LIST_DIR}/src/cccorelib.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/cccorelib.h
    ${CMAKE_CURRENT_LIST_DIR}/src/AutoSegmentationTools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/BoundingBox.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/CCGeom.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/CCMath.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/CCConst.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/CCMiscTools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/CCShareable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/CloudSamplingTools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GenericIndexedCloudPersist.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PointCloud.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GenericOctree.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ChamferDistanceTransform.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/DgmOctree.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/DgmOctreeReferenceCloud.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GenericCloud.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GenericIndexedCloud.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GenericProgressCallback.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GeometricalAnalysisTools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TrueKdTree.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/StatisticalTestingTools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GenericDistribution.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GenericTriangle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SimpleTriangle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GenericMesh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GenericIndexedMesh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SimpleMesh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/KdTree.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/LocalModel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/WeibullDistribution.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ScalarFieldTools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ScalarField.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/RegistrationTools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ReferenceCloud.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Polyline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PointProjectionTools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ManualSegmentationTools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/NormalDistribution.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Delaynay2dMesh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ErrorFunction.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/FastMarching.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/DistanceComputationTools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/wrappers.h
    ${CMAKE_CURRENT_LIST_DIR}/src/casters.h
)

if (NOT USE_EMBEDDED_MODULES)
    cmake_minimum_required(VERSION 3.12)
    project(pycccorelib)

    set(CMAKE_CXX_STANDARD 14)

    find_package(pybind11 CONFIG REQUIRED)

    pybind11_add_module(cccorelib ${cccorelib_sources})
    target_link_libraries(cccorelib PRIVATE CCCoreLib)
else ()
    function(embed_cccorelib_in target)
        target_sources(${target} PRIVATE ${cccorelib_sources})
        target_link_libraries(${target} CCCoreLib)
    endfunction(embed_cccorelib_in)
endif ()
