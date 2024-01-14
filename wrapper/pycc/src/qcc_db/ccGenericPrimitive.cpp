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

#include <ccGLMatrix.h>
#include <ccGenericPrimitive.h>
#include <ccMesh.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccGenericPrimitive(py::module &m)
{
    py::class_<ccGenericPrimitive, ccMesh>(m, "ccGenericPrimitive")
        .def("getTypeName", &ccGenericPrimitive::getTypeName)
        .def("clone", &ccGenericPrimitive::getTypeName)
        .def("setColor", &ccGenericPrimitive::setColor, "col"_a)
        .def("hasDrawingPrecision", &ccGenericPrimitive::hasDrawingPrecision)
        .def("setDrawingPrecision", &ccGenericPrimitive::setDrawingPrecision, "steps"_a)
        .def("getDrawingPrecision", &ccGenericPrimitive::getDrawingPrecision)
        .def("getTransformation",
             (const ccGLMatrix &(ccGenericPrimitive::*)()
                  const)(&ccGenericPrimitive::getTransformation));
}
