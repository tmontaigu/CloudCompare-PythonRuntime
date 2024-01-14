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

#include <ccDish.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccDish(py::module &m)
{
    py::class_<ccDish, ccGenericPrimitive>(m, "ccDish", R"doc(
    ccDish

    Parameters
    ----------
    radius : PointCoordinateType
        base radius
    height : PointCoordinateType
        maximum height of dished surface above base
    radius2 : PointCoordinateType, default = 0
        If radius2 is zero, dish is drawn as a section of sphere.
         If radius2 is >0, dish is defined as half of an ellipsoid.
    transMat : , optional
        optional 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
    name : str, default: Sphere
        name of the sphere object

    Example
    -------

    .. code:: Python

    dish = pycc.ccDish(1.0, 4.0)
    )doc")
        .def(
            py::init<PointCoordinateType,
                     PointCoordinateType,
                     PointCoordinateType,
                     const ccGLMatrix *,
                     QString,
                     unsigned>(),
            "radius"_a,
            "height"_a,
            "radius2"_a = 0,
            "transMat"_a = nullptr,
            "name"_a = QString("Dish"),
            "precision"_a = []() { return ccDish::DEFAULT_DRAWING_PRECISION; }());
}
