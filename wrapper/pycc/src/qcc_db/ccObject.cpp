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

// TODO: This should be made into a proper Pull-Request for pybind11
//  https://github.com/pybind/pybind11/issues/1708
void removePatient(py::object &nurse, py::object &patient)
{
    py::detail::internals &internals = py::detail::get_internals();
    auto instance = reinterpret_cast<py::detail::instance *>(nurse.ptr());
    if (!instance->has_patients)
    {
        return;
    }
    auto &nursePatients = internals.patients[nurse.ptr()];

    auto it = std::find(nursePatients.begin(), nursePatients.end(), patient.ptr());
    if (it == std::end(nursePatients))
    {
        return;
    }

    nursePatients.erase(it);
    if (nursePatients.empty())
    {
        instance->has_patients = false;
    }
    patient.dec_ref();
}

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
        .def("getParent", &ccHObject::getParent, py::return_value_policy::reference_internal)
        // Children management
        .def("getChildrenNumber", &ccHObject::getChildrenNumber)
        .def("getChildCountRecursive", &ccHObject::getChildCountRecursive)
        .def("getChild",
             &ccHObject::getChild,
             "child"_a,
             py::return_value_policy::reference_internal)
        .def("find", &ccHObject::find, "uniqueID"_a, py::return_value_policy::reference_internal)
        .def(
            "detachChild",
            [](py::object &self, py::object &child)
            {
                self.cast<ccHObject *>()->detachChild(child.cast<ccHObject *>());
                removePatient(child, self);
            },
            "child"_a)
        .def("detachAllChildren", &ccHObject::detachAllChildren)
        .def("swapChildren", &ccHObject::swapChildren, "firstChildIndex"_a, "secondChildIndex"_a)
        .def("getChildIndex", &ccHObject::getChildIndex, "child"_a)
        .def("getIndex", &ccHObject::getIndex)
        .def("getFirstChild",
             &ccHObject::getFirstChild,
             py::return_value_policy::reference,
             py::keep_alive<0, 1>())
        .def("getLastChild", &ccHObject::getLastChild, py::return_value_policy::reference_internal)
        .def("isAncestorOf", &ccHObject::isAncestorOf, "anObject"_a);

    py::class_<ccShiftedObject, ccHObject>(m, "ccShiftedObject")
        .def("setGlobalShift",
             (void(ccShiftedObject::*)(double, double, double))(&ccShiftedObject::setGlobalShift),
             "x_"_a,
             "y"_a,
             "z"_a)
        .def("setGlobalShift",
             (void(ccShiftedObject::*)(const CCVector3d &))(&ccShiftedObject::setGlobalShift),
             "shift"_a)
        .def("getGlobalShift", &ccShiftedObject::getGlobalShift)
        .def("setGlobalScale", &ccShiftedObject::setGlobalScale, "scale"_a)
        .def("isShifted", &ccShiftedObject::isShifted)
        .def("getGlobalScale", &ccShiftedObject::setGlobalScale);
}
