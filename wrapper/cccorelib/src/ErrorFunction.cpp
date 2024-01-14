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

#include <ErrorFunction.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ErrorFunction(py::module &cccorelib)
{
    cccorelib.attr("c_erfRelativeError") = CCCoreLib::c_erfRelativeError;
    py::class_<CCCoreLib::ErrorFunction>(cccorelib, "ErrorFunction")
        .def_static("erfc", &CCCoreLib::ErrorFunction::erfc, "x"_a)
        .def_static("erf", &CCCoreLib::ErrorFunction::erf, "x"_a);
}
