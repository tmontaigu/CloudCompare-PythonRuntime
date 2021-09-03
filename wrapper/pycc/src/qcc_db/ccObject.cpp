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

#include <ccHObject.h>
#include <ccObject.h>
#include <ccShiftedObject.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccObject(py::module &m)
{
    py::class_<ccObject>(m, "ccObject")
        .def("getName", &ccObject::getName)
        .def("setName", &ccObject::setName)
        //			.def("getClassID", &ccObject::getClassID)
        .def("getUniqueID", &ccObject::getUniqueID)
        .def("isEnabled", &ccObject::isEnabled)
        .def("setEnabled", &ccObject::setEnabled)
        .def("toggleActivation", &ccObject::toggleActivation)
        .def("isLocked", &ccObject::isLocked)
        .def("setLocked", &ccObject::setLocked)
        .def("isLeaf", &ccObject::isLeaf)
        .def("isCustom", &ccObject::isCustom)
        .def("isHierarchy", &ccObject::isHierarchy)
        .def("getMetaData", &ccObject::getMetaData, "key"_a)
        .def("removeMetaData", &ccObject::removeMetaData, "key"_a)
        .def("setMetaData",
             static_cast<void (ccObject::*)(const QString &, const QVariant &)>(
                 &ccObject::setMetaData),
             "key"_a,
             "data"_a)
        .def("hasMetaData", &ccObject::hasMetaData, "key"_a);

    py::class_<ccHObject, ccObject, ccDrawableObject>(m, "ccHObject")
        .def("isGroup", &ccHObject::isGroup)
        .def("getParent", &ccHObject::getParent, py::return_value_policy::reference)
        // Children management
        .def("getChildrenNumber", &ccHObject::getChildrenNumber)
        .def("getChildCountRecursive", &ccHObject::getChildCountRecursive)
        .def("getChild", &ccHObject::getChild, py::return_value_policy::reference)
        .def("find", &ccHObject::find, py::return_value_policy::reference);

    py::class_<ccShiftedObject, ccHObject>(m, "ccShiftedObject")
        .def("setGlobalShift",
             (void (ccShiftedObject::*)(double, double, double))(&ccShiftedObject::setGlobalShift),
             "x_"_a,
             "y"_a,
             "z"_a)
        .def("setGlobalShift",
             (void (ccShiftedObject::*)(const CCVector3d &))(&ccShiftedObject::setGlobalShift),
             "shift"_a)
        .def("getGlobalShift", &ccShiftedObject::getGlobalShift)
        .def("setGlobalScale", &ccShiftedObject::setGlobalScale, "scale"_a)
        .def("isShifted", &ccShiftedObject::isShifted)
        .def("getGlobalScale", &ccShiftedObject::setGlobalScale);
}
