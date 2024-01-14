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

#include <LocalModel.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_LocalModel(py::module &cccorelib)
{
    py::class_<CCCoreLib::LocalModel>(cccorelib, "LocalModel")
        // TODO
        //			.def_static("New", &CCCoreLib::LocalModel::New, "type"_a, "subset"_a,
        //"center"_a, "squaredRadius"_a)
        .def("getType", &CCCoreLib::LocalModel::getType)
        .def("getCenter", &CCCoreLib::LocalModel::getCenter)
        .def("getSquareSize", &CCCoreLib::LocalModel::getSquareSize)
        .def("computeDistanceFromModelToPoint",
             &CCCoreLib::LocalModel::computeDistanceFromModelToPoint,
             "P"_a,
             "nearestPoint"_a = nullptr);
}
