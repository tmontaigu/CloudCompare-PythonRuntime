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

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <GenericCloud.h>
#include <GenericIndexedCloudPersist.h>
#include <GenericProgressCallback.h>
#include <ScalarFieldTools.h>

namespace py = pybind11;
using namespace pybind11::literals;

using CCCoreLib::ScalarFieldTools;

#include "cccorelib.h"

void define_ScalarFieldTools(py::module &cccorelib)
{
    py::class_<CCCoreLib::KMeanClass> pyKMeanClass(cccorelib, "KMeanClass");
    py::class_<ScalarFieldTools> pyScalarFieldTools(cccorelib, "ScalarFieldTools");

    pyKMeanClass.def_readwrite("mean", &CCCoreLib::KMeanClass::mean)
        .def_readwrite("minValue", &CCCoreLib::KMeanClass::minValue)
        .def_readwrite("maxValue", &CCCoreLib::KMeanClass::maxValue);

    pyScalarFieldTools
        .def_static("computeMeanScalarValue", &ScalarFieldTools::computeMeanScalarValue, "theCloud"_a)
        .def_static(
            "computeMeanSquareScalarValue", &ScalarFieldTools::computeMeanSquareScalarValue, "theCloud"_a)
        .def_static("computeScalarFieldGradient",
                    &ScalarFieldTools::computeScalarFieldGradient,
                    "theCloud"_a,
                    "radius"_a,
                    "euclideanDistances"_a,
                    "sameInAndOutScalarField"_a = false,
                    "progressCb"_a = nullptr,
                    "theOctree"_a = nullptr)
        .def_static("applyScalarFieldGaussianFilter",
                    &ScalarFieldTools::applyScalarFieldGaussianFilter,
                    "sigma"_a,
                    "theCloud"_a,
                    "sigmaSF"_a,
                    "progressCb"_a = nullptr,
                    "theOctree"_a = nullptr)
        .def_static("multiplyScalarFields",
                    &ScalarFieldTools::multiplyScalarFields,
                    "firstCloud"_a,
                    "secondCloud"_a,
                    "progressCb"_a = nullptr)
        .def_static(
            "computeScalarFieldHistogram",
            [](const CCCoreLib::GenericCloud *theCloud, unsigned numberOfClasses)
            {
                std::vector<int> histo;
                ScalarFieldTools::computeScalarFieldHistogram(theCloud, numberOfClasses, histo);
                return py::array_t<int>(histo.size(), histo.data());
            },
            "theCloud"_a,
            "numberOfClasses"_a)
        .def_static(
            "computeScalarFieldExtremas",
            [](const CCCoreLib::GenericCloud *theCloud)
            {
                ScalarType minV{};
                ScalarType maxV{};
                CCCoreLib::ScalarFieldTools::computeScalarFieldExtremas(theCloud, minV, maxV);
                return py::make_tuple(minV, maxV);
            },
            "theCloud"_a)
        .def_static(
            "countScalarFieldValidValues", &ScalarFieldTools::countScalarFieldValidValues, "theCloud"_a)
        .def_static("computeKmeans",
                    &ScalarFieldTools::computeKmeans,
                    "theCloud"_a,
                    "K"_a,
                    "kmcc"_a,
                    "progressCb"_a = nullptr)
        .def_static(
            "SetScalarValueToNaN", &CCCoreLib::ScalarFieldTools::SetScalarValueToNaN, "P"_a, "scalarValue"_a)
        .def_static(
            "SetScalarValueInverted", &ScalarFieldTools::SetScalarValueInverted, "P"_a, "scalarValue"_a)
        .def_static(
            "SetScalarValueInverted", &ScalarFieldTools::SetScalarValueInverted, "P"_a, "scalarValue"_a);
}
