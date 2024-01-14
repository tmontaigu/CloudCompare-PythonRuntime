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

#include <ccIndexedTransformation.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccIndexedTransformation(py::module &m)
{
    py::class_<ccIndexedTransformation, ccGLMatrix>(m, "ccIndexedTransformation")
        .def(py::init<>())
        .def("getIndex", &ccIndexedTransformation::getIndex)
        .def("setIndex", &ccIndexedTransformation::setIndex, "index"_a)
        .def_static("Interpolate",
                    &ccIndexedTransformation::Interpolate,
                    "interpIndex"_a,
                    "trans1"_a,
                    "trans2"_a)
        .def(py::self * py::self)
        .def(py::self *= py::self)
        .def("__iadd__",
             [](ccIndexedTransformation &self, const CCVector3 &T) { return self += T; })
        .def("__isub__",
             [](ccIndexedTransformation &self, const CCVector3 &T) { return self -= T; })
        .def("transposed", &ccIndexedTransformation::transposed)
        .def("inverse", &ccIndexedTransformation::inverse);
}
