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

#include <GenericIndexedCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_GenericIndexedCloud(py::module &cccorelib)
{
    py::class_<CCCoreLib::GenericIndexedCloud, CCCoreLib::GenericCloud>(cccorelib, "GenericIndexedCloud")
        .def(
            "getPoint",
            [](CCCoreLib::GenericIndexedCloud &self, const unsigned index)
            {
                if (index < self.size())
                {
                    return *self.getPoint(index);
                }
                else
                {
                    throw std::out_of_range(std::string("index ") + std::to_string(index) +
                                            " is out of range");
                }
            },
            py::return_value_policy::reference)
        .def(
            "getPoint",
            [](CCCoreLib::GenericIndexedCloud &self, const unsigned index, CCVector3 &P)
            {
                if (index < self.size())
                {
                    self.getPoint(index, P);
                }
                else
                {
                    throw std::out_of_range(std::string("index ") + std::to_string(index) +
                                            " is out of range");
                }
            },
            py::return_value_policy::reference);
}
