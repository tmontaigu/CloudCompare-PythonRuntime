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

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include <BoundingBox.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_BoundingBox(py::module &cccorelib)
{
    py::class_<CCCoreLib::BoundingBox>(cccorelib, "BoundingBox")
        .def(py::init<>())
        .def(py::init<CCVector3, CCVector3, bool>(), "minCorner"_a, "maxCorner"_a, "valid"_a)
        .def("clear", &CCCoreLib::BoundingBox::clear)
        .def("add", &CCCoreLib::BoundingBox::add, "aPoint"_a)
        .def("minCorner", [](const CCCoreLib::BoundingBox &self) { return self.minCorner(); })
        .def("maxCorner", [](const CCCoreLib::BoundingBox &self) { return self.maxCorner(); })
        .def("getCenter", &CCCoreLib::BoundingBox::getCenter)
        .def("getDiagVec", &CCCoreLib::BoundingBox::getDiagVec)
        .def("getDiagNorm", &CCCoreLib::BoundingBox::getDiagNorm)
        .def("getDiagNormd", &CCCoreLib::BoundingBox::getDiagNormd)
        .def("getMinBoxDim", &CCCoreLib::BoundingBox::getMinBoxDim)
        .def("getMaxBoxDim", &CCCoreLib::BoundingBox::getMaxBoxDim)
        .def("computeVolume", &CCCoreLib::BoundingBox::computeVolume)
        .def("setValidity", &CCCoreLib::BoundingBox::setValidity, "state"_a)
        .def("isValid", &CCCoreLib::BoundingBox::isValid)
        .def("minDistTo", &CCCoreLib::BoundingBox::minDistTo, "box"_a)
        .def("contains", &CCCoreLib::BoundingBox::contains, "P"_a)
        .def(py::self + py::self)
        .def(py::self += py::self)
        .def(py::self += CCVector3())
        .def(py::self -= CCVector3())
        .def(py::self *= double());
    // TODO operator *= square matrix
}
