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

#include <GenericCloud.h>
#include <NormalDistribution.h>
#include <iostream>

namespace py = pybind11;
using namespace pybind11::literals;

void define_NormalDistribution(py::module &cccorelib)
{
    py::class_<CCCoreLib::NormalDistribution, CCCoreLib::GenericDistribution>(cccorelib, "NormalDistribution")
        .def(py::init<>())
        .def(py::init<ScalarType, ScalarType>(), "_mu"_a, "_sigma2"_a)
        .def("getParameters",
             [](const CCCoreLib::NormalDistribution &self)
             {
                 ScalarType mu, sigma2 = 0.0;
                 self.getParameters(mu, sigma2);
                 return std::make_pair(mu, sigma2);
             })
        .def("setParameters", &CCCoreLib::NormalDistribution::setParameters, "_mu"_a, "_sigma2"_a)
        .def("getMu", &CCCoreLib::NormalDistribution::getMu)
        .def("getSigma2", &CCCoreLib::NormalDistribution::getSigma2)
        .def("computeParameters",
             (bool(CCCoreLib::NormalDistribution::*)(const CCCoreLib::GenericCloud *))(
                 &CCCoreLib::NormalDistribution::computeParameters),
             "cloud"_a)
        .def("computeRobustParameters",
             &CCCoreLib::NormalDistribution::computeRobustParameters,
             "values"_a,
             "nSigma"_a);
}
