// ##########################################################################
// #                                                                        #
// #                CLOUDCOMPARE PLUGIN: PythonRuntime                      #
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

#include <ccDisc.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccDisc(py::module &m)
{
    py::class_<ccDisc, ccGenericPrimitive>(m, "ccDisc", R"doc(
    ccDisc

    Parameters
    ----------
    radius : PointCoordinateType
        radius of the disc
    transMat : , optional
         optional 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
    name : str, default: Disc
        name of the disc object
    precision : int, default: 24
        drawing precision (angular step = 360/precision)
    uniqueID : int, optional
        unique ID (handle with care)

    Example
    -------

    .. code:: Python

        disc = pycc.ccDisc(3.0)
        disc2 = pycc.ccDisc(5.0, name="Disc2")
)doc")
		.def(
			py::init<PointCoordinateType, const ccGLMatrix *, QString, unsigned, unsigned>(),
			"radius"_a,
			"transMat"_a = nullptr,
			"name"_a = QString("Disc"),
			"precision"_a = 24,
			"uniqueId"_a = []() { return ccUniqueIDGenerator::InvalidUniqueID; }())
		.def("getRadius", &ccDisc::getRadius)
		.def("setRadius", &ccDisc::setRadius, "radius"_a);
}
