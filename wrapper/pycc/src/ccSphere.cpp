//##########################################################################
//#                                                                        #
//#                CLOUDCOMPARE PLUGIN: PythonPlugin                       #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                   COPYRIGHT: Thomas Montaigu                           #
//#                                                                        #
//##########################################################################

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <ccSphere.h>

#include "casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccSphere(py::module &m)
{
    py::class_<ccSphere, ccGenericPrimitive, std::unique_ptr<ccSphere, py::nodelete>>(m, "ccSphere")
        .def(py::init<PointCoordinateType, const ccGLMatrix *, QString, unsigned, unsigned>(),
             "radius"_a,
             "transMat"_a = nullptr,
             "name"_a = QString("Sphere"),
             "precision"_a = 24,
             "uniqueId"_a = ccUniqueIDGenerator::InvalidUniqueID)
        .def("getRadius", &ccSphere::getRadius)
        .def("setRadius", &ccSphere::setRadius, "radius"_a);
}