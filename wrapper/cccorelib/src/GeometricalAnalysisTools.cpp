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

#include <GenericProgressCallback.h>
#include <GeometricalAnalysisTools.h>
#include <ScalarField.h>

namespace py = pybind11;
using namespace pybind11::literals;

using CCCoreLib::GeometricalAnalysisTools;

void define_GeometricalAnalysisTools(py::module &cccorelib)
{
    py::class_<GeometricalAnalysisTools> GeometricalAnalysisTools(cccorelib, "GeometricalAnalysisTools");
    py::enum_<GeometricalAnalysisTools::GeomCharacteristic> PyGeomCharacteristic(GeometricalAnalysisTools,
                                                                                 "GeomCharacteristic");
    py::enum_<GeometricalAnalysisTools::Density> PyDensity(GeometricalAnalysisTools, "Density");
    py::enum_<GeometricalAnalysisTools::ErrorCode> PyErrorCode(GeometricalAnalysisTools, "ErrorCode");

    GeometricalAnalysisTools.def_static("ComputeCharactersitic",
                                        &GeometricalAnalysisTools::ComputeCharactersitic,
                                        "c"_a,
                                        "subOptions"_a,
                                        "cloud"_a,
                                        "kernelRadius"_a,
                                        "roughnessUpDir"_a = nullptr,
                                        "progressCb"_a = nullptr,
                                        "inputOctree"_a = nullptr);

    GeometricalAnalysisTools.def_static("ComputeLocalDensityApprox",
                                        &GeometricalAnalysisTools::ComputeLocalDensityApprox,
                                        "cloud"_a,
                                        "densityType"_a,
                                        "progressCb"_a = nullptr,
                                        "DgmOctree"_a = nullptr);

    GeometricalAnalysisTools.def_static(
        "ComputeGravityCenter", &GeometricalAnalysisTools::ComputeGravityCenter, "theCloud"_a);
    GeometricalAnalysisTools.def_static("ComputeWeightedGravityCenter",
                                        &GeometricalAnalysisTools::ComputeWeightedGravityCenter,
                                        "theCloud"_a,
                                        "weights"_a);
    GeometricalAnalysisTools.def_static("ComputeCrossCovarianceMatrix",
                                        &GeometricalAnalysisTools::ComputeCrossCovarianceMatrix,
                                        "P"_a,
                                        "Q"_a,
                                        "pGravityCenter"_a,
                                        "qGravityCenter"_a);
    GeometricalAnalysisTools.def_static("ComputeWeightedCrossCovarianceMatrix",
                                        &GeometricalAnalysisTools::ComputeWeightedCrossCovarianceMatrix,
                                        "P"_a,
                                        "Q"_a,
                                        "pGravityCenter"_a,
                                        "qGravityCenter"_a,
                                        "coupleWeights"_a = nullptr);
    GeometricalAnalysisTools.def_static("FlagDuplicatePoints",
                                        &GeometricalAnalysisTools::FlagDuplicatePoints,
                                        "theCloud"_a,
                                        "minDistanceBetweenPoints"_a,
                                        "progressCb"_a = nullptr,
                                        "inputOctree"_a = nullptr);

    GeometricalAnalysisTools.def_static("DetectSphereRobust",
                                        &GeometricalAnalysisTools::DetectSphereRobust,
                                        "cloud"_a,
                                        "outlierRatio"_a,
                                        "center"_a,
                                        "radius"_a,
                                        "rms"_a,
                                        "progressCb"_a = nullptr,
                                        "confidence"_a = 0.99,
                                        "seed"_a = 0);
    GeometricalAnalysisTools.def_static("ComputeSphereFrom4",
                                        &GeometricalAnalysisTools::ComputeSphereFrom4,
                                        "A"_a,
                                        "B"_a,
                                        "C"_a,
                                        "D"_a,
                                        "center"_a,
                                        "radius"_a);

    GeometricalAnalysisTools.def_static("DetectCircle",
                                        &GeometricalAnalysisTools::DetectCircle,
                                        "cloud"_a,
                                        "center"_a,
                                        "normal"_a,
                                        "radius"_a,
                                        "rms"_a,
                                        "progressCb"_a = nullptr);

    PyGeomCharacteristic.value("Feature", GeometricalAnalysisTools::GeomCharacteristic::Feature)
        .value("Curvature", GeometricalAnalysisTools::GeomCharacteristic::Curvature)
        .value("LocalDensity", GeometricalAnalysisTools::GeomCharacteristic::LocalDensity)
        .value("ApproxLocalDensity", GeometricalAnalysisTools::GeomCharacteristic::ApproxLocalDensity)
        .value("Roughness", GeometricalAnalysisTools::GeomCharacteristic::Roughness)
        .value("MomentOrder1", GeometricalAnalysisTools::GeomCharacteristic::MomentOrder1)
        .export_values();

    PyDensity.value("DENSITY_KNN", GeometricalAnalysisTools::Density::DENSITY_KNN)
        .value("DENSITY_2D", GeometricalAnalysisTools::Density::DENSITY_2D)
        .value("DENSITY_3D", GeometricalAnalysisTools::Density::DENSITY_3D)
        .export_values();

    PyErrorCode.value("NoError", GeometricalAnalysisTools::ErrorCode::NoError)
        .value("InvalidInput", GeometricalAnalysisTools::ErrorCode::InvalidInput)
        .value("NotEnoughPoints", GeometricalAnalysisTools::ErrorCode::NotEnoughPoints)
        .value("OctreeComputationFailed", GeometricalAnalysisTools::ErrorCode::OctreeComputationFailed)
        .value("ProcessFailed", GeometricalAnalysisTools::ErrorCode::ProcessFailed)
        .value("UnhandledCharacteristic", GeometricalAnalysisTools::ErrorCode::UnhandledCharacteristic)
        .value("NotEnoughMemory", GeometricalAnalysisTools::ErrorCode::NotEnoughMemory)
        .value("ProcessCancelledByUser", GeometricalAnalysisTools::ErrorCode::ProcessCancelledByUser)
        .export_values();
}
