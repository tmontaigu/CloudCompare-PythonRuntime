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

#include <ccCone.h>
#include <ccCylinder.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccCylinder(py::module &m)
{
    py::class_<ccCylinder, ccCone>(m, "ccCylinder", R"doc(
    ccCylinder

    Parameters
    ----------
    radius : PointCoordinateType
    height : PointCoordinateType
    transMat : , optional
         optional 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
    name : str, default: Sphere
        name of the sphere object
    precision : int, default: 24
        drawing precision (angular step = 360/precision)
    uniqueID : int, optional
        unique ID (handle with care)

    Example
    -------

    .. code:: Python

        cylinder = pycc.ccCylinder(3.0, 10.0)
        cylinder2 = pycc.ccCylinder(radius=5.0, height=50, name="Cylinder2")
)doc")
        .def(
            py::init<PointCoordinateType,
                     PointCoordinateType,
                     const ccGLMatrix *,
                     QString,
                     unsigned,
                     unsigned>(),
            "radius"_a,
            "height"_a,
            "transMat"_a = nullptr,
            "name"_a = QString("Cylinder"),
            "precision"_a = []() { return ccCylinder::DEFAULT_DRAWING_PRECISION; }(),
            "uniqueID"_a = []() { return ccUniqueIDGenerator::InvalidUniqueID; }())
        .def("getRadius", &ccCylinder::getBottomRadius)
        .def("setRadius", &ccCylinder::setBottomRadius, "radius"_a);
}
