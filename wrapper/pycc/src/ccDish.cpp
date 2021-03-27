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

#include <ccDish.h>

#include "casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccDish(py::module &m)
{
    py::class_<ccDish, ccGenericPrimitive, std::unique_ptr<ccDish, py::nodelete>>(m, "ccDish")
        .def(py::init<PointCoordinateType,
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
             "precision"_a = [](){ return ccDish::DEFAULT_DRAWING_PRECISION; }());
}
