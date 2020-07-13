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
                                        [](CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic c,
                                           int subOption,
                                           CCCoreLib::GenericIndexedCloudPersist *cloud,
                                           PointCoordinateType kernelRadius) {
                                            return CCCoreLib::GeometricalAnalysisTools::ComputeCharactersitic(
                                                c, subOption, cloud, kernelRadius);
                                        });

    GeometricalAnalysisTools.def_static(
        "ComputeLocalDensityApprox",
        [](CCCoreLib::GenericIndexedCloudPersist *cloud,
           CCCoreLib::GeometricalAnalysisTools::Density densityType) {
            return CCCoreLib::GeometricalAnalysisTools::ComputeLocalDensityApprox(cloud, densityType);
        });

    GeometricalAnalysisTools.def_static("ComputeGravityCenter",
                                        &CCCoreLib::GeometricalAnalysisTools::ComputeGravityCenter);
    GeometricalAnalysisTools.def_static("ComputeWeightedGravityCenter",
                                        &CCCoreLib::GeometricalAnalysisTools::ComputeWeightedGravityCenter);
    GeometricalAnalysisTools.def_static("ComputeCrossCovarianceMatrix",
                                        &CCCoreLib::GeometricalAnalysisTools::ComputeCrossCovarianceMatrix);
    GeometricalAnalysisTools.def_static(
        "ComputeWeightedCrossCovarianceMatrix",
        &CCCoreLib::GeometricalAnalysisTools::ComputeWeightedCrossCovarianceMatrix);
    GeometricalAnalysisTools.def_static(
        "FlagDuplicatePoints",
        [](CCCoreLib::GenericIndexedCloudPersist *theCloud,
           double minDistanceBetweenPoints = std::numeric_limits<double>::epsilon()) {
            return CCCoreLib::GeometricalAnalysisTools::FlagDuplicatePoints(theCloud,
                                                                            minDistanceBetweenPoints);
        });

    GeometricalAnalysisTools.def_static("DetectSphereRobust",
                                        &CCCoreLib::GeometricalAnalysisTools::DetectSphereRobust);

    GeometricalAnalysisTools.def_static("ComputeSphereFrom4",
                                        &CCCoreLib::GeometricalAnalysisTools::ComputeSphereFrom4);

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