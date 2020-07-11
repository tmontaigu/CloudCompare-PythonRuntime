cmake_minimum_required(VERSION 3.12)
project(pycc)

set(CMAKE_CXX_STANDARD 11)

find_package(pybind11 CONFIG REQUIRED)


pybind11_add_module(cccorelib
        cccorelib/src/cccorelib.cpp
        cccorelib/src/AutoSegmentationTools.cpp
        cccorelib/src/BoundingBox.cpp
        cccorelib/src/CCGeom.cpp
        cccorelib/src/CCMath.cpp
        cccorelib/src/CCMiscTools.cpp
        cccorelib/src/CloudSamplingTools.cpp
        cccorelib/src/GenericIndexedCloudPersist.cpp
        cccorelib/src/PointCloud.cpp
        cccorelib/src/GenericOctree.cpp
        cccorelib/src/ChamferDistanceTransform.cpp
        cccorelib/src/DgmOctree.cpp
        cccorelib/src/DgmOctreeReferenceCloud.cpp
        cccorelib/src/GenericCloud.cpp
        cccorelib/src/GenericIndexedCloud.cpp
        cccorelib/src/GenericProgressCallback.cpp
        cccorelib/src/GeometricalAnalysisTools.cpp
        cccorelib/src/TrueKdTree.cpp
        cccorelib/src/StatisticalTestingTools.cpp
        cccorelib/src/GenericDistribution.cpp
        cccorelib/src/GenericTriangle.cpp
        cccorelib/src/SimpleTriangle.cpp
        cccorelib/src/GenericMesh.cpp
        cccorelib/src/GenericIndexedMesh.cpp
        cccorelib/src/SimpleMesh.cpp
        cccorelib/src/KdTree.cpp
        cccorelib/src/LocalModel.cpp
        cccorelib/src/WeibullDistribution.cpp
        cccorelib/src/ScalarFieldTools.cpp
        cccorelib/src/ScalarField.cpp
        cccorelib/src/RegistrationTools.cpp
        cccorelib/src/ReferenceCloud.cpp
        cccorelib/src/Polyline.cpp
        cccorelib/src/PointProjectionTools.cpp
        src/casters.h
        src/wrappers.h
)
target_link_libraries(cccorelib PRIVATE CCCoreLib)

pybind11_add_module(pycc src/pycc.cpp src/wrappers.h src/casters.h)
target_link_libraries(pycc PRIVATE QCC_DB_LIB PythonPlugin)
target_include_directories(pycc PRIVATE ../include)