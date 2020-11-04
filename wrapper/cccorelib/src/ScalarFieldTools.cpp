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
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <GenericCloud.h>
#include <GenericIndexedCloudPersist.h>
#include <GenericProgressCallback.h>
#include <ScalarFieldTools.h>

namespace py = pybind11;
using namespace pybind11::literals;

#include "cccorelib.h"

void define_ScalarFieldTools(py::module &cccorelib)
{
    py::class_<CCCoreLib::KMeanClass>(cccorelib, "KMeanClass")
        .def_readwrite("mean", &CCCoreLib::KMeanClass::mean)
        .def_readwrite("minValue", &CCCoreLib::KMeanClass::minValue)
        .def_readwrite("maxValue", &CCCoreLib::KMeanClass::maxValue);

    py::class_<CCCoreLib::ScalarFieldTools>(cccorelib, "ScalarFieldTools")
        .def_static(
            "computeMeanScalarValue", &CCCoreLib::ScalarFieldTools::computeMeanScalarValue, "theCloud"_a)
        .def_static("computeMeanSquareScalarValue",
                    &CCCoreLib::ScalarFieldTools::computeMeanSquareScalarValue,
                    "theCloud"_a)
        .def_static("computeScalarFieldGradient",
                    &CCCoreLib::ScalarFieldTools::computeScalarFieldGradient,
                    "theCloud"_a,
                    "radius"_a,
                    "euclideanDistances"_a,
                    "sameInAndOutScalarField"_a = false,
                    "progressCb"_a = nullptr,
                    "theOctree"_a = nullptr)
        .def_static("applyScalarFieldGaussianFilter",
                    &CCCoreLib::ScalarFieldTools::applyScalarFieldGaussianFilter,
                    "sigma"_a,
                    "theCloud"_a,
                    "sigmaSF"_a,
                    "progressCb"_a = nullptr,
                    "theOctree"_a = nullptr)
        .def_static("multiplyScalarFields",
                    &CCCoreLib::ScalarFieldTools::multiplyScalarFields,
                    "firstCloud"_a,
                    "secondCloud"_a,
                    "progressCb"_a = nullptr)

        .def_static("computeScalarFieldHistogram",
                    &CCCoreLib::ScalarFieldTools::computeScalarFieldHistogram,
                    "theCloud"_a,
                    "numberOfClasses"_a,
                    "histo"_a)
        .def_static("computeScalarFieldExtremas",
                    &CCCoreLib::ScalarFieldTools::computeScalarFieldExtremas,
                    "theCloud"_a,
                    "minV"_a,
                    "minV"_a)
        .def_static("countScalarFieldValidValues",
                    &CCCoreLib::ScalarFieldTools::countScalarFieldValidValues,
                    "theCloud"_a)
        .def_static("computeKmeans",
                    &CCCoreLib::ScalarFieldTools::computeKmeans,
                    "theCloud"_a,
                    "K"_a,
                    "kmcc"_a,
                    "progressCb"_a = nullptr)
        .def_static(
            "SetScalarValueToNaN", &CCCoreLib::ScalarFieldTools::SetScalarValueToNaN, "P"_a, "scalarValue"_a)
        .def_static("SetScalarValueInverted",
                    &CCCoreLib::ScalarFieldTools::SetScalarValueInverted,
                    "P"_a,
                    "scalarValue"_a);
}