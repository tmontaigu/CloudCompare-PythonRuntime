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
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <ccMesh.h>
#include <ccSubMesh.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccSubMesh(py::module &m)
{
    py::class_<ccSubMesh, ccGenericMesh>(m, "ccSubMesh")
        .def(py::init<ccMesh *>(), "parentMesh"_a)
        .def("getTriGlobalIndex", &ccSubMesh::getTriGlobalIndex, "index"_a)
        .def("getCurrentTriGlobalIndex", &ccSubMesh::getCurrentTriGlobalIndex)
        .def("forwardIterator", &ccSubMesh::forwardIterator)
        .def("clear", &ccSubMesh::clear)
        .def("addTriangleIndex",
             static_cast<bool (ccSubMesh::*)(unsigned)>(&ccSubMesh::addTriangleIndex),
             "globalIndex"_a)
        .def("addTriangleIndex",
             static_cast<bool (ccSubMesh::*)(unsigned, unsigned)>(&ccSubMesh::addTriangleIndex),
             "firstIndex"_a,
             "lastIndex"_a)
        .def("setTriangleIndex", &ccSubMesh::setTriangleIndex, "localIndex"_a, "globalIndex"_a)
        .def("reserve", &ccSubMesh::reserve, "n"_a)
        .def("resize", &ccSubMesh::resize, "n"_a)
        .def("getAssociatedMesh",
             static_cast<ccMesh *(ccSubMesh::*)()>(&ccSubMesh::getAssociatedMesh))
        .def("setAssociatedMesh",
             &ccSubMesh::setAssociatedMesh,
             "mesh"_a,
             "unlinkPreviousOne"_a = true)
        // TODO createNewSubMeshFromSelection

        ;
}
