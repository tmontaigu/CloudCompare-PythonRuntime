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

#include <GenericCloud.h>
#include <GenericDistribution.h>

namespace py = pybind11;
using namespace pybind11::literals;

#include "cccorelib.h"

void define_GenericDistribution(py::module &cccorelib)
{

    py::class_<CCCoreLib::GenericDistribution> GenericDistribution(cccorelib, "GenericDistribution");
    GenericDistribution
        .def("getName", &CCCoreLib::GenericDistribution::getName, py::return_value_policy::reference)
        .def("isValid", &CCCoreLib::GenericDistribution::isValid)
        .def("computeParameters", &CCCoreLib::GenericDistribution::computeParameters, "values"_a)
        .def("computeP",
             (double(CCCoreLib::GenericDistribution::*)(ScalarType)
                  const)(&CCCoreLib::GenericDistribution::computeP),
             "x"_a)
        .def("computeP",
             (double(CCCoreLib::GenericDistribution::*)(ScalarType, ScalarType)
                  const)(&CCCoreLib::GenericDistribution::computeP),
             "x1"_a,
             "x2"_a)
        .def("computePfromZero", &CCCoreLib::GenericDistribution::computePfromZero, "x"_a)
        .def("computeChi2Dist",
             &CCCoreLib::GenericDistribution::computeChi2Dist,
             "GenericDistribution",
             "Yk"_a,
             "numberOfClasses"_a,
             "histo"_a = nullptr);

    py::bind_vector<CCCoreLib::GenericDistribution::ScalarContainer>(GenericDistribution, "ScalarContainer");
}
