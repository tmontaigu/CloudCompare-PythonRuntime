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

#include <DgmOctree.h>
#include <GenericProgressCallback.h>

namespace py = pybind11;
using namespace pybind11::literals;
using CCCoreLib::DgmOctree;

#include "cccorelib.h"

void define_DgmOctree(py::module &cccorelib)
{
    py::class_<DgmOctree, CCCoreLib::GenericOctree> PyDgmOctree(cccorelib, "DgmOctree");
    py::class_<DgmOctree::PointDescriptor> PyPointDescriptor(PyDgmOctree, "PointDescriptor");
    py::class_<DgmOctree::CellDescriptor> PyCellDescriptor(PyDgmOctree, "CellDescriptor");
    py::class_<DgmOctree::NearestNeighboursSearchStruct> PyNearestNeighboursSearchStruct(
        PyDgmOctree, "NearestNeighboursSearchStruct");

    py::bind_vector<DgmOctree::NeighbourCellsSet>(PyDgmOctree, "NeighbourCellsSet");
    py::bind_vector<DgmOctree::NeighboursSet>(PyDgmOctree, "NeighboursSet");

    PyDgmOctree.def_static("GET_BIT_SHIFT", &DgmOctree::GET_BIT_SHIFT, "level"_a)
        .def_static("OCTREE_LENGTH", &DgmOctree::OCTREE_LENGTH, "level"_a)
        .def_property_readonly_static(
            "MAX_OCTREE_LEVEL", [](const py::object & /* self */) { return DgmOctree::MAX_OCTREE_LEVEL; })
        .def_property_readonly_static(
            "MAX_OCTREE_LENGTH", [](const py::object & /* self */) { return DgmOctree::MAX_OCTREE_LENGTH; })
        .def_property_readonly_static(
            "INVALID_CELL_CODE", [](const py::object & /* self */) { return DgmOctree::INVALID_CELL_CODE; })
        .def("clear", &DgmOctree::clear)
        .def(
            "build",
            [](DgmOctree &self, CCCoreLib::GenericProgressCallback *progressCb = nullptr)
            { return self.build(); },
            "progressCb"_a = nullptr)
        .def(
            "build",
            [](DgmOctree &self,
               const CCVector3 &octreeMin,
               const CCVector3 &octreeMax,
               const CCVector3 *pointsMinFilter = nullptr,
               const CCVector3 *pointsMaxFilter = nullptr,
               CCCoreLib::GenericProgressCallback *progressCb = nullptr)
            { return self.build(octreeMax, octreeMax, pointsMinFilter, pointsMaxFilter, progressCb); },
            "octreeMin"_a,
            "octreeMax"_a,
            "pointsMinFilter"_a = nullptr,
            "pointsMaxFilter"_a = nullptr,
            "progressCb"_a = nullptr)
        .def("getNumberOfProjectedPoints", &DgmOctree::getNumberOfProjectedPoints)
        .def("getOctreeMins", &DgmOctree::getOctreeMins)
        .def("getOctreeMaxs", &DgmOctree::getOctreeMaxs)
        .def("getBoundingBox", &DgmOctree::getBoundingBox, "bbMin"_a, "bbMax"_a)
        .def(
            "getMinFillIndexes", &DgmOctree::getMinFillIndexes, "level"_a, py::return_value_policy::reference)
        .def(
            "getMaxFillIndexes", &DgmOctree::getMaxFillIndexes, "level"_a, py::return_value_policy::reference)
        .def("getCellSize", &DgmOctree::getCellSize, "level"_a)
        //			.def("getCellDistanceFromBorders", )
        //			.def("getCellDistanceFromBorders", )
        ;

    PyPointDescriptor.def_readwrite("point", &DgmOctree::PointDescriptor::point)
        .def_readwrite("pointIndex", &DgmOctree::PointDescriptor::pointIndex)
        .def_readwrite("squareDistd", &DgmOctree::PointDescriptor::squareDistd)
        .def(py::init<>())
        .def(py::init<const CCVector3 *, unsigned>())
        .def(py::init<const CCVector3 *, unsigned, double>())
        .def_static("distComp", &DgmOctree::PointDescriptor::distComp);

    PyCellDescriptor.def_readwrite("center", &DgmOctree::CellDescriptor::center)
        .def_readwrite("index", &DgmOctree::CellDescriptor::index)
        .def(py::init<>())
        .def(py::init<const CCVector3 &, unsigned>());

    PyNearestNeighboursSearchStruct
        .def_readwrite("queryPoint", &DgmOctree::NearestNeighboursSearchStruct::queryPoint)
        .def_readwrite("level", &DgmOctree::NearestNeighboursSearchStruct::level)
        .def_readwrite("minNumberOfNeighbors",
                       &DgmOctree::NearestNeighboursSearchStruct::minNumberOfNeighbors)
        .def_readwrite("cellPos", &DgmOctree::NearestNeighboursSearchStruct::cellPos)
        .def_readwrite("cellCenter", &DgmOctree::NearestNeighboursSearchStruct::cellCenter)
        .def_readwrite("maxSearchSquareDistd",
                       &DgmOctree::NearestNeighboursSearchStruct::maxSearchSquareDistd)
        .def_readwrite("minimalCellsSetToVisit",
                       &DgmOctree::NearestNeighboursSearchStruct::minimalCellsSetToVisit)
        .def_readwrite("pointsInNeighbourhood",
                       &DgmOctree::NearestNeighboursSearchStruct::pointsInNeighbourhood)
        .def_readwrite("alreadyVisitedNeighbourhoodSize",
                       &DgmOctree::NearestNeighboursSearchStruct::alreadyVisitedNeighbourhoodSize)
        .def_readwrite("theNearestPointIndex",
                       &DgmOctree::NearestNeighboursSearchStruct::theNearestPointIndex)
        .def(py::init<>());
}
