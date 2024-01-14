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
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <GenericIndexedCloudPersist.h>
#include <ccPointCloud.h>
#include <ccPolyline.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccPolyline(py::module &m)
{
    py::class_<ccPolyline, CCCoreLib::Polyline, ccShiftedObject>(m, "ccPolyline")
        .def(
            py::init([](CCCoreLib::GenericIndexedCloudPersist *cloud,
                        unsigned uniqueID = ccUniqueIDGenerator::InvalidUniqueID)
                     { return new ccPolyline(cloud, uniqueID); }),
            "associatedCloud"_a,
            "uniqueID"_a = []() { return ccUniqueIDGenerator::InvalidUniqueID; }())

        .def("set2DMode", &ccPolyline::set2DMode, "state"_a)
        .def("is2DMode", &ccPolyline::is2DMode)
        .def("setForeground", &ccPolyline::setForeground, "state"_a)
        .def("setColor", &ccPolyline::setColor, "col"_a)
        .def("setWidth", &ccPolyline::setWidth, "width"_a)
        .def("getWidth", &ccPolyline::getWidth)
        .def("getColor", &ccPolyline::getColor)
        .def(
            "split",
            [](ccPolyline &self, PointCoordinateType maxEdgeLength)
            {
                std::vector<ccPolyline *> parts;
                self.split(maxEdgeLength, parts);
                return parts;
            },
            "maxEdgeLength"_a)
        .def("computeLength", &ccPolyline::computeLength)
        .def("showVertices", &ccPolyline::showVertices, "state"_a)
        .def("verticesShown", &ccPolyline::verticesShown)
        .def("setVertexMarkerWidth", &ccPolyline::setVertexMarkerWidth, "width"_a)
        .def("getVertexMarkerWidth", &ccPolyline::getVertexMarkerWidth)
        // TODO .def("initWith", &ccPolyline::initWith, "vertices"_a, "poly"_a)
        .def("importParametersFrom", &ccPolyline::importParametersFrom, "poly"_a)
        .def("showArrow", &ccPolyline::showArrow, "state"_a, "vertIndex"_a, "length"_a)
        .def("segmentCount", &ccPolyline::segmentCount)
        .def("samplePoints",
             &ccPolyline::samplePoints,
             "densityBased"_a,
             "samplingParameter"_a,
             "withRGB"_a)
        .def("smoothChaikin", &ccPolyline::smoothChaikin, "ratio"_a, "iterationCount"_a);
}
