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
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <ccPointCloud.h>
#include <ccProgressDialog.h>
#include <ccRasterGrid.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccRasterGrid(py::module &m)
{
    py::class_<ccRasterCell>(m, "ccRasterCell")
        .def(py::init<>())
        .def_readwrite("h", &ccRasterCell::h)
        .def_readwrite("avgHeight", &ccRasterCell::avgHeight)
        .def_readwrite("stdDevHeight", &ccRasterCell::stdDevHeight)
        .def_readwrite("minHeight", &ccRasterCell::minHeight)
        .def_readwrite("maxHeight", &ccRasterCell::maxHeight)
        .def_readwrite("nbPoints", &ccRasterCell::nbPoints)
        .def_readwrite("pointIndex", &ccRasterCell::pointIndex)
        .def_readwrite("color", &ccRasterCell::color);

    py::class_<ccRasterGrid> PyRasterGrid(m, "ccRasterGrid");

    py::enum_<ccRasterGrid::ExportableFields>(PyRasterGrid, "ExportableFields")
        .value("PER_CELL_HEIGHT", ccRasterGrid::ExportableFields::PER_CELL_HEIGHT)
        .value("PER_CELL_COUNT", ccRasterGrid::ExportableFields::PER_CELL_COUNT)
        .value("PER_CELL_MIN_HEIGHT", ccRasterGrid::ExportableFields::PER_CELL_MIN_HEIGHT)
        .value("PER_CELL_MAX_HEIGHT", ccRasterGrid::ExportableFields::PER_CELL_MAX_HEIGHT)
        .value("PER_CELL_AVG_HEIGHT", ccRasterGrid::ExportableFields::PER_CELL_AVG_HEIGHT)
        .value("PER_CELL_HEIGHT_STD_DEV", ccRasterGrid::ExportableFields::PER_CELL_HEIGHT_STD_DEV)
        .value("PER_CELL_HEIGHT_RANGE", ccRasterGrid::ExportableFields::PER_CELL_HEIGHT_RANGE)
        .value("PER_CELL_INVALID", ccRasterGrid::ExportableFields::PER_CELL_INVALID)
        .export_values();

    py::enum_<ccRasterGrid::ProjectionType>(PyRasterGrid, "ProjectionType")
        .value("PROJ_MINIMUM_VALUE", ccRasterGrid::ProjectionType::PROJ_MINIMUM_VALUE)
        .value("PROJ_AVERAGE_VALUE", ccRasterGrid::ProjectionType::PROJ_AVERAGE_VALUE)
        .value("PROJ_MAXIMUM_VALUE", ccRasterGrid::ProjectionType::PROJ_MAXIMUM_VALUE)
        .value("INVALID_PROJECTION_TYPE", ccRasterGrid::ProjectionType::INVALID_PROJECTION_TYPE)
        .export_values();

    py::enum_<ccRasterGrid::EmptyCellFillOption>(PyRasterGrid, "EmptyCellFillOption")
        .value("LEAVE_EMPTY", ccRasterGrid::EmptyCellFillOption::LEAVE_EMPTY)
        .value("FILL_MINIMUM_HEIGHT", ccRasterGrid::EmptyCellFillOption::FILL_MINIMUM_HEIGHT)
        .value("FILL_MAXIMUM_HEIGHT", ccRasterGrid::EmptyCellFillOption::FILL_MAXIMUM_HEIGHT)
        .value("FILL_CUSTOM_HEIGHT", ccRasterGrid::EmptyCellFillOption::FILL_CUSTOM_HEIGHT)
        .value("FILL_AVERAGE_HEIGHT", ccRasterGrid::EmptyCellFillOption::FILL_AVERAGE_HEIGHT)
        .value("INTERPOLATE", ccRasterGrid::EmptyCellFillOption::INTERPOLATE)
        .export_values();

    PyRasterGrid.def(py::init<>())
        .def_static("ComputeGridSize",
                    &ccRasterGrid::ComputeGridSize,
                    "Z"_a,
                    "box"_a,
                    "gridStep"_a,
                    "width"_a,
                    "height"_a)
        .def("init", &ccRasterGrid::ComputeGridSize, "w"_a, "h"_a, "gridStep"_a, "minCorner"_a)
        .def("reset", &ccRasterGrid::reset)
        .def_static("GetDefaultFieldName", &ccRasterGrid::GetDefaultFieldName, "field"_a)
        .def("convertToCloud",
             &ccRasterGrid::convertToCloud,
             "exportedFields"_a,
             "interpolateSF"_a,
             "interpolateColors"_a,
             "resampleInputCloudXY"_a,
             "resampleInputCloudZ"_a,
             "inputCloud"_a,
             "Z"_a,
             "box"_a,
             "fillEmptyCells"_a,
             "emptyCellsHeight"_a,
             "exportToOriginalCS"_a)
        .def("fillWith",
             &ccRasterGrid::fillWith,
             "cloud"_a,
             "projectionDimension"_a,
             "maxEdgeLength"_a,
             "projectionType"_a,
             "interpolateEmptyCells"_a,
             "sfInterpolation"_a = ccRasterGrid::ProjectionType::INVALID_PROJECTION_TYPE,
             "progressDialog"_a = nullptr)
        .def("fillEmptyCells",
             &ccRasterGrid::fillEmptyCells,
             "projectionDimension"_a,
             "customCellHeight"_a = 0)
        .def("updateNonEmptyCellCount", &ccRasterGrid::updateNonEmptyCellCount)
        .def("updateCellStats", &ccRasterGrid::updateCellStats)
        .def("interpolateEmptyCells", &ccRasterGrid::interpolateEmptyCells, "maxSquareEdgeLength"_a)
        .def("setValid", &ccRasterGrid::setValid, "state"_a)
        .def("isValid", &ccRasterGrid::isValid)
        .def("computeCellPos", &ccRasterGrid::computeCellPos, "P"_a, "X"_a, "Y"_a)
        .def("computeCellCenter", &ccRasterGrid::computeCellCenter, "i"_a, "j"_a, "X"_a, "Y"_a)
        // TODO rows
        // todo scalarfields
        .def_readwrite("width", &ccRasterGrid::width)
        .def_readwrite("height", &ccRasterGrid::height)
        .def_readwrite("gridStep", &ccRasterGrid::gridStep)
        .def_readwrite("minCorner", &ccRasterGrid::minCorner)
        .def_readwrite("minHeight", &ccRasterGrid::minHeight)
        .def_readwrite("maxHeight", &ccRasterGrid::maxHeight)
        .def_readwrite("meanHeight", &ccRasterGrid::meanHeight)
        .def_readwrite("nonEmptyCellCount", &ccRasterGrid::nonEmptyCellCount)
        .def_readwrite("validCellCount", &ccRasterGrid::validCellCount)
        .def_readwrite("hasColors", &ccRasterGrid::hasColors)
        .def_readwrite("valid", &ccRasterGrid::valid);
}
