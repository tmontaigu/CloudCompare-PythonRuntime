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

#include <cc2DLabel.h>
#include <ccGenericMesh.h>
#include <ccGenericPointCloud.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_cc2DLabel(py::module &m)
{
    py::class_<cc2DLabel, ccHObject, ccInteractor> Pycc2DLabel(m, "cc2DLabel");
    Pycc2DLabel.def(py::init<QString>(), "name"_a = QString("Label"));
    Pycc2DLabel.def("getRawName", &cc2DLabel::getRawName);
    Pycc2DLabel.def("getLabelContent", &cc2DLabel::getLabelContent, "precision"_a);
    Pycc2DLabel.def("getTitle", &cc2DLabel::getTitle, "precision"_a);
    Pycc2DLabel.def("setPosition", &cc2DLabel::setPosition, "x"_a, "y"_a);
    Pycc2DLabel.def("getPosition",
                    [](const cc2DLabel &self)
                    {
                        py::tuple pos(2);
                        pos[0] = self.getPosition()[0];
                        pos[1] = self.getPosition()[1];
                        return pos;
                    });
    Pycc2DLabel.def("clear", &cc2DLabel::clear, "ignoreDependencies"_a = false);
    Pycc2DLabel.def("size", &cc2DLabel::size);
    Pycc2DLabel.def("addPickedPoint",
                    (bool(cc2DLabel::*)(ccGenericPointCloud *, unsigned, bool)) &
                        cc2DLabel::addPickedPoint,
                    "cloud"_a,
                    "pointIndex"_a,
                    "entityCenter"_a = false);
    Pycc2DLabel.def("addPickedPoint",
                    (bool(cc2DLabel::*)(ccGenericMesh *, unsigned, const CCVector2d &, bool)) &
                        cc2DLabel::addPickedPoint,
                    "cloud"_a,
                    "triangleIndex"_a,
                    "uv"_a,
                    "entityCenter"_a = false);
    Pycc2DLabel.def("setCollapsed", &cc2DLabel::setCollapsed, "state"_a);
    Pycc2DLabel.def("isCollapsed", &cc2DLabel::isCollapsed);
    Pycc2DLabel.def("displayPointLegend", &cc2DLabel::displayPointLegend, "state"_a);
    Pycc2DLabel.def("isPointLegendDisplayed", &cc2DLabel::isPointLegendDisplayed);
    Pycc2DLabel.def("setDisplayedIn2D", &cc2DLabel::setDisplayedIn2D, "state"_a);
    Pycc2DLabel.def("isDisplayedIn2D", &cc2DLabel::isDisplayedIn2D);

    py::class_<cc2DLabel::PickedPoint>(Pycc2DLabel, "PickedPoint")
        .def_readwrite(
            "_cloud", &cc2DLabel::PickedPoint::_cloud, py::return_value_policy::reference)
        .def_readwrite("_mesh", &cc2DLabel::PickedPoint::_mesh, py::return_value_policy::reference)
        .def_readwrite("index", &cc2DLabel::PickedPoint::index)
        .def_readwrite("pos2D", &cc2DLabel::PickedPoint::pos2D)
        .def_readwrite("uv", &cc2DLabel::PickedPoint::uv)
        .def_readwrite("entityCenterPoint", &cc2DLabel::PickedPoint::entityCenterPoint)
        .def("getPointPosition", &cc2DLabel::PickedPoint::getPointPosition)
        .def("cloudOrVertices",
             &cc2DLabel::PickedPoint::cloudOrVertices,
             py::return_value_policy::reference)
        .def("getUniqueID", &cc2DLabel::PickedPoint::getUniqueID)
        .def("entity", &cc2DLabel::PickedPoint::entity, py::return_value_policy::reference)
        .def("itemTitle", &cc2DLabel::PickedPoint::itemTitle)
        .def("prefix", &cc2DLabel::PickedPoint::prefix)
        .def(py::init<>())
        .def(py::init<ccGenericPointCloud *, unsigned, bool>(),
             "_cloud"_a,
             "pointIndex"_a,
             "centerPoint"_a = false)
        .def(py::init<ccGenericMesh *, unsigned, const CCVector2d &, bool>(),
             "_mesh"_a,
             "triIndex"_a,
             "uv"_a,
             "centerPoint"_a = false);

    Pycc2DLabel.def("addPickedPoint",
                    (bool(cc2DLabel::*)(const cc2DLabel::PickedPoint &)) &
                        cc2DLabel::addPickedPoint,
                    "pp"_a);
    Pycc2DLabel.def("getPickedPoint",
                    (const cc2DLabel::PickedPoint &(cc2DLabel::*)(unsigned) const) &
                        cc2DLabel::getPickedPoint,
                    "index"_a);
    Pycc2DLabel.def("getPickedPoint",
                    (cc2DLabel::PickedPoint & (cc2DLabel::*)(unsigned)) & cc2DLabel::getPickedPoint,
                    "index"_a);
}
