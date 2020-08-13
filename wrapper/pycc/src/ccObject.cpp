#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>

#include <ccObject.h>
#include <ccHObject.h>
#include <ccShiftedObject.h>

#include "casters.h"

namespace py = pybind11;
using namespace pybind11::literals;


void define_ccObject(py::module &m) {
	// TODO Metadata { get & set }
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
			.def("isHierarchy", &ccObject::isHierarchy);

	py::class_<ccHObject, ccObject, ccDrawableObject>(m, "ccHObject")
			.def("isGroup", &ccHObject::isGroup)
			.def("getParent", &ccHObject::getParent, py::return_value_policy::reference)
					// Children management
			.def("getChildrenNumber", &ccHObject::getChildrenNumber)
			.def("getChildCountRecursive", &ccHObject::getChildCountRecursive)
			.def("getChild", &ccHObject::getChild, py::return_value_policy::reference)
			.def("find", &ccHObject::find, py::return_value_policy::reference);

	py::class_<ccShiftedObject, ccHObject>(m, "ccShiftedObject");
}
