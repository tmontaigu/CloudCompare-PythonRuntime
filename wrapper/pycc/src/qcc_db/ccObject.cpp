// ##########################################################################
// #                                                                        #
// #                CLOUDCOMPARE PLUGIN: PythonPlugin                       #
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

#include <ccGenericGLDisplay.h>
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
    instance->has_patients = !nursePatients.empty();
    if (!instance->has_patients)
    {
        internals.patients.erase(nurse.ptr());
    }
    patient.dec_ref();
}

void setDeletable(py::object &object, bool state)
{
    auto instance = reinterpret_cast<py::detail::instance *>(object.ptr());
    instance->owned = state;
    for (auto &v_h : py::detail::values_and_holders(instance))
    {
        if (v_h)
        {
            v_h.set_holder_constructed(state);
        }
    }
}

/// This is a hack.
///
/// This function creates a py::object from a value
/// but it makes the object think it does not own the data
/// so when the ref count reaches 0 it does no call the destructor
/// unless setDeletable(obj, true) was called in between
template <typename T> py::object castToFakeOwnedObject(T &&obj)
{
    py::object pyObj = py::cast(obj, py::return_value_policy::take_ownership);
    setDeletable(pyObj, false);
    return pyObj;
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

    py::enum_<ccHObject::DEPENDENCY_FLAGS>(m, "DEPENDENCY_FLAGS")
        .value("DP_NONE", ccHObject::DEPENDENCY_FLAGS::DP_NONE)
        .value("DP_NOTIFY_OTHER_ON_DELETE", ccHObject::DEPENDENCY_FLAGS::DP_NOTIFY_OTHER_ON_DELETE)
        .value("DP_NOTIFY_OTHER_ON_UPDATE", ccHObject::DEPENDENCY_FLAGS::DP_NOTIFY_OTHER_ON_UPDATE)
        .value("DP_DELETE_OTHER", ccHObject::DEPENDENCY_FLAGS::DP_DELETE_OTHER)
        .value("DP_PARENT_OF_OTHER", ccHObject::DEPENDENCY_FLAGS::DP_PARENT_OF_OTHER)
        .export_values();

    py::class_<ccHObject, ccObject, ccDrawableObject>(m, "ccHObject")
        .def("isGroup", &ccHObject::isGroup)
        .def("getParent", &ccHObject::getParent, py::return_value_policy::reference_internal)
        // Children management
        .def("getChildrenNumber", &ccHObject::getChildrenNumber)
        .def("getChildCountRecursive", &ccHObject::getChildCountRecursive)
        .def(
            "getChild",
            [](py::object &self, unsigned int index)
            {
                auto *child = self.cast<ccHObject *>()->getChild(index);
                return castToFakeOwnedObject(child);
            },
            py::keep_alive<0, 1>(),
            "child"_a)
        .def("find", &ccHObject::find, "uniqueID"_a, py::return_value_policy::reference_internal)
        .def(
            "detachChild",
            [](py::object &self, py::object &child)
            {
                self.cast<ccHObject *>()->detachChild(child.cast<ccHObject *>());
                removePatient(child, self);
                setDeletable(child, true);
            },
            "child"_a)
        .def("detachChild", &ccHObject::detachChild, "child"_a)
        .def("detachAllChildren", &ccHObject::detachAllChildren)
        .def("swapChildren", &ccHObject::swapChildren, "firstChildIndex"_a, "secondChildIndex"_a)
        .def("getChildIndex", &ccHObject::getChildIndex, "child"_a)
        .def("getIndex", &ccHObject::getIndex)
        .def(
            "addChild",
            [](ccHObject &self,
               ccHObject *child,
               ccHObject::DEPENDENCY_FLAGS dependencyFlags,
               int insertIndex) { return self.addChild(child, dependencyFlags, insertIndex); },
            py::keep_alive<2, 1>(), // keep alive parent (1) while added child (2) is alive
            "child"_a,
            "dependencyFlags"_a = ccHObject::DEPENDENCY_FLAGS::DP_PARENT_OF_OTHER,
            "insertIndex"_a = -1)
        .def(
            "getFirstChild",
            [](ccHObject &self)
            {
                ccHObject *child = self.getFirstChild();
                return castToFakeOwnedObject(child);
            },
            py::keep_alive<0, 1>())
        .def(
            "getLastChild",
            [](ccHObject &self)
            {
                ccHObject *child = self.getLastChild();
                return castToFakeOwnedObject(child);
            },
            py::keep_alive<0, 1>())
        .def("isAncestorOf", &ccHObject::isAncestorOf, "anObject"_a)
        // bounding-box
        .def("getDisplayBB_recursive",
             &ccHObject::getDisplayBB_recursive,
             "relative"_a,
             "display"_a = nullptr)

        // display
        .def("applyGLTransformation_recursive",
             &ccHObject::applyGLTransformation_recursive,
             "trans"_a = nullptr)
        .def("getGLTransformationHistory", &ccHObject::getGLTransformationHistory)
        .def("setGLTransformationHistory", &ccHObject::setGLTransformationHistory, "mat"_a)
        .def("resetGLTransformationHistory", &ccHObject::resetGLTransformationHistory);

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
