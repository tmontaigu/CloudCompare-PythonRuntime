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

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>

#include <GenericCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_GenericCloud(py::module &cccorelib)
{
    py::class_<CCCoreLib::GenericCloud>(cccorelib, "GenericCloud")
        .def("size", &CCCoreLib::GenericCloud::size)
        .def("forEach", &CCCoreLib::GenericCloud::forEach, "action"_a)
        .def("getBoundingBox", &CCCoreLib::GenericCloud::getBoundingBox, "bbMin"_a, "bbMax"_a)
        .def("testVisibility", &CCCoreLib::GenericCloud::testVisibility, "P"_a)
        .def("placeIteratorAtBeginning", &CCCoreLib::GenericCloud::placeIteratorAtBeginning)
        .def("getNextPoint", &CCCoreLib::GenericCloud::getNextPoint, py::return_value_policy::reference)
        .def("enableScalarField", &CCCoreLib::GenericCloud::enableScalarField)
        .def("isScalarFieldEnabled", &CCCoreLib::GenericCloud::enableScalarField)
        .def("setPointScalarValue", &CCCoreLib::GenericCloud::setPointScalarValue, "pointIndex"_a, "value"_a)
        .def("getPointScalarValue", &CCCoreLib::GenericCloud::getPointScalarValue, "pointIndex"_a);
}
