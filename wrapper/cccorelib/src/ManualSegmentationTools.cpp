// ##########################################################################
// #                                                                        #
// #                CLOUDCOMPARE PLUGIN: PythonRuntime                       #
// #                                                                        #
// #  This program is free software; you can redistribute it and/or modify  #
// #  it under the terms of the GNU General Public License as published by  #
// #  the Free Software Foundation; version 2 of the License.               #
// #                                                                        #
// #  This program is distributed in the hope that it will be useful,       #
// #  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
// #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
// #  GNU General Public License for more details.                          #
// #                                                                        #
// #                   COPYRIGHT: Thomas Montaigu                           #
// #                                                                        #
// ##########################################################################

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
    using CCCoreLib::ManualSegmentationTools;

    py::class_<ManualSegmentationTools> PyManualSegmentationTools(cccorelib, "ManualSegmentationTools");
    py::class_<ManualSegmentationTools::MeshCutterParams> PyMeshCutterParams(PyManualSegmentationTools,
                                                                             "MeshCutterParams");
    PyManualSegmentationTools
        .def_static(
            "segment",
            (CCCoreLib::ReferenceCloud *
             (*)(CCCoreLib::GenericIndexedCloudPersist *, const CCCoreLib::Polyline *, bool, const float *))(
                &ManualSegmentationTools::segment),
            "aCloud"_a,
            "poly"_a,
            "keepInside"_a,
            "viewMat"_a = nullptr)

        .def_static("segment",
                    (CCCoreLib::ReferenceCloud *
                     (*)(CCCoreLib::GenericIndexedCloudPersist *, ScalarType, ScalarType, bool))(
                        &ManualSegmentationTools::segment),
                    "cloud"_a,
                    "minDist"_a,
                    "maxDist"_a,
                    "outside"_a = false)
        .def_static("isPointInsidePoly",
                    (bool (*)(const CCVector2 &, const CCCoreLib::GenericIndexedCloud *))(
                        &ManualSegmentationTools::isPointInsidePoly),
                    "P"_a,
                    "polyVertices"_a)
        .def_static("isPointInsidePoly",
                    (bool (*)(const CCVector2 &, const std::vector<CCVector2> &))(
                        &ManualSegmentationTools::isPointInsidePoly),
                    "P"_a,
                    "polyVertices"_a)

        .def_static("segmentMesh",
                    &ManualSegmentationTools::segmentMesh,
                    "theMesh"_a,
                    "pointsIndexes"_a,
                    "pointsWillBeInside"_a,
                    "progressCb"_a = nullptr,
                    "destCloud"_a = nullptr,
                    "indexShift"_a = 0,
                    "triangleIndexMap"_a = nullptr)
        .def_static("segmentMeshWithAAPlane",
                    &ManualSegmentationTools::segmentMeshWithAAPlane,
                    "mesh"_a,
                    "vertices"_a,
                    "ioParams"_a,
                    "progressCb"_a = nullptr)
        .def_static("segmentMeshWithAABox",
                    &ManualSegmentationTools::segmentMeshWithAABox,
                    "mesh"_a,
                    "vertices"_a,
                    "ioParams"_a,
                    "progressCb"_a = nullptr);

    PyMeshCutterParams
        .def_readwrite("insideMesh",
                       &ManualSegmentationTools::MeshCutterParams::insideMesh,
                       py::return_value_policy::reference)
        .def_readwrite("outsideMesh",
                       &ManualSegmentationTools::MeshCutterParams::outsideMesh,
                       py::return_value_policy::reference)
        .def_readwrite("generateOutsideMesh", &ManualSegmentationTools::MeshCutterParams::generateOutsideMesh)
        .def_readwrite("epsilon", &ManualSegmentationTools::MeshCutterParams::epsilon)
        .def_readwrite("planeOrthoDim", &ManualSegmentationTools::MeshCutterParams::planeOrthoDim)
        .def_readwrite("planeCoord", &ManualSegmentationTools::MeshCutterParams::planeCoord)
        .def_readwrite("bbMin", &ManualSegmentationTools::MeshCutterParams::bbMin)
        .def_readwrite("bbMax", &ManualSegmentationTools::MeshCutterParams::bbMax)
        .def_readwrite("trackOrigIndexes", &ManualSegmentationTools::MeshCutterParams::trackOrigIndexes)
        .def_readwrite("origTriIndexesMapInside",
                       &ManualSegmentationTools::MeshCutterParams::origTriIndexesMapInside)
        .def_readwrite("origTriIndexesMapOutside",
                       &ManualSegmentationTools::MeshCutterParams::origTriIndexesMapOutside)
        .def(py::init<>());
}
