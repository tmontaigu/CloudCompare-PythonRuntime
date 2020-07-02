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
//#                             COPYRIGHT: Thomas Montaigu                 #
//#                                                                        #
//##########################################################################

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <ccHObject.h>
#include <ccHObjectCaster.h>
#include <ccPointCloud.h>
#include <ccScalarField.h>
#include <ccDrawableObject.h>

#include <BoundingBox.h>
#include <ccMainAppInterface.h>

#include "wrappers.h"
#include "casters.h"
#include "exposed.h"


namespace py = pybind11;


template<class T>
using observer_ptr = std::unique_ptr<T, py::nodelete>;


PYBIND11_MODULE(pycc, m) {
	m.doc() = R"pbdoc(
        Python module exposing some CloudCompare functions
    )pbdoc";

	m.def("PrintMessage", &PrintMessage);
	m.def("PrintWarning", &PrintWarning);
	m.def("PrintError", &PrintError);

	py::class_<CCVector3>(m, "Vector3")
			.def(py::init<PointCoordinateType, PointCoordinateType, PointCoordinateType>())
			.def_readwrite("x", &CCVector3::x)
			.def_readwrite("y", &CCVector3::y)
			.def_readwrite("z", &CCVector3::z)
			.def("__mul__", [](const CCVector3 &self, PointCoordinateType val) {
				return self * val;
			})
			.def("__sub__", [](const CCVector3 &self, const CCVector3 &other) {
				return self - other;
			})
			.def("__div__", &CCVector3::operator/)
			.def("__add__", &CCVector3::operator+)
			.def("__repr__", [](const CCVector3 &self) {
				return "<Vector3(" + std::to_string(self.x) + ", " + std::to_string(self.y) + ", " +
				       std::to_string(self.z) + ")>";
			});

	py::class_<CCCoreLib::BoundingBox>(m, "BoundingBox")
			.def(py::init<CCVector3, CCVector3>())
			.def("minCorner", [](const CCCoreLib::BoundingBox &self) { return self.minCorner(); })
			.def("maxCorner", [](const CCCoreLib::BoundingBox &self) { return self.maxCorner(); })
			.def("getCenter", &CCCoreLib::BoundingBox::getCenter)
			.def("getDiagVec", &CCCoreLib::BoundingBox::getDiagVec)
			.def("computeVolume", &CCCoreLib::BoundingBox::computeVolume)
			.def("isValid", &CCCoreLib::BoundingBox::isValid)
			.def("contains", &CCCoreLib::BoundingBox::contains);

	py::class_<ccGenericGLDisplay>(m, "ccGenericGLDisplay");

	py::class_<CCCoreLib::ScalarField, observer_ptr<CCCoreLib::ScalarField>>(m, "ScalarField")
			.def("getName", &CCCoreLib::ScalarField::getName)
			.def("setName", &CCCoreLib::ScalarField::setName)
			.def("computeMeanAndVariance", &CCCoreLib::ScalarField::computeMeanAndVariance)
			.def("computeMinAndMax", &CCCoreLib::ScalarField::computeMinAndMax)
			.def("getMin", &CCCoreLib::ScalarField::getMin)
			.def("getMax", &CCCoreLib::ScalarField::getMax)
			.def("fill", &CCCoreLib::ScalarField::fill)
			.def("asArray", [](CCCoreLib::ScalarField &self) {
				return PyCC::VectorAsNumpyArray(self);
			})
			.def("__repr__", [](const CCCoreLib::ScalarField &self) {
				return std::string("<ScalarField(name=") + self.getName() + ")>";
			});

	py::class_<ccScalarField, CCCoreLib::ScalarField, observer_ptr<ccScalarField>>(m, "ccScalarField");

	py::class_<ccDrawableObject>(m, "ccDrawableObject")
	        .def("isVisible", &ccDrawableObject::isVisible)
	        .def("setVisible", &ccDrawableObject::setVisible)
	        .def("toggleVisibility", &ccDrawableObject::toggleVisibility)
	        .def("isVisibilityLocked", &ccDrawableObject::isVisiblityLocked)
	        .def("lockVisibility", &ccDrawableObject::lockVisibility)
	        .def("isSelected", &ccDrawableObject::isSelected)
	        .def("setSelected", &ccDrawableObject::setSelected)
			.def("hasColors", &ccDrawableObject::hasColors)
			.def("colorsShown", &ccDrawableObject::colorsShown)
			.def("showColors", &ccDrawableObject::showColors)
			.def("toggleColors", &ccDrawableObject::toggleColors)
			.def("hasNormals", &ccDrawableObject::hasNormals)
			.def("normalsShown", &ccDrawableObject::normalsShown)
			.def("showNormals", &ccDrawableObject::showNormals)
			.def("toggleNormals", &ccDrawableObject::toggleNormals)
			.def("hasDisplayedScalarField", &ccDrawableObject::hasDisplayedScalarField)
			.def("hasScalarFields", &ccDrawableObject::hasScalarFields)
			.def("showSF", &ccDrawableObject::showSF)
			.def("toggleSF", &ccDrawableObject::toggleSF)
			.def("sfShown", &ccDrawableObject::sfShown)
	        .def("showNameIn3D", &ccDrawableObject::showNameIn3D)
	        .def("nameShownIn3D", &ccDrawableObject::nameShownIn3D)
	        .def("toggleShowName", &ccDrawableObject::toggleShowName);

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


	py::class_<ccPointCloud, ccHObject>(m, "ccPointCloud")
			.def("size", &ccPointCloud::size)
					// ScalarField management
			.def("getNumberOfScalarField", &ccPointCloud::getNumberOfScalarFields)
			.def("getScalarFieldName", &ccPointCloud::getScalarFieldName)
			.def("getScalarFieldIndexByName", &ccPointCloud::getScalarFieldIndexByName)
			.def("getCurrentDisplayedScalarField", &ccPointCloud::getCurrentDisplayedScalarField)
			.def("getCurrentDisplayedScalarFieldIndex", &ccPointCloud::getCurrentDisplayedScalarFieldIndex)
			.def("getScalarField", &ccPointCloud::getScalarField)
			.def("__repr__", [](const ccPointCloud &self) {
				return std::string("<ccPointCloud(") + self.getName().toStdString() + ", " +
				       std::to_string(self.size()) + " points)>";
			});


	py::class_<ccPythonInstance, observer_ptr<ccPythonInstance>>(m, "ccPythonInstance")
			.def("haveSelection", &ccPythonInstance::haveSelection)
			.def("haveOneSelection", &ccPythonInstance::haveOneSelection)
			.def("getSelectedEntities", &ccPythonInstance::getSelectedEntities, py::return_value_policy::reference)
			.def("setSelectedInDB", &ccPythonInstance::setSelectedInDB)
			.def("dbRootObject", &ccPythonInstance::dbRootObject, py::return_value_policy::reference)
			.def("redrawAll", &ccPythonInstance::redrawAll)
			.def("refreshAll", &ccPythonInstance::refreshAll)
			.def("enableAll", &ccPythonInstance::enableAll)
			.def("disableAll", &ccPythonInstance::disableAll)
			.def("updateUI", &ccPythonInstance::updateUI)
			.def("freezeUI", &ccPythonInstance::freezeUI)
			.def("loadFile", &ccPythonInstance::loadFile, py::return_value_policy::reference);

	m.def("GetInstance", &GetInstance);
}