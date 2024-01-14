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

#include <GenericProgressCallback.h>
#include <KdTree.h>

namespace py = pybind11;
using namespace pybind11::literals;

#include "cccorelib.h"

void define_KdTree(py::module &cccorelib)
{

    py::class_<CCCoreLib::KDTree> a(cccorelib, "KDTree");
    py::bind_vector<std::vector<unsigned>>(a, "IndicesVector", py::module_local(true));

    a.def(py::init<>())
        .def("buildFromCloud", &CCCoreLib::KDTree::buildFromCloud, "cloud"_a, "progressCb"_a = nullptr)
        .def("getAssociatedCloud", &CCCoreLib::KDTree::getAssociatedCloud, py::return_value_policy::reference)
        .def(
            "findNearestNeighbour",
            [](CCCoreLib::KDTree &self, py::sequence &queryPoint, ScalarType maxDist)
            {
                PointCoordinateType point[3];
                point[0] = queryPoint[0].cast<PointCoordinateType>();
                point[1] = queryPoint[1].cast<PointCoordinateType>();
                point[2] = queryPoint[2].cast<PointCoordinateType>();

                unsigned nearestIndex{0};
                py::object ret = py::none();
                if (self.findNearestNeighbour(point, nearestIndex, maxDist))
                {
                    ret = py::cast(nearestIndex);
                }
                return ret;
            },
            "queryPoint"_a,
            "maxDist"_a)
        .def(
            "findNearestNeighbourWithMaxDist",
            [](CCCoreLib::KDTree &self, py::sequence &queryPoint, ScalarType maxDist)
            {
                PointCoordinateType point[3];
                point[0] = queryPoint[0].cast<PointCoordinateType>();
                point[1] = queryPoint[1].cast<PointCoordinateType>();
                point[2] = queryPoint[2].cast<PointCoordinateType>();
                return self.findNearestNeighbourWithMaxDist(point, maxDist);
            },
            "queryPoint"_a,
            "maxDist"_a)
        .def(
            "radiusSearch",
            [](CCCoreLib::KDTree &self,
               py::sequence &queryPoint,
               ScalarType distance,
               ScalarType tolerance,
               std::vector<unsigned> &points)
            {
                PointCoordinateType point[3];
                point[0] = queryPoint[0].cast<PointCoordinateType>();
                point[1] = queryPoint[1].cast<PointCoordinateType>();
                point[2] = queryPoint[2].cast<PointCoordinateType>();
                return self.radiusSearch(point, distance, tolerance, points);
            },
            "queryPoint"_a,
            "distance"_a,
            "tolerance"_a,
            "points"_a);
}
