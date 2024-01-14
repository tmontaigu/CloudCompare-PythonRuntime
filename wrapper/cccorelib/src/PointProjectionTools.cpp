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

#include <CCGeom.h>
#include <GenericCloud.h>
#include <GenericIndexedMesh.h>
#include <GenericProgressCallback.h>
#include <PointCloud.h>
#include <PointProjectionTools.h>
#include <SquareMatrix.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_PointProjectionTools(py::module &cccorelib)
{
    using CCCoreLib::PointProjectionTools;

    py::enum_<CCCoreLib::TRIANGULATION_TYPES>(cccorelib, "TRIANGULATION_TYPES")
        .value("DELAUNAY_2D_AXIS_ALIGNED", CCCoreLib::TRIANGULATION_TYPES::DELAUNAY_2D_AXIS_ALIGNED)
        .value("DELAUNAY_2D_BEST_LS_PLANE", CCCoreLib::TRIANGULATION_TYPES::DELAUNAY_2D_BEST_LS_PLANE);

    py::class_<CCCoreLib::SquareMatrixTpl<double>> PySquareMatrixd(cccorelib, "SquareMatrixd");

    py::class_<PointProjectionTools> PyPointProjectionTools(cccorelib, "PointProjectionTools");
    py::class_<PointProjectionTools::Transformation> PyTransformation(PyPointProjectionTools,
                                                                      "Transformation");
    py::class_<PointProjectionTools::IndexedCCVector2> PyIndexedCCVector2(PyPointProjectionTools,
                                                                          "IndexedCCVector2");

    PyPointProjectionTools
        .def_static("developCloudOnCylinder",
                    &PointProjectionTools::developCloudOnCylinder,
                    "cloud"_a,
                    "radius"_a,
                    "dim"_a = 2,
                    "center"_a = nullptr,
                    "progressCb"_a = nullptr)
        .def_static("developCloudOnCone",
                    &PointProjectionTools::developCloudOnCone,
                    "cloud"_a,
                    "dim"_a,
                    "baseRadius"_a,
                    "alpha"_a,
                    "center"_a,
                    "progressCb"_a = nullptr)
        .def_static("applyTransformation",
                    static_cast<CCCoreLib::PointCloud *(*)(CCCoreLib::GenericCloud *,
                                                           PointProjectionTools::Transformation &,
                                                           CCCoreLib::GenericProgressCallback *)>(
                        &PointProjectionTools::applyTransformation),
                    "cloud"_a,
                    "trans"_a,
                    "progressCb"_a = nullptr)
        .def_static("applyTransformation",
                    static_cast<CCCoreLib::PointCloud *(*)(CCCoreLib::GenericIndexedCloud *,
                                                           PointProjectionTools::Transformation &,
                                                           CCCoreLib::GenericProgressCallback *)>(
                        &PointProjectionTools::applyTransformation),
                    "cloud"_a,
                    "trans"_a,
                    "progressCb"_a = nullptr)
        .def_static("computeTriangulation",
                    &PointProjectionTools::computeTriangulation,
                    "cloud"_a,
                    "type"_a,
                    "maxEdgeLength"_a,
                    "dim"_a,
                    "outputErrorStr"_a);

    PyTransformation.def_readwrite("R", &PointProjectionTools::Transformation::R)
        .def_readwrite("T", &PointProjectionTools::Transformation::T)
        .def_readwrite("s", &PointProjectionTools::Transformation::s)
        .def(py::init<>())
        .def("apply",
             (CCVector3(PointProjectionTools::Transformation::*)(const CCVector3 &)
                  const)(&PointProjectionTools::Transformation::apply),
             "P"_a)
        .def("apply",
             (void(PointProjectionTools::Transformation::*)(CCCoreLib::GenericIndexedCloudPersist &)
                  const)(&PointProjectionTools::Transformation::apply),
             "cloud"_a);

    PyIndexedCCVector2.def(py::init<>())
        .def(py::init<PointCoordinateType, PointCoordinateType>(), "x"_a, "y"_a)
        .def(py::init<PointCoordinateType, PointCoordinateType, unsigned>(), "x"_a, "y"_a, "i"_a)
        .def(py::init<const CCVector2 &>(), "v"_a)
        .def_readwrite("index", &PointProjectionTools::IndexedCCVector2::index);

    // bind Vectors
    //    PointProjectionTools.def_static("extractConvexHull2D", &PointProjectionTools::extractConvexHull2D)
}
