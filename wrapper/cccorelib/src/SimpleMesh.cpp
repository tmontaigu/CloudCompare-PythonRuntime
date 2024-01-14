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

#include <GenericIndexedCloud.h>
#include <GenericIndexedMesh.h>
#include <SimpleMesh.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_SimpleMesh(py::module &cccorelib)
{
    py::class_<CCCoreLib::SimpleMesh, CCCoreLib::GenericIndexedMesh>(cccorelib, "SimpleMesh")
        .def(py::init<CCCoreLib::GenericIndexedCloud *, bool>(),
             "theVertices"_a,
             "linkVerticesWithMesh"_a = false)
        .def("capacity", &CCCoreLib::SimpleMesh::capacity)
        .def("vertices", &CCCoreLib::SimpleMesh::vertices, py::return_value_policy::reference)
        .def("clear", &CCCoreLib::SimpleMesh::clear)
        .def("addTriangle", &CCCoreLib::SimpleMesh::addTriangle, "i1"_a, "i2"_a, "i3"_a)
        .def("reserve", &CCCoreLib::SimpleMesh::reserve, "n"_a)
        .def("resize", &CCCoreLib::SimpleMesh::resize, "n"_a);
}
