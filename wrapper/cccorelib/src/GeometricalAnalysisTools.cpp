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

#include <GenericProgressCallback.h>
#include <GeometricalAnalysisTools.h>
#include <ScalarField.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_GeometricalAnalysisTools(py::module &cccorelib)
{
    py::class_<CCCoreLib::GeometricalAnalysisTools> GeometricalAnalysisTools(cccorelib,
                                                                             "GeometricalAnalysisTools");
    GeometricalAnalysisTools.def_static("ComputeCharactersitic",
                                        &CCCoreLib::GeometricalAnalysisTools::ComputeCharactersitic,
                                        "c"_a,
                                        "subOptions"_a,
                                        "cloud"_a,
                                        "kernelRadius"_a,
                                        "progressCb"_a = nullptr,
                                        "inputOctree"_a = nullptr);

    GeometricalAnalysisTools.def_static("ComputeLocalDensityApprox",
                                        &CCCoreLib::GeometricalAnalysisTools::ComputeLocalDensityApprox,
                                        "cloud"_a,
                                        "densityType"_a,
                                        "progressCb"_a = nullptr,
                                        "DgmOctree"_a = nullptr);

    GeometricalAnalysisTools.def_static(
        "ComputeGravityCenter", &CCCoreLib::GeometricalAnalysisTools::ComputeGravityCenter, "theCloud"_a);
    GeometricalAnalysisTools.def_static("ComputeWeightedGravityCenter",
                                        &CCCoreLib::GeometricalAnalysisTools::ComputeWeightedGravityCenter,
                                        "theCloud"_a,
                                        "weights"_a);
    GeometricalAnalysisTools.def_static("ComputeCrossCovarianceMatrix",
                                        &CCCoreLib::GeometricalAnalysisTools::ComputeCrossCovarianceMatrix,
                                        "P"_a,
                                        "Q"_a,
                                        "pGravityCenter"_a,
                                        "qGravityCenter"_a);
    GeometricalAnalysisTools.def_static(
        "ComputeWeightedCrossCovarianceMatrix",
        &CCCoreLib::GeometricalAnalysisTools::ComputeWeightedCrossCovarianceMatrix,
        "P"_a,
        "Q"_a,
        "pGravityCenter"_a,
        "qGravityCenter"_a,
        "coupleWeights"_a = nullptr);
    GeometricalAnalysisTools.def_static("FlagDuplicatePoints",
                                        &CCCoreLib::GeometricalAnalysisTools::FlagDuplicatePoints,
                                        "theCloud"_a,
                                        "minDistanceBetweenPoints"_a,
                                        "progressCb"_a = nullptr,
                                        "inputOctree"_a = nullptr);

    GeometricalAnalysisTools.def_static("DetectSphereRobust",
                                        &CCCoreLib::GeometricalAnalysisTools::DetectSphereRobust,
                                        "cloud"_a,
                                        "outlierRatio"_a,
                                        "center"_a,
                                        "radius"_a,
                                        "rms"_a,
                                        "progressCb"_a = nullptr,
                                        "confidence"_a = 0.99,
                                        "seed"_a = 0);

    GeometricalAnalysisTools.def_static("ComputeSphereFrom4",
                                        &CCCoreLib::GeometricalAnalysisTools::ComputeSphereFrom4,
                                        "A"_a,
                                        "B"_a,
                                        "C"_a,
                                        "D"_a,
                                        "center"_a,
                                        "radius"_a);

    py::enum_<CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic>(GeometricalAnalysisTools,
                                                                       "GeomCharacteristic")
        .value("Feature", CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic::Feature)
        .value("Curvature", CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic::Curvature)
        .value("LocalDensity", CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic::LocalDensity)
        .value("ApproxLocalDensity",
               CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic::ApproxLocalDensity)
        .value("Roughness", CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic::Roughness)
        .value("MomentOrder1", CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic::MomentOrder1)
        .export_values();

    py::enum_<CCCoreLib::GeometricalAnalysisTools::Density>(GeometricalAnalysisTools, "Density")
        .value("DENSITY_KNN", CCCoreLib::GeometricalAnalysisTools::Density::DENSITY_KNN)
        .value("DENSITY_2D", CCCoreLib::GeometricalAnalysisTools::Density::DENSITY_2D)
        .value("DENSITY_3D", CCCoreLib::GeometricalAnalysisTools::Density::DENSITY_3D)
        .export_values();

    py::enum_<CCCoreLib::GeometricalAnalysisTools::ErrorCode>(GeometricalAnalysisTools, "ErrorCode")
        .value("NoError", CCCoreLib::GeometricalAnalysisTools::ErrorCode::NoError)
        .value("InvalidInput", CCCoreLib::GeometricalAnalysisTools::ErrorCode::InvalidInput)
        .value("NotEnoughPoints", CCCoreLib::GeometricalAnalysisTools::ErrorCode::NotEnoughPoints)
        .value("OctreeComputationFailed",
               CCCoreLib::GeometricalAnalysisTools::ErrorCode::OctreeComputationFailed)
        .value("ProcessFailed", CCCoreLib::GeometricalAnalysisTools::ErrorCode::ProcessFailed)
        .value("UnhandledCharacteristic",
               CCCoreLib::GeometricalAnalysisTools::ErrorCode::UnhandledCharacteristic)
        .value("NotEnoughMemory", CCCoreLib::GeometricalAnalysisTools::ErrorCode::NotEnoughMemory)
        .value("ProcessCancelledByUser",
               CCCoreLib::GeometricalAnalysisTools::ErrorCode::ProcessCancelledByUser)
        .export_values();
}