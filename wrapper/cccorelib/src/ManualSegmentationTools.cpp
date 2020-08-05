#include <pybind11/pybind11.h>

#include <GenericIndexedCloudPersist.h>
#include <GenericIndexedMesh.h>
#include <GenericProgressCallback.h>
#include <ManualSegmentationTools.h>
#include <Polyline.h>
#include <SimpleMesh.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ManualSegmentationTools(py::module &cccorelib)
{
    py::class_<CCCoreLib::ManualSegmentationTools> ManualSegmentationTools(cccorelib,
                                                                           "ManualSegmentationTools");
    ManualSegmentationTools
        .def_static(
            "segment",
            (CCCoreLib::ReferenceCloud *
             (*)(CCCoreLib::GenericIndexedCloudPersist *, const CCCoreLib::Polyline *, bool, const float *))(
                &CCCoreLib::ManualSegmentationTools::segment),
            "aCloud"_a,
            "poly"_a,
            "keppInside"_a,
            "viewMat"_a = nullptr)
        .def_static("segmentReferenceCloud",
                    &CCCoreLib::ManualSegmentationTools::segmentReferenceCloud,
                    "cloud"_a,
                    "minDist"_a,
                    "maxDist"_a,
                    "outside"_a)
        .def_static("segment",
                    (CCCoreLib::ReferenceCloud *
                     (*)(CCCoreLib::GenericIndexedCloudPersist *, ScalarType, ScalarType, bool))(
                        &CCCoreLib::ManualSegmentationTools::segment),
                    "cloud"_a,
                    "minDist"_a,
                    "maxDist"_a,
                    "outside"_a = false)
        .def_static("isPointInsidePoly",
                    (bool (*)(const CCVector2 &, const CCCoreLib::GenericIndexedCloud *))(
                        &CCCoreLib::ManualSegmentationTools::isPointInsidePoly),
                    "P"_a,
                    "polyVertices"_a)
        .def_static("isPointInsidePoly",
                    (bool (*)(const CCVector2 &, const std::vector<CCVector2> &))(
                        &CCCoreLib::ManualSegmentationTools::isPointInsidePoly),
                    "P"_a,
                    "polyVertices"_a)

        .def_static("segmentMesh",
                    &CCCoreLib::ManualSegmentationTools::segmentMesh,
                    "theMesh"_a,
                    "pointsIndexes"_a,
                    "pointsWillBeInside"_a,
                    "progressCb"_a = nullptr,
                    "destCloud"_a = nullptr,
                    "indexShift"_a = 0);

    py::class_<CCCoreLib::ManualSegmentationTools::MeshCutterParams>(ManualSegmentationTools,
                                                                     "MeshCutterParams")
        .def_readwrite("insideMesh",
                       &CCCoreLib::ManualSegmentationTools::MeshCutterParams::insideMesh,
                       py::return_value_policy::reference)
        .def_readwrite("outsideMesh",
                       &CCCoreLib::ManualSegmentationTools::MeshCutterParams::outsideMesh,
                       py::return_value_policy::reference)
        .def_readwrite("generateOutsideMesh",
                       &CCCoreLib::ManualSegmentationTools::MeshCutterParams::generateOutsideMesh)
        .def_readwrite("epsilon", &CCCoreLib::ManualSegmentationTools::MeshCutterParams::epsilon)
        .def_readwrite("planeOrthoDim", &CCCoreLib::ManualSegmentationTools::MeshCutterParams::planeOrthoDim)
        .def_readwrite("planeCoord", &CCCoreLib::ManualSegmentationTools::MeshCutterParams::planeCoord)
        .def_readwrite("bbMin", &CCCoreLib::ManualSegmentationTools::MeshCutterParams::bbMin)
        .def_readwrite("bbMax", &CCCoreLib::ManualSegmentationTools::MeshCutterParams::bbMax)
        .def_readwrite("trackOrigIndexes",
                       &CCCoreLib::ManualSegmentationTools::MeshCutterParams::trackOrigIndexes)
        .def_readwrite("origTriIndexesMapInside",
                       &CCCoreLib::ManualSegmentationTools::MeshCutterParams::origTriIndexesMapInside)
        .def_readwrite("origTriIndexesMapOutside",
                       &CCCoreLib::ManualSegmentationTools::MeshCutterParams::origTriIndexesMapOutside)
        .def(py::init<>());

    ManualSegmentationTools
        .def_static("segmentMeshWithAAPlane",
                    &CCCoreLib::ManualSegmentationTools::segmentMeshWithAAPlane,
                    "mesh"_a,
                    "vertices"_a,
                    "ioParams"_a,
                    "progressCb"_a = nullptr)
        .def_static("segmentMeshWithAABox",
                    &CCCoreLib::ManualSegmentationTools::segmentMeshWithAABox,
                    "mesh"_a,
                    "vertices"_a,
                    "ioParams"_a,
                    "progressCb"_a = nullptr);
}