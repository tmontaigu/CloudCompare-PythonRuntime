#include <pybind11/pybind11.h>

#include <DistanceComputationTools.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_DistanceComputationTools(py::module &cccorelib)
{
    py::class_<
        CCCoreLib::DistanceComputationTools> DistanceComputationTools (cccorelib, "DistanceComputationTools");

    py::enum_<CCCoreLib::DistanceComputationTools::ERROR_MEASURES> (DistanceComputationTools, "ERRPOR_MEASURES")
        .value("RMS", CCCoreLib::DistanceComputationTools::ERROR_MEASURES::RMS)
        .value("MAX_DIST_68_PERCENT", CCCoreLib::DistanceComputationTools::ERROR_MEASURES::MAX_DIST_68_PERCENT)
        .value("MAX_DIST_95_PERCENT", CCCoreLib::DistanceComputationTools::ERROR_MEASURES::MAX_DIST_95_PERCENT)
        .value("MAX_DIST_99_PERCENT", CCCoreLib::DistanceComputationTools::ERROR_MEASURES::MAX_DIST_99_PERCENT)
        .value("MAX_DIST", CCCoreLib::DistanceComputationTools::ERROR_MEASURES::MAX_DIST)
        .export_values();
}