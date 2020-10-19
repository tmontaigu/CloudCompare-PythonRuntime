#include <pybind11/pybind11.h>

#include <BoundingBox.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_BoundingBox(py::module &cccorelib)
{
    py::class_<CCCoreLib::BoundingBox>(cccorelib, "BoundingBox")
        .def(py::init<>())
        .def(py::init<CCVector3, CCVector3>())
        .def("clear", &CCCoreLib::BoundingBox::clear)
        .def("add", &CCCoreLib::BoundingBox::add)
        .def("minCorner", [](const CCCoreLib::BoundingBox &self) { return self.minCorner(); })
        .def("maxCorner", [](const CCCoreLib::BoundingBox &self) { return self.maxCorner(); })
        .def("getCenter", &CCCoreLib::BoundingBox::getCenter)
        .def("getDiagVec", &CCCoreLib::BoundingBox::getDiagVec)
        .def("getDiagNorm", &CCCoreLib::BoundingBox::getDiagNorm)
        .def("getDiagNormd", &CCCoreLib::BoundingBox::getDiagNormd)
        .def("getMinBoxDim", &CCCoreLib::BoundingBox::getMinBoxDim)
        .def("getMaxBoxDim", &CCCoreLib::BoundingBox::getMaxBoxDim)
        .def("computeVolume", &CCCoreLib::BoundingBox::computeVolume)
        .def("setValidity", &CCCoreLib::BoundingBox::setValidity)
        .def("isValid", &CCCoreLib::BoundingBox::isValid)
        .def("minDistTo", &CCCoreLib::BoundingBox::minDistTo)
        .def("contains", &CCCoreLib::BoundingBox::contains);
}