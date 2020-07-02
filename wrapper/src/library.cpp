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


PYBIND11_MODULE(pycc, m)
{
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
			.def("__mul__", [](const CCVector3 &self, PointCoordinateType val)
			{
				return self * val;
			})
			.def("__sub__", [](const CCVector3 &self, const CCVector3 &other)
			{
				return self - other;
			})
			.def("__div__", &CCVector3::operator/)
			.def("__add__", &CCVector3::operator+)
			.def("__repr__", [](const CCVector3 &self)
			{
				return "<Vector3(" + std::to_string(self.x) + ", " + std::to_string(self.y) + ", " +
				       std::to_string(self.z) + ")>";
			});

	py::class_<CCCoreLib::BoundingBox>(m, "BoundingBox")
			.def(py::init<CCVector3, CCVector3>())
			.def("minCorner", [](const CCCoreLib::BoundingBox &self)
			{ return self.minCorner(); })
			.def("maxCorner", [](const CCCoreLib::BoundingBox &self)
			{ return self.maxCorner(); })
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
			.def("asArray", [](CCCoreLib::ScalarField &self)
			{
				return PyCC::VectorAsNumpyArray(self);
			})
			.def("__repr__", [](const CCCoreLib::ScalarField &self)
			{ return std::string("<ScalarField(name=") + self.getName() + ")>"; });

	py::class_<ccScalarField, CCCoreLib::ScalarField, observer_ptr<ccScalarField>>(m, "ccScalarField");

	// TODO Metadata { get & set }
	py::class_<ccObject>(m, "ccObject")
			.def("getName", &ccObject::getName)
			.def("setName", &ccObject::setName)
//			.def("getClassID", &ccObject::getClassID)
			.def("getUniqueID", &ccObject::getUniqueID);

	py::class_<ccHObject, ccObject>(m, "ccHObject")
			.def("getChildrenNumber", &ccHObject::getChildrenNumber)
			.def("getChildCountRecursive", &ccHObject::getChildCountRecursive)
			.def("getChild", &ccHObject::getChild, py::return_value_policy::reference)
			.def("toPointCloud", [](ccHObject *obj)
			{
				bool unused{false};
				return ccHObjectCaster::ToPointCloud(obj, &unused);
			}, py::return_value_policy::reference);

	py::class_<ccPointCloud, ccHObject>(m, "ccPointCloud")
			.def("size", &ccPointCloud::size)
					// ScalarField management
			.def("getNumberOfScalarField", &ccPointCloud::getNumberOfScalarFields)
			.def("getScalarFieldName", &ccPointCloud::getScalarFieldName)
			.def("getScalarFieldIndexByName", &ccPointCloud::getScalarFieldIndexByName)
			.def("getCurrentDisplayedScalarField", &ccPointCloud::getCurrentDisplayedScalarField)
			.def("getCurrentDisplayedScalarFieldIndex", &ccPointCloud::getCurrentDisplayedScalarFieldIndex)
			.def("getScalarField", &ccPointCloud::getScalarField)
			.def("__repr__", [](const ccPointCloud& self) {
				return std::string("<ccPointCloud(") + self.getName().toStdString() + ", " + std::to_string(self.size()) + " points)>";
			});


	py::class_<ccPythonInstance, observer_ptr<ccPythonInstance>>(m, "ccPythonInstance")
			.def("haveSelection", &ccPythonInstance::haveSelection)
			.def("haveOneSelection", &ccPythonInstance::haveOneSelection)
			.def("getSelectedEntities", &ccPythonInstance::getSelectedEntities, py::return_value_policy::reference)
			.def("loadFile", &ccPythonInstance::loadFile, py::return_value_policy::reference);

	m.def("GetInstance", &GetInstance);
}