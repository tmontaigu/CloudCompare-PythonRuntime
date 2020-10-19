#include <pybind11/pybind11.h>

#include <CCGeom.h>
#include <GenericCloud.h>
#include <GenericIndexedMesh.h>
#include <GenericProgressCallback.h>
#include <PointCloud.h>
#include <PointProjectionTools.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_PointProjectionTools(py::module &cccorelib)
{
    py::enum_<CCCoreLib::TRIANGULATION_TYPES>(cccorelib, "TRIANGULATION_TYPES")
        .value("DELAUNAY_2D_AXIS_ALIGNED", CCCoreLib::TRIANGULATION_TYPES::DELAUNAY_2D_AXIS_ALIGNED)
        .value("DELAUNAY_2D_BEST_LS_PLANE", CCCoreLib::TRIANGULATION_TYPES::DELAUNAY_2D_BEST_LS_PLANE);

    py::class_<CCCoreLib::PointProjectionTools> PointProjectionTools(cccorelib, "PointProjectionTools");

    PointProjectionTools
        .def_static("developCloudOnCylinder",
                    &CCCoreLib::PointProjectionTools::developCloudOnCylinder,
                    "cloud"_a,
                    "radius"_a,
                    "dim"_a = 2,
                    "center"_a = nullptr,
                    "progressCb"_a = nullptr)
        .def_static("developCloudOnCone",
                    &CCCoreLib::PointProjectionTools::developCloudOnCone,
                    "cloud"_a,
                    "dim"_a,
                    "baseRadius"_a,
                    "alpha"_a,
                    "center"_a,
                    "progressCb"_a = nullptr)
        .def_static("applyTransformation",
                    &CCCoreLib::PointProjectionTools::applyTransformation,
                    "cloud"_a,
                    "trans"_a,
                    "progressCb"_a = nullptr)
        .def_static("computeTriangulation",
                    &CCCoreLib::PointProjectionTools::computeTriangulation,
                    "cloud"_a,
                    "type"_a,
                    "maxEdgeLength"_a,
                    "dim"_a,
                    "outputErrorStr"_a);

    py::class_<CCCoreLib::PointProjectionTools::Transformation>(PointProjectionTools, "Transformation")
        .def_readwrite("R", &CCCoreLib::PointProjectionTools::Transformation::R)
        .def_readwrite("T", &CCCoreLib::PointProjectionTools::Transformation::T)
        .def_readwrite("s", &CCCoreLib::PointProjectionTools::Transformation::s)
        .def(py::init<>())
        // TODO
        //			.def("apply", &CCCoreLib::PointProjectionTools::Transformation::apply, "P"_a)
        //			.def("")
        ;

    // TODO convexhull static methods

    py::class_<CCCoreLib::PointProjectionTools::IndexedCCVector2>(PointProjectionTools, "IndexedCCVector2")
        .def(py::init<>())
        .def(py::init<PointCoordinateType, PointCoordinateType>(), "x"_a, "y"_a)
        .def(py::init<PointCoordinateType, PointCoordinateType, unsigned>(), "x"_a, "y"_a, "i"_a)
        .def(py::init<const CCVector2 &>(), "v"_a)
        .def_readwrite("index", &CCCoreLib::PointProjectionTools::IndexedCCVector2::index);
}