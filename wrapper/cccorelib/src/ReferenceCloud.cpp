//##########################################################################
//#                                                                        #
//#                CLOUDCOMPARE PLUGIN: PythonPlugin                       #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                   COPYRIGHT: Thomas Montaigu                           #
//#                                                                        #
//##########################################################################

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

#include <GenericIndexedCloudPersist.h>
#include <ReferenceCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ReferenceCloud(py::module &cccorelib)
{
    py::class_<CCCoreLib::ReferenceCloud, CCCoreLib::GenericIndexedCloudPersist>(cccorelib, "ReferenceCloud")
        .def(py::init<CCCoreLib::GenericIndexedCloudPersist *>(), "associatedCloud"_a)
        .def("getPointGlobalIndex", &CCCoreLib::ReferenceCloud::getPointGlobalIndex)
        .def("getCurrentPointGlobalIndex", &CCCoreLib::ReferenceCloud::getCurrentPointGlobalIndex)
        .def("getCurrentPointScalarValue", &CCCoreLib::ReferenceCloud::getCurrentPointScalarValue)
        .def("setCurrentPointScalarValue", &CCCoreLib::ReferenceCloud::setCurrentPointScalarValue)
        .def("forwardIterator", &CCCoreLib::ReferenceCloud::forwardIterator)
        .def("clear", &CCCoreLib::ReferenceCloud::clear, "releaseMemory"_a = false)
        .def("addPointIndex",
             [](CCCoreLib::ReferenceCloud &self, unsigned firstIndex, unsigned lastIndex) {
                 self.addPointIndex(firstIndex, lastIndex);
             })
        .def("addPointIndex",
             [](CCCoreLib::ReferenceCloud &self, unsigned globalIndex) { self.addPointIndex(globalIndex); })
        .def("setPointIndex", &CCCoreLib::ReferenceCloud::setPointIndex)
        .def("reserve", &CCCoreLib::ReferenceCloud::reserve)
        .def("resize", &CCCoreLib::ReferenceCloud::resize)
        .def("capacity", &CCCoreLib::ReferenceCloud::capacity)
        .def("swap", &CCCoreLib::ReferenceCloud::swap)
        .def("removeCurrentPointGlobalIndex", &CCCoreLib::ReferenceCloud::removeCurrentPointGlobalIndex)
        .def("removePointGlobalIndex", &CCCoreLib::ReferenceCloud::removePointGlobalIndex)
        .def(
            "getAssociatedCloud",
            [](CCCoreLib::ReferenceCloud &self) { return self.getAssociatedCloud(); },
            py::return_value_policy::reference)
        .def("setAssociatedCloud", &CCCoreLib::ReferenceCloud::setAssociatedCloud)
        .def("add", &CCCoreLib::ReferenceCloud::add)
        .def("invalidateBoundingBox", &CCCoreLib::ReferenceCloud::invalidateBoundingBox);
}