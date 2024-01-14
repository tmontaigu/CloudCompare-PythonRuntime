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

#include <CCShareable.h>

#include "wrappers.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_CCShareable(py::module &cccorelib)
{
    py::class_<CCShareable, CCShareableHolder<CCShareable>>(cccorelib, "CCShareable")
        .def(py::init<>())
        .def("link", &CCShareable::link)
        .def("release", &CCShareable::release)
        .def("getLinkCount", &CCShareable::getLinkCount)
#ifdef CC_TRACK_ALIVE_SHARED_OBJECTS
        .def_static("GetAliveCount", &CCShareable::GetAliveCount);
#else
        ;
#endif
}
