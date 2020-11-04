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

#include <CCMath.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_CCMath(py::module &cccorelib)
{
    cccorelib.def("LessThanEpsilon", [](const double x) { return CCCoreLib::LessThanEpsilon(x); });
    cccorelib.def("GreaterThanEpsilon", [](const double x) { return CCCoreLib::GreaterThanEpsilon(x); });
    cccorelib.def("RadiansToDegrees",
                  [](const double radians) { return CCCoreLib::RadiansToDegrees(radians); });
    cccorelib.def("DegreesToRadians",
                  [](const double degrees) { return CCCoreLib::DegreesToRadians(degrees); });
}