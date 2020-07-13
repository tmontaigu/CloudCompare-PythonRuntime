cmake_minimum_required(VERSION 3.12)
project(cccorelib)

set(CMAKE_CXX_STANDARD 14)

find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(cccorelib
        src/cccorelib.cpp
        src/AutoSegmentationTools.cpp
        src/BoundingBox.cpp
        src/CCGeom.cpp
        src/CCMath.cpp
        src/CCMiscTools.cpp
        src/CloudSamplingTools.cpp
        src/GenericIndexedCloudPersist.cpp
        src/PointCloud.cpp
        src/GenericOctree.cpp
        src/ChamferDistanceTransform.cpp
        src/DgmOctree.cpp
        src/DgmOctreeReferenceCloud.cpp
        src/GenericCloud.cpp
        src/GenericIndexedCloud.cpp
        src/GenericProgressCallback.cpp
        src/GeometricalAnalysisTools.cpp
        src/TrueKdTree.cpp
        src/StatisticalTestingTools.cpp
        src/GenericDistribution.cpp
        src/GenericTriangle.cpp
        src/SimpleTriangle.cpp
        src/GenericMesh.cpp
        src/GenericIndexedMesh.cpp
        src/SimpleMesh.cpp
        src/KdTree.cpp
        src/LocalModel.cpp
        src/WeibullDistribution.cpp
        src/ScalarFieldTools.cpp
        src/ScalarField.cpp
        src/RegistrationTools.cpp
        src/ReferenceCloud.cpp
        src/Polyline.cpp
        src/PointProjectionTools.cpp
        src/ManualSegmentationTools.cpp
        src/NormalDistribution.cpp
        src/Delaynay2dMesh.cpp
        src/ErrorFunction.cpp
        src/FastMarching.cpp
        src/wrappers.h
)
target_link_libraries(cccorelib PRIVATE CCCoreLib)