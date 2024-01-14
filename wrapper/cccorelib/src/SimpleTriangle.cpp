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

#include <GenericTriangle.h>
#include <SimpleTriangle.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_SimpleTriangle(py::module &cccorelib)
{
    py::class_<CCCoreLib::SimpleRefTriangle, CCCoreLib::GenericTriangle>(cccorelib, "SimpleRefTriangle")
        .def(py::init<>())
        .def(py::init<const CCVector3 *, const CCVector3 *, const CCVector3 *>(), "_A"_a, "_B"_a, "_C"_a)
        .def_readonly("A", &CCCoreLib::SimpleRefTriangle::A, py::return_value_policy::reference)
        .def_readonly("B", &CCCoreLib::SimpleRefTriangle::B, py::return_value_policy::reference)
        .def_readonly("C", &CCCoreLib::SimpleRefTriangle::C, py::return_value_policy::reference);

    py::class_<CCCoreLib::SimpleTriangle, CCCoreLib::GenericTriangle>(cccorelib, "SimpleTriangle")
        .def(py::init<>())
        .def(py::init<const CCVector3 &, const CCVector3 &, const CCVector3 &>())
        .def_readonly("A", &CCCoreLib::SimpleTriangle::A, py::return_value_policy::reference)
        .def_readonly("B", &CCCoreLib::SimpleTriangle::B, py::return_value_policy::reference)
        .def_readonly("C", &CCCoreLib::SimpleTriangle::C, py::return_value_policy::reference);
}
