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

#include <ChamferDistanceTransform.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ChamferDistanceTransform(py::module &cccorelib)
{

    using GRID3Dus = CCCoreLib::Grid3D<unsigned short>;
    py::class_<GRID3Dus> PyGrid3Dus(cccorelib, "Grid3Dus");
    PyGrid3Dus.def(py::init<>());
    PyGrid3Dus.def("size", &GRID3Dus::size);
    PyGrid3Dus.def("isInitialized", &GRID3Dus::isInitialized);
    PyGrid3Dus.def("init", &GRID3Dus::init);

    py::class_<CCCoreLib::ChamferDistanceTransform, GRID3Dus>(cccorelib, "ChamferDistanceTransform")
        .def_property_readonly_static("MAX_DIST",
                                      [](const py::object & /* self */)
                                      { return CCCoreLib::ChamferDistanceTransform::MAX_DIST; })
        .def("init", &CCCoreLib::ChamferDistanceTransform::init, "gridSize"_a)
        .def("propagateDistance",
             &CCCoreLib::ChamferDistanceTransform::propagateDistance,
             "type"_a,
             "progressCb"_a = nullptr);
}
