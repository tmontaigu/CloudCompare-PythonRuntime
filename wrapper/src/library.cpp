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
#include <pybind11/stl_bind.h>

#include <ccHObject.h>
#include <ccHObjectCaster.h>
#include <ccPointCloud.h>
#include <ccScalarField.h>
#include <ccDrawableObject.h>
#include <CloudSamplingTools.h>

#include <BoundingBox.h>
#include <ccMainAppInterface.h>
#include <AutoSegmentationTools.h>

#include "wrappers.h"
#include "casters.h"
#include "Runtime.h"


namespace py = pybind11;


template<class T>
using observer_ptr = std::unique_ptr<T, py::nodelete>;

PYBIND11_MAKE_OPAQUE(CCCoreLib::ReferenceCloudContainer);

PYBIND11_MODULE(pycc, m)
{
	m.doc() = R"pbdoc(
        Python module exposing some CloudCompare functions
    )pbdoc";


	py::class_<CCVector3>(m, "Vector3")
			.def(py::init<>())
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


	py::class_<CCCoreLib::GenericCloud>(m, "GenericCloud")
			.def("size", &CCCoreLib::GenericCloud::size)
			.def("getBoundingBox", &CCCoreLib::GenericCloud::getBoundingBox);


	py::class_<CCCoreLib::GenericIndexedCloud, CCCoreLib::GenericCloud>(m, "GenericIndexedCloud");
	py::class_<CCCoreLib::GenericIndexedCloudPersist, CCCoreLib::GenericIndexedCloud>(m, "GenericIndexedCloudPersist");
	py::class_<CCCoreLib::ReferenceCloud, CCCoreLib::GenericIndexedCloudPersist>(m, "ReferenceCloud");

	py::bind_vector<CCCoreLib::ReferenceCloudContainer>(m, "ReferenceCloudContainer");


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
			{
				return std::string("<ScalarField(name=") + self.getName() + ")>";
			});

	py::class_<ccScalarField, CCCoreLib::ScalarField, observer_ptr<ccScalarField>>(m, "ccScalarField");

	py::class_<ccGenericGLDisplay>(m, "ccGenericGLDisplay");

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


	py::class_<ccGenericPointCloud, ccHObject, CCCoreLib::GenericIndexedCloudPersist>(m, "ccGenericPointCloud");

	py::class_<ccPointCloud, ccGenericPointCloud>(m, "ccPointCloud")
			// ScalarField management
			.def("getNumberOfScalarField", &ccPointCloud::getNumberOfScalarFields)
			.def("getScalarFieldName", &ccPointCloud::getScalarFieldName)
			.def("getScalarFieldIndexByName", &ccPointCloud::getScalarFieldIndexByName)
			.def("getCurrentDisplayedScalarField", &ccPointCloud::getCurrentDisplayedScalarField)
			.def("getCurrentDisplayedScalarFieldIndex", &ccPointCloud::getCurrentDisplayedScalarFieldIndex)
			.def("getScalarField", &ccPointCloud::getScalarField)
			.def("setCurrentDisplayedScalarField", &ccPointCloud::setCurrentDisplayedScalarField)
			.def("partialClone", [](const ccPointCloud *self, const CCCoreLib::ReferenceCloud *selection,
			                        ccPythonInstance *pythonInstance)
			{
				// TODO use opt param to check errs
				ccPointCloud *cloned = self->partialClone(selection);
				pythonInstance->addToDB(cloned);
				return cloned;
			}, py::return_value_policy::reference)
			.def("__repr__", [](const ccPointCloud &self)
			{
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


	/**
	 * CLOUD SAMPLING TOOLS
	 */

	py::class_<CCCoreLib::CloudSamplingTools> cloudSamplingTools(m, "CloudSamplingTools");

	py::enum_<CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD>(cloudSamplingTools, "RESAMPLING_CELL_METHOD")
			.value("CELL_CENTER", CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD::CELL_CENTER)
			.value("CELL_GRAVITY_CENTER", CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD::CELL_GRAVITY_CENTER)
			.export_values();

	py::enum_<CCCoreLib::CloudSamplingTools::SUBSAMPLING_CELL_METHOD>(cloudSamplingTools, "SUBSAMPLING_CELL_METHODS")
			.value("RANDOM_POINT", CCCoreLib::CloudSamplingTools::SUBSAMPLING_CELL_METHOD::RANDOM_POINT)
			.value("NEAREST_POINT_TO_CELL_CENTER",
			       CCCoreLib::CloudSamplingTools::SUBSAMPLING_CELL_METHOD::NEAREST_POINT_TO_CELL_CENTER)
			.export_values();

//	m.def("resampleCloudWithOctreeAtLevel", [](CCCoreLib::GenericIndexedCloudPersist *cloud,
//	                                           unsigned char octreeLevel,
//	                                           CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD resamplingMethod)
//	{
//		return CCCoreLib::CloudSamplingTools::resampleCloudWithOctreeAtLevel(cloud, octreeLevel, resamplingMethod);
//	});

	m.def("resampleCloudWithOctree", [](CCCoreLib::GenericIndexedCloudPersist *cloud,
	                                    int newNumberOfPoints,
	                                    CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD resamplingMethod)
	{
		return CCCoreLib::CloudSamplingTools::resampleCloudWithOctree(cloud, newNumberOfPoints, resamplingMethod);
	});

	m.def("subsampleCloudWithOctreeAtLevel", [](CCCoreLib::GenericIndexedCloudPersist *cloud,
	                                            unsigned char octreeLevel,
	                                            CCCoreLib::CloudSamplingTools::SUBSAMPLING_CELL_METHOD subsamplingMethod)
	{
		return CCCoreLib::CloudSamplingTools::subsampleCloudWithOctreeAtLevel(cloud, octreeLevel, subsamplingMethod);
	});

	m.def("subsampleCloudWithOctree", [](CCCoreLib::GenericIndexedCloudPersist *cloud,
	                                     int newNumberOfPoints,
	                                     CCCoreLib::CloudSamplingTools::SUBSAMPLING_CELL_METHOD subsamplingMethod)
	{
		return CCCoreLib::CloudSamplingTools::subsampleCloudWithOctree(cloud, newNumberOfPoints, subsamplingMethod);
	});

	m.def("subsampleCloudRandomly", [](CCCoreLib::GenericIndexedCloudPersist *cloud, int newNumPts)
	{
		return CCCoreLib::CloudSamplingTools::subsampleCloudRandomly(cloud, newNumPts);
	});

	py::class_<CCCoreLib::CloudSamplingTools::SFModulationParams>(cloudSamplingTools, "SFModulationParams")
			.def(py::init<bool>())
			.def_readwrite("enabled", &CCCoreLib::CloudSamplingTools::SFModulationParams::enabled)
			.def_readwrite("a", &CCCoreLib::CloudSamplingTools::SFModulationParams::a)
			.def_readwrite("b", &CCCoreLib::CloudSamplingTools::SFModulationParams::b);

	m.def("resampleCloudSpatially", [](CCCoreLib::GenericIndexedCloudPersist *cloud, PointCoordinateType minDistance,
	                                   const CCCoreLib::CloudSamplingTools::SFModulationParams &modParams)
	{
		return CCCoreLib::CloudSamplingTools::resampleCloudSpatially(cloud, minDistance, modParams);
	});

	m.def("sorFilter", [](CCCoreLib::GenericIndexedCloudPersist *cloud,
	                      int knn = 6,
	                      double nSigma = 1.0)
	{
		return CCCoreLib::CloudSamplingTools::sorFilter(cloud, knn, nSigma);
	});

	m.def("noiseFilter", [](CCCoreLib::GenericIndexedCloudPersist *cloud,
	                        PointCoordinateType kernelRadius,
	                        double nSigma,
	                        bool removeIsolatedPoints = false,
	                        bool useKnn = false,
	                        int knn = 6,
	                        bool useAbsoluteError = true,
	                        double absoluteError = 0.0)
	{
		return CCCoreLib::CloudSamplingTools::noiseFilter(cloud, kernelRadius, nSigma, removeIsolatedPoints, useKnn,
		                                                  knn,
		                                                  useAbsoluteError, absoluteError);
	});


	m.def("labelConnectedComponents",
	      [](CCCoreLib::GenericIndexedCloudPersist *cloud, unsigned char level, bool sixConnectivity = false)
	      {
		      return CCCoreLib::AutoSegmentationTools::labelConnectedComponents(cloud, level, sixConnectivity);
	      });

	m.def("extractConnectedComponents", &CCCoreLib::AutoSegmentationTools::extractConnectedComponents);

	m.def("GetInstance", &GetInstance);
}