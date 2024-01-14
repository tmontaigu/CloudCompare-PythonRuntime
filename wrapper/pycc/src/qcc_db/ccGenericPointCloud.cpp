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

#include <ccGenericPointCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccGenericPointCloud(py::module &m)
{
    py::class_<ccGenericPointCloud, ccShiftedObject, CCCoreLib::GenericIndexedCloudPersist>(
        m, "ccGenericPointCloud")
        .def("clone",
             &ccGenericPointCloud::clone,
             "desttClous"_a = nullptr,
             "ignoreChildren"_a = false)
        .def("clear", &ccGenericPointCloud::clear);
}
