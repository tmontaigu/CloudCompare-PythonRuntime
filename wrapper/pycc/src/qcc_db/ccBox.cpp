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

#include <ccBox.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccBox(py::module &m)
{
    py::class_<ccBox, ccGenericPrimitive>(m, "ccBox", R"doc(
    ccBox

    Parameters
    ----------
    insideRadius : cccorelib.CCVector3
        box dimensions
    transMat : , optional
        optional 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
    name : str, default: Sphere
        name of the sphere object

    Example
    -------

        >>> import pycc
        >>> import cccorelib
        >>> box = pycc.ccBox(cccorelib.CCVector3(5.0, 10.0, 4.0))
    )doc")
        .def(py::init<const CCVector3 &, const ccGLMatrix *, QString>(),
             "dims"_a,
             "transMat"_a = nullptr,
             "name"_a = QString("Box"))
        .def("getDimensions", &ccBox::getDimensions)
        .def("setDimensions", &ccBox::setDimensions, "dims"_a);
}
