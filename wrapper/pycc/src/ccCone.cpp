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

#include <ccCone.h>

#include "casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccCone(py::module &m)
{
    py::class_<ccCone, ccGenericPrimitive, std::unique_ptr<ccCone, py::nodelete>>(m, "ccCone")
        .def(py::init<PointCoordinateType,
                      PointCoordinateType,
                      PointCoordinateType,
                      PointCoordinateType,
                      PointCoordinateType,
                      const ccGLMatrix *,
                      QString,
                      unsigned,
                      unsigned>(),
             "bottomRadius"_a,
             "topRadius"_a,
             "height"_a,
             "xOff"_a = 0,
             "yOff"_a = 0,
             "transMat"_a = nullptr,
             "name"_a = QString("Cone"),
             "precision"_a = ccCone::DEFAULT_DRAWING_PRECISION,
             "uniqueID"_a = ccUniqueIDGenerator::InvalidUniqueID)
        .def("getHeight", &ccCone::getHeight)
        .def("setHeight", &ccCone::setHeight, "height"_a)
        .def("getBottomRadius", &ccCone::getBottomRadius)
        .def("setBottomRadius", &ccCone::setBottomRadius, "radius"_a)
        .def("getTopRadius", &ccCone::getTopRadius)
        .def("setTopRadius", &ccCone::setTopRadius, "radius"_a)
        .def("getBottomCenter", &ccCone::getBottomCenter)
        .def("getTopCenter", &ccCone::getTopCenter)
        .def("getSmallCenter", &ccCone::getSmallCenter)
        .def("getLargeCenter", &ccCone::getLargeCenter)
        .def("getSmallRadius", &ccCone::getSmallRadius)
        .def("getLargeRadius", &ccCone::getLargeRadius)
        .def("isSnoutMode", &ccCone::isSnoutMode);
}