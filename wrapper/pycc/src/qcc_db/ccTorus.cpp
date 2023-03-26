// ##########################################################################
// #                                                                        #
// #                CLOUDCOMPARE PLUGIN: PythonPlugin                       #
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

#include <ccTorus.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccTorus(py::module &m)
{
    py::class_<ccTorus, ccGenericPrimitive>(m, "ccTorus")
        .def(
            py::init<PointCoordinateType,
                     PointCoordinateType,
                     double,
                     bool,
                     PointCoordinateType,
                     const ccGLMatrix *,
                     QString,
                     unsigned,
                     unsigned>(),
            "insideRadius"_a,
            "outsideRadius"_a,
            "angleRad"_a = 2.0 * M_PI,
            "rectangularSection"_a = false,
            "rectSectionHeight"_a = 0,
            "transMat"_a = nullptr,
            "name"_a = QString("Torus"),
            "precision"_a = []() { return ccTorus::DEFAULT_DRAWING_PRECISION; }(),
            "uniqueId"_a = []() { return ccUniqueIDGenerator::InvalidUniqueID; }());
}
