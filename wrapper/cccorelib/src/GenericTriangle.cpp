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

#include <GenericTriangle.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_GenericTriangle(py::module &cccorelib)
{
    py::class_<CCCoreLib::GenericTriangle>(cccorelib, "GenericTriangle")
        .def("_getA", &CCCoreLib::GenericTriangle::_getA, py::return_value_policy::reference)
        .def("_getB", &CCCoreLib::GenericTriangle::_getB, py::return_value_policy::reference)
        .def("_getC", &CCCoreLib::GenericTriangle::_getC, py::return_value_policy::reference);
}
