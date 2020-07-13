#include <pybind11/pybind11.h>

#include <FastMarching.h>
#include <FastMarchingForPropagation.h>
#include <GenericCloud.h>
#include <ReferenceCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_FastMarching(py::module &cccorelib)
{
    cccorelib.attr("c_FastMarchingNeighbourPosShift") = CCCoreLib::c_FastMarchingNeighbourPosShift;
    py::class_<CCCoreLib::FastMarching>(cccorelib, "FastMarching")
        .def("setSeedCell", &CCCoreLib::FastMarching::setSeedCell, "pos"_a)
        .def("propagate", &CCCoreLib::FastMarching::setSeedCell)
        .def("cleanLastPropagation", &CCCoreLib::FastMarching::cleanLastPropagation)
        .def("getTime", &CCCoreLib::FastMarching::getTime, "pos"_a, "absoluteCoordinates"_a = false)
        .def("setExtendedConnectivity", &CCCoreLib::FastMarching::setExtendedConnectivity, "state"_a);

    // FIX commented def create link errors
    py::class_<CCCoreLib::FastMarchingForPropagation, CCCoreLib::FastMarching>(cccorelib,
                                                                               "FastMarchingForPropagation")
//        .def(py::init<>())
//        .def("init",
//             &CCCoreLib::FastMarchingForPropagation::init,
//             "theCloud"_a,
//             "theOctree"_a,
//             "gridLevel"_a,
//             "constantAcceleration"_a = false)
//        .def("extractPropagatedPoints",
//             &CCCoreLib::FastMarchingForPropagation::extractPropagatedPoints,
//             "Zk"_a)
//        .def("setPropagationTimingsAsDistances",
//             &CCCoreLib::FastMarchingForPropagation::setPropagationTimingsAsDistances)
        .def(
            "setDetectionThreshold", &CCCoreLib::FastMarchingForPropagation::setDetectionThreshold, "value"_a)
        .def("setJumpCoef", &CCCoreLib::FastMarchingForPropagation::setJumpCoef, "value"_a)

//        .def("findPeaks", &CCCoreLib::FastMarchingForPropagation::findPeaks);
        ;
}