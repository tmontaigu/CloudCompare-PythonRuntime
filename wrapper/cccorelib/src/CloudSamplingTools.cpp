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

#include <CloudSamplingTools.h>

#include <BoundingBox.h>
#include <GenericProgressCallback.h>
#include <GeometricalAnalysisTools.h>
#include <PointCloud.h>
#include <ReferenceCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_CloudSamplingTools(py::module &cccorelib)
{
    py::class_<CCCoreLib::CloudSamplingTools> CloudSamplingTools(cccorelib, "CloudSamplingTools");

    py::enum_<CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD>(CloudSamplingTools,
                                                                     "RESAMPLING_CELL_METHOD")
        .value("CELL_CENTER", CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD::CELL_CENTER)
        .value("CELL_GRAVITY_CENTER",
               CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD::CELL_GRAVITY_CENTER)
        .export_values();

    py::enum_<CCCoreLib::CloudSamplingTools::SUBSAMPLING_CELL_METHOD>(CloudSamplingTools,
                                                                      "SUBSAMPLING_CELL_METHODS")
        .value("RANDOM_POINT", CCCoreLib::CloudSamplingTools::SUBSAMPLING_CELL_METHOD::RANDOM_POINT)
        .value("NEAREST_POINT_TO_CELL_CENTER",
               CCCoreLib::CloudSamplingTools::SUBSAMPLING_CELL_METHOD::NEAREST_POINT_TO_CELL_CENTER)
        .export_values();

    CloudSamplingTools.def_static("resampleCloudWithOctreeAtLevel",
                                  &CCCoreLib::CloudSamplingTools::resampleCloudWithOctreeAtLevel,
                                  "cloud"_a,
                                  "octreeLevel"_a,
                                  "resamplingMethod"_a,
                                  "progressCb"_a = nullptr,
                                  "inputOctree"_a = nullptr);

    CloudSamplingTools.def_static("resampleCloudWithOctree",
                                  &CCCoreLib::CloudSamplingTools::resampleCloudWithOctree,
                                  "cloud"_a,
                                  "newNumberOfPoints"_a,
                                  "resamplingMethod"_a,
                                  "progressCb"_a = nullptr,
                                  "inputOctree"_a = nullptr);

    CloudSamplingTools.def_static("subsampleCloudWithOctreeAtLevel",
                                  &CCCoreLib::CloudSamplingTools::subsampleCloudWithOctreeAtLevel,
                                  "cloud"_a,
                                  "octreeLevel"_a,
                                  "subsamplingMethod"_a,
                                  "progressCb"_a = nullptr,
                                  "inputOctree"_a = nullptr);

    CloudSamplingTools.def_static("subsampleCloudWithOctree",
                                  &CCCoreLib::CloudSamplingTools::subsampleCloudWithOctree,
                                  "cloud"_a,
                                  "newNumberOfPoints"_a,
                                  "subsamplingMethod"_a,
                                  "progressCb"_a = nullptr,
                                  "inputOctree"_a = nullptr);

    CloudSamplingTools.def_static("subsampleCloudRandomly",
                                  &CCCoreLib::CloudSamplingTools::subsampleCloudRandomly,
                                  "cloud"_a,
                                  "newNumberOfPoints"_a,
                                  "progressCb"_a = nullptr);

    py::class_<CCCoreLib::CloudSamplingTools::SFModulationParams>(CloudSamplingTools, "SFModulationParams")
        .def(py::init<bool>())
        .def_readwrite("enabled", &CCCoreLib::CloudSamplingTools::SFModulationParams::enabled)
        .def_readwrite("a", &CCCoreLib::CloudSamplingTools::SFModulationParams::a)
        .def_readwrite("b", &CCCoreLib::CloudSamplingTools::SFModulationParams::b);

    CloudSamplingTools.def_static("resampleCloudSpatially",
                                  &CCCoreLib::CloudSamplingTools::resampleCloudSpatially,
                                  "cloud"_a,
                                  "minDistance"_a,
                                  "modParams"_a,
                                  "octree"_a = nullptr,
                                  "progressCb"_a = nullptr);

    CloudSamplingTools.def_static("sorFilter",
                                  &CCCoreLib::CloudSamplingTools::sorFilter,
                                  "cloud"_a,
                                  "knn"_a = 6,
                                  "nSigma"_a = 1.0,
                                  "octree"_a = nullptr,
                                  "progressCb"_a = nullptr);

    CloudSamplingTools.def_static("noiseFilter",
                                  &CCCoreLib::CloudSamplingTools::noiseFilter,
                                  "cloud"_a,
                                  "kernelRadius"_a,
                                  "nSigma"_a,
                                  "removeIsolatedPoints"_a = false,
                                  "useKnn"_a = false,
                                  "knn"_a = 6,
                                  "useAbsoluteError"_a = true,
                                  "absoluteError"_a = true,
                                  "octree"_a = nullptr,
                                  "progressCb"_a = nullptr);
}
