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

#include <GenericIndexedMesh.h>
#include <GenericProgressCallback.h>
#include <RegistrationTools.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_RegistrationTools(py::module &cccorelib)
{
    py::class_<CCCoreLib::RegistrationTools> RegistrationTools(cccorelib, "RegistrationTools");

    py::enum_<CCCoreLib::RegistrationTools::TRANSFORMATION_FILTERS>(RegistrationTools,
                                                                    "TRANSFORMATION_FILTERS")
        .value("SKIP_NONE", CCCoreLib::RegistrationTools::TRANSFORMATION_FILTERS::SKIP_NONE)
        .value("SKIP_RXY", CCCoreLib::RegistrationTools::TRANSFORMATION_FILTERS::SKIP_RXY)
        .value("SKIP_RYZ", CCCoreLib::RegistrationTools::TRANSFORMATION_FILTERS::SKIP_RYZ)
        .value("SKIP_RXZ", CCCoreLib::RegistrationTools::TRANSFORMATION_FILTERS::SKIP_RXZ)
        .value("SKIP_ROTATION", CCCoreLib::RegistrationTools::TRANSFORMATION_FILTERS::SKIP_ROTATION)
        .value("SKIP_TX", CCCoreLib::RegistrationTools::TRANSFORMATION_FILTERS::SKIP_TX)
        .value("SKIP_TY", CCCoreLib::RegistrationTools::TRANSFORMATION_FILTERS::SKIP_TY)
        .value("SKIP_TZ", CCCoreLib::RegistrationTools::TRANSFORMATION_FILTERS::SKIP_TZ)
        .value("SKIP_TRANSLATION", CCCoreLib::RegistrationTools::TRANSFORMATION_FILTERS::SKIP_TRANSLATION)
        .export_values();

    RegistrationTools.def_static("FilterTransformation",
                                 &CCCoreLib::RegistrationTools::FilterTransformation,
                                 "inTrans"_a,
                                 "transformationFilters"_a,
                                 "toBeAlignedGravityCenter"_a,
                                 "referenceGravityCenter"_a,
                                 "outTrans"_a);

    // HornRegistrationTools

    py::class_<CCCoreLib::HornRegistrationTools, CCCoreLib::RegistrationTools>(cccorelib,
                                                                               "HornRegistrationTools")
        .def_static("FindAbsoluteOrientation",
                    &CCCoreLib::HornRegistrationTools::FindAbsoluteOrientation,
                    "lCloud"_a,
                    "rCloud"_a,
                    "trans"_a,
                    "fixedScale"_a = false)
        .def_static(
            "ComputeRMS", &CCCoreLib::HornRegistrationTools::ComputeRMS, "lCloud"_a, "rCloud"_a, "trans"_a);

    // ICPRegistrationTools

    py::class_<CCCoreLib::ICPRegistrationTools, CCCoreLib::RegistrationTools> ICPRegistrationTools(
        cccorelib, "ICPRegistrationTools");
    py::enum_<CCCoreLib::ICPRegistrationTools::CONVERGENCE_TYPE> PyConvergenceType(ICPRegistrationTools,
                                                                                   "CONVERGENCE_TYPE");
    py::enum_<CCCoreLib::ICPRegistrationTools::RESULT_TYPE> PyResultType(ICPRegistrationTools, "RESULT_TYPE");
    py::class_<CCCoreLib::ICPRegistrationTools::Parameters> PyParameters(ICPRegistrationTools, "Parameters");

    ICPRegistrationTools.def_static("Register",
                                    &CCCoreLib::ICPRegistrationTools::Register,
                                    "modelCloud"_a,
                                    "modelMesh"_a,
                                    "dataCloud"_a,
                                    "params"_a,
                                    "totalTrans"_a,
                                    "finalRMS"_a,
                                    "finalPointCount"_a,
                                    "progressCb"_a = nullptr);

    PyConvergenceType
        .value("MAX_ERROR_CONVERGENCE",
               CCCoreLib::ICPRegistrationTools::CONVERGENCE_TYPE::MAX_ERROR_CONVERGENCE)
        .value("MAX_ITER_CONVERGENCE",
               CCCoreLib::ICPRegistrationTools::CONVERGENCE_TYPE::MAX_ITER_CONVERGENCE)
        .export_values();

    PyResultType.value("ICP_NOTHING_TO_DO", CCCoreLib::ICPRegistrationTools::RESULT_TYPE::ICP_NOTHING_TO_DO)
        .value("ICP_APPLY_TRANSFO", CCCoreLib::ICPRegistrationTools::RESULT_TYPE::ICP_APPLY_TRANSFO)
        .value("ICP_ERROR", CCCoreLib::ICPRegistrationTools::RESULT_TYPE::ICP_ERROR)

        .value("ICP_ERROR_REGISTRATION_STEP",
               CCCoreLib::ICPRegistrationTools::RESULT_TYPE::ICP_ERROR_REGISTRATION_STEP)
        .value("ICP_ERROR_DIST_COMPUTATION",
               CCCoreLib::ICPRegistrationTools::RESULT_TYPE::ICP_ERROR_DIST_COMPUTATION)
        .value("ICP_ERROR_NOT_ENOUGH_MEMORY",
               CCCoreLib::ICPRegistrationTools::RESULT_TYPE::ICP_ERROR_NOT_ENOUGH_MEMORY)
        .value("ICP_ERROR_CANCELED_BY_USER",
               CCCoreLib::ICPRegistrationTools::RESULT_TYPE::ICP_ERROR_CANCELED_BY_USER)
        .value("ICP_ERROR_INVALID_INPUT",
               CCCoreLib::ICPRegistrationTools::RESULT_TYPE::ICP_ERROR_INVALID_INPUT)
        .export_values();

    PyParameters.def(py::init<>())
        .def_readwrite("convType", &CCCoreLib::ICPRegistrationTools::Parameters::convType)
        .def_readwrite("minRMSDecrease", &CCCoreLib::ICPRegistrationTools::Parameters::minRMSDecrease)
        .def_readwrite("nbMaxIterations", &CCCoreLib::ICPRegistrationTools::Parameters::nbMaxIterations)
        .def_readwrite("adjustScale", &CCCoreLib::ICPRegistrationTools::Parameters::adjustScale)
        .def_readwrite("filterOutFarthestPoints",
                       &CCCoreLib::ICPRegistrationTools::Parameters::filterOutFarthestPoints)
        .def_readwrite("samplingLimit", &CCCoreLib::ICPRegistrationTools::Parameters::samplingLimit)
        .def_readwrite("finalOverlapRatio", &CCCoreLib::ICPRegistrationTools::Parameters::finalOverlapRatio)
        .def_readwrite("modelWeights", &CCCoreLib::ICPRegistrationTools::Parameters::modelWeights)
        .def_readwrite("dataWeights", &CCCoreLib::ICPRegistrationTools::Parameters::dataWeights)
        .def_readwrite("transformationFilters",
                       &CCCoreLib::ICPRegistrationTools::Parameters::transformationFilters)
        .def_readwrite("maxThreadCount", &CCCoreLib::ICPRegistrationTools::Parameters::maxThreadCount);

    // FPCSRegistrationTools

    py::class_<CCCoreLib::FPCSRegistrationTools, CCCoreLib::RegistrationTools>(cccorelib,
                                                                               "FPCSRegistrationTools")
        .def_static("RegisterClouds",
                    &CCCoreLib::FPCSRegistrationTools::RegisterClouds,
                    "modelCloud"_a,
                    "dataCloud"_a,
                    "transform"_a,
                    "delta"_a,
                    "beta"_a,
                    "overlap"_a,
                    "nbBases"_a,
                    "nbTries"_a,
                    "progressCb"_a = nullptr,
                    "nbMaxCandidates"_a = 0);
}
