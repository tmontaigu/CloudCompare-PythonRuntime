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

#include <GenericIndexedMesh.h>
#include <GenericMesh.h>
#include <GenericTriangle.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_GenericIndexedMesh(py::module &cccorelib)
{
    py::class_<CCCoreLib::VerticesIndexes>(cccorelib, "VerticesIndexes")
        .def(py::init<>())
        .def(py::init<unsigned, unsigned, unsigned>(), "_i1"_a, "_i2"_a, "_i3"_a)
        .def_readwrite("i1", &CCCoreLib::VerticesIndexes::i1)
        .def_readwrite("i2", &CCCoreLib::VerticesIndexes::i2)
        .def_readwrite("i3", &CCCoreLib::VerticesIndexes::i3)
        .def("__getitem__",
             [](CCCoreLib::VerticesIndexes &self, unsigned index)
             {
                 switch (index)
                 {
                 case 0:
                     return self.i1;
                 case 1:
                     return self.i2;
                 case 2:
                     return self.i3;
                 default:
                     throw py::index_error("index out of range");
                 }
             });

    py::class_<CCCoreLib::GenericIndexedMesh, CCCoreLib::GenericMesh>(cccorelib, "GenericIndexedMesh")
        .def("_getTriangle",
             &CCCoreLib::GenericIndexedMesh::_getTriangle,
             "triangleIndex"_a,
             py::return_value_policy::reference)
        .def("getTriangleVertIndexes",
             &CCCoreLib::GenericIndexedMesh::getTriangleVertIndexes,
             "triangleIndex"_a,
             py::return_value_policy::reference)
        .def("getTriangleVertices",
             &CCCoreLib::GenericIndexedMesh::getTriangleVertices,
             "triangleIndex"_a,
             "A"_a,
             "B"_a,
             "C"_a)
        .def("getNextTriangleVertIndexes",
             &CCCoreLib::GenericIndexedMesh::getNextTriangleVertIndexes,
             py::return_value_policy::reference);
}
