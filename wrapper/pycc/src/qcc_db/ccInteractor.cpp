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

#include <cc2DLabel.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccInteractor(py::module &m)
{
    py::class_<ccInteractor>(m, "ccInteractor")
        .def("acceptClick", &ccInteractor::acceptClick, "x"_a, "y"_a, "button"_a)
        .def("move2D",
             &ccInteractor::move2D,
             "x"_a,
             "y"_a,
             "dx"_a,
             "dy"_a,
             "screenWidth"_a,
             "screenHeight"_a)
        .def("move3D", &ccInteractor::move3D, "u"_a);
}
