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
#include <pybind11/stl_bind.h>

#include <GenericIndexedCloudPersist.h>
#include <ReferenceCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;

using CCCoreLib::ReferenceCloud;

void define_ReferenceCloud(py::module &cccorelib)
{
    py::class_<ReferenceCloud, CCCoreLib::GenericIndexedCloudPersist>(cccorelib, "ReferenceCloud")
        .def(py::init<CCCoreLib::GenericIndexedCloudPersist *>(), "associatedCloud"_a, py::keep_alive<1, 2>())
        .def("getPointGlobalIndex", &ReferenceCloud::getPointGlobalIndex)
        .def("getCurrentPointCoordinates", &ReferenceCloud::getCurrentPointCoordinates)
        .def("getCurrentPointGlobalIndex", &ReferenceCloud::getCurrentPointGlobalIndex)
        .def("getCurrentPointScalarValue", &ReferenceCloud::getCurrentPointScalarValue)
        .def("setCurrentPointScalarValue", &ReferenceCloud::setCurrentPointScalarValue)
        .def("forwardIterator", &ReferenceCloud::forwardIterator)
        .def("clear", &ReferenceCloud::clear, "releaseMemory"_a = false)
        .def("addPointIndex",
             static_cast<bool (ReferenceCloud::*)(unsigned)>(&ReferenceCloud::addPointIndex),
             "unsigned"_a)
        .def("addPointIndex",
             static_cast<bool (ReferenceCloud::*)(unsigned, unsigned)>(&ReferenceCloud::addPointIndex),
             "firstIndex"_a,
             "lastIndex"_a)
        .def("setPointIndex", &ReferenceCloud::setPointIndex, "firstIndex"_a, "lastIndex"_a)
        .def("reserve", &ReferenceCloud::reserve, "n"_a)
        .def("resize", &ReferenceCloud::resize, "_n")
        .def("capacity", &ReferenceCloud::capacity)
        .def("swap", &ReferenceCloud::swap, "i"_a, "j"_a)
        .def("removeCurrentPointGlobalIndex", &ReferenceCloud::removeCurrentPointGlobalIndex)
        .def("removePointGlobalIndex", &ReferenceCloud::removePointGlobalIndex, "localIndex"_a)
        .def("getAssociatedCloud",
             static_cast<CCCoreLib::GenericIndexedCloudPersist *(ReferenceCloud::*)()>(
                 &ReferenceCloud::getAssociatedCloud),
             py::return_value_policy::reference)
        .def("setAssociatedCloud", &ReferenceCloud::setAssociatedCloud, "cloud"_a)
        .def("add", &ReferenceCloud::add, "cloud"_a)
        .def("invalidateBoundingBox", &ReferenceCloud::invalidateBoundingBox);
}
