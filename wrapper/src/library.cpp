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
#include <GeometricalAnalysisTools.h>
#include <GenericProgressCallback.h>
#include <PointCloud.h>

#include "wrappers.h"
#include "casters.h"
#include "Runtime.h"


namespace py = pybind11;
using namespace pybind11::literals;


template<class T>
using observer_ptr = std::unique_ptr<T, py::nodelete>;

PYBIND11_MAKE_OPAQUE(CCCoreLib::ReferenceCloudContainer);

PYBIND11_MODULE(pycc, m)
{
	m.doc() = R"pbdoc(
        Python module exposing some CloudCompare functions
    )pbdoc";

	py::class_<CCVector2>(m, "Vector2");


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

	py::class_<CCCoreLib::PointCloud, CCCoreLib::GenericIndexedCloudPersist>(m, "PointCloud");

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

	/* CCCoreLib */

	/* contants */
	m.attr("SQRT_3") = CCCoreLib::SQRT_3;

	/* Math */
	m.def("LessThanEpsilon", [](const double x)
	{ return CCCoreLib::LessThanEpsilon(x); });
	m.def("GreaterThanEpsilon", [](const double x)
	{ return CCCoreLib::GreaterThanEpsilon(x); });
	m.def("RadiansToDegrees", [](const double radians)
	{
		return CCCoreLib::RadiansToDegrees(radians);
	});
	m.def("DegreesToRadians", [](const double degrees)
	{
		return CCCoreLib::DegreesToRadians(degrees);
	});

	/**
	 * MiscTools
	 */

	py::class_<CCCoreLib::CCMiscTools> CCMiscTools(m, "CCMiscTools");
	CCMiscTools.def_static("EnlargeBox", &CCCoreLib::CCMiscTools::EnlargeBox);
	CCMiscTools.def_static("MakeMinAndMaxCubical", &CCCoreLib::CCMiscTools::MakeMinAndMaxCubical);
//	CCMiscTools.def_static("ComputeBaseVectors", &CCCoreLib::CCMiscTools::ComputeBaseVectors);
//	CCMiscTools.def_static("TriBoxOverlap", &CCCoreLib::CCMiscTools::TriBoxOverlap);

	/** GenericProgress CallBack
	 *
	 */
	py::class_<CCCoreLib::GenericProgressCallback>(m, "GenericProgressCallback");
	py::class_<CCCoreLib::NormalizedProgress>(m, "NormalizedProgress");


	py::class_<CCCoreLib::GenericOctree>(m, "GenericOctree");
	py::class_<CCCoreLib::DgmOctree, CCCoreLib::GenericOctree>(m, "DgmOctree");


	/**
	 * CloudSamplingTools
	 */

	py::class_<CCCoreLib::CloudSamplingTools> CloudSamplingTools(m, "CloudSamplingTools");

	py::enum_<CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD>(CloudSamplingTools, "RESAMPLING_CELL_METHOD")
			.value("CELL_CENTER", CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD::CELL_CENTER)
			.value("CELL_GRAVITY_CENTER", CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD::CELL_GRAVITY_CENTER)
			.export_values();

	py::enum_<CCCoreLib::CloudSamplingTools::SUBSAMPLING_CELL_METHOD>(CloudSamplingTools, "SUBSAMPLING_CELL_METHODS")
			.value("RANDOM_POINT", CCCoreLib::CloudSamplingTools::SUBSAMPLING_CELL_METHOD::RANDOM_POINT)
			.value("NEAREST_POINT_TO_CELL_CENTER",
			       CCCoreLib::CloudSamplingTools::SUBSAMPLING_CELL_METHOD::NEAREST_POINT_TO_CELL_CENTER)
			.export_values();

	CloudSamplingTools.def_static("resampleCloudWithOctreeAtLevel",
	                              &CCCoreLib::CloudSamplingTools::resampleCloudWithOctreeAtLevel, "cloud"_a,
	                              "octreeLevel"_a, "resamplingMethod"_a, "progressCb"_a = nullptr,
	                              "inputOctree"_a = nullptr);

	CloudSamplingTools.def_static("subsampleCloudWithOctreeAtLevel",
	                              &CCCoreLib::CloudSamplingTools::subsampleCloudWithOctreeAtLevel, "cloud"_a,
	                              "octreeLevel"_a, "subsamplingMethod"_a, "progressCb"_a = nullptr,
	                              "inputOctree"_a = nullptr);


	CloudSamplingTools.def_static("subsampleCloudWithOctree", &CCCoreLib::CloudSamplingTools::subsampleCloudWithOctree,
	                              "cloud"_a,
	                              "newNumberOfPoints"_a, "subsamplingMethod"_a, "progressCb"_a = nullptr,
	                              "inputOctree"_a = nullptr);

	CloudSamplingTools.def_static("subsampleCloudRandomly", &CCCoreLib::CloudSamplingTools::subsampleCloudRandomly,
	                              "cloud"_a,
	                              "newNumberOfPoints"_a, "progressCb"_a = nullptr);

	py::class_<CCCoreLib::CloudSamplingTools::SFModulationParams>(CloudSamplingTools, "SFModulationParams")
			.def(py::init<bool>())
			.def_readwrite("enabled", &CCCoreLib::CloudSamplingTools::SFModulationParams::enabled)
			.def_readwrite("a", &CCCoreLib::CloudSamplingTools::SFModulationParams::a)
			.def_readwrite("b", &CCCoreLib::CloudSamplingTools::SFModulationParams::b);


	CloudSamplingTools.def_static("resampleCloudSpatially", &CCCoreLib::CloudSamplingTools::resampleCloudSpatially,
	                              "cloud"_a, "minDistance"_a,
	                              "modParams"_a, "octree"_a = nullptr, "progressCb"_a = nullptr);

	CloudSamplingTools.def_static("sorFilter", &CCCoreLib::CloudSamplingTools::sorFilter, "cloud"_a, "knn"_a = 6,
	                              "nSigma"_a = 1.0,
	                              "octree"_a = nullptr, "progressCb"_a = nullptr);


	CloudSamplingTools.def_static("noiseFilter", &CCCoreLib::CloudSamplingTools::noiseFilter, "cloud"_a,
	                              "kernelRadius"_a, "nSigma"_a,
	                              "removeIsolatedPoints"_a = false,
	                              "useKnn"_a = false, "knn"_a = 6, "useAbsoluteError"_a = true,
	                              "absoluteError"_a = true, "octree"_a = nullptr,
	                              "progressCb"_a = nullptr);

	/**
	 * GeometricalAnalysisTools
	*/

	py::class_<CCCoreLib::GeometricalAnalysisTools> GeometricalAnalysisTools(m, "GeometricalAnalysisTools");
	GeometricalAnalysisTools.def_static("ComputeCharactersitic",
	                                    [](CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic c,
	                                       int subOption,
	                                       CCCoreLib::GenericIndexedCloudPersist *cloud,
	                                       PointCoordinateType kernelRadius)
	                                    {
		                                    return CCCoreLib::GeometricalAnalysisTools::ComputeCharactersitic(c,
		                                                                                                      subOption,
		                                                                                                      cloud,
		                                                                                                      kernelRadius);
	                                    });

	GeometricalAnalysisTools.def_static("ComputeLocalDensityApprox",
	                                    [](
			                                    CCCoreLib::GenericIndexedCloudPersist *cloud,
			                                    CCCoreLib::GeometricalAnalysisTools::Density densityType)
	                                    {
		                                    return CCCoreLib::GeometricalAnalysisTools::ComputeLocalDensityApprox(cloud,
		                                                                                                          densityType);
	                                    });

	GeometricalAnalysisTools.def_static("ComputeGravityCenter",
	                                    &CCCoreLib::GeometricalAnalysisTools::ComputeGravityCenter);
	GeometricalAnalysisTools.def_static("ComputeWeightedGravityCenter",
	                                    &CCCoreLib::GeometricalAnalysisTools::ComputeWeightedGravityCenter);
	GeometricalAnalysisTools.def_static("ComputeCrossCovarianceMatrix",
	                                    &CCCoreLib::GeometricalAnalysisTools::ComputeCrossCovarianceMatrix);
	GeometricalAnalysisTools.def_static("ComputeWeightedCrossCovarianceMatrix",
	                                    &CCCoreLib::GeometricalAnalysisTools::ComputeWeightedCrossCovarianceMatrix);
	GeometricalAnalysisTools.def_static("FlagDuplicatePoints", [](CCCoreLib::GenericIndexedCloudPersist *theCloud,
	                                                              double minDistanceBetweenPoints = std::numeric_limits<double>::epsilon())
	{
		return CCCoreLib::GeometricalAnalysisTools::FlagDuplicatePoints(theCloud, minDistanceBetweenPoints);
	});

//	GeometricalAnalysisTools.def_static("DetectSphereRobust",
//	                                    &CCCoreLib::GeometricalAnalysisTools::DetectSphereRobust);

	GeometricalAnalysisTools.def_static("ComputeSphereFrom4",
	                                    &CCCoreLib::GeometricalAnalysisTools::ComputeSphereFrom4);


	py::enum_<CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic>(GeometricalAnalysisTools, "GeomCharacteristic")
			.value("Feature", CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic::Feature)
			.value("Curvature", CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic::Curvature)
			.value("LocalDensity", CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic::LocalDensity)
			.value("ApproxLocalDensity", CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic::ApproxLocalDensity)
			.value("Roughness", CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic::Roughness)
			.value("MomentOrder1", CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic::MomentOrder1)
			.export_values();

	py::enum_<CCCoreLib::GeometricalAnalysisTools::Density>(GeometricalAnalysisTools, "Density")
			.value("DENSITY_KNN", CCCoreLib::GeometricalAnalysisTools::Density::DENSITY_KNN)
			.value("DENSITY_2D", CCCoreLib::GeometricalAnalysisTools::Density::DENSITY_2D)
			.value("DENSITY_3D", CCCoreLib::GeometricalAnalysisTools::Density::DENSITY_3D)
			.export_values();

	py::enum_<CCCoreLib::GeometricalAnalysisTools::ErrorCode>(GeometricalAnalysisTools, "ErrorCode")
			.value("NoError", CCCoreLib::GeometricalAnalysisTools::ErrorCode::NoError)
			.value("InvalidInput", CCCoreLib::GeometricalAnalysisTools::ErrorCode::InvalidInput)
			.value("NotEnoughPoints", CCCoreLib::GeometricalAnalysisTools::ErrorCode::NotEnoughPoints)
			.value("OctreeComputationFailed", CCCoreLib::GeometricalAnalysisTools::ErrorCode::OctreeComputationFailed)
			.value("ProcessFailed", CCCoreLib::GeometricalAnalysisTools::ErrorCode::ProcessFailed)
			.value("UnhandledCharacteristic", CCCoreLib::GeometricalAnalysisTools::ErrorCode::UnhandledCharacteristic)
			.value("NotEnoughMemory", CCCoreLib::GeometricalAnalysisTools::ErrorCode::NotEnoughMemory)
			.value("ProcessCancelledByUser",
			       CCCoreLib::GeometricalAnalysisTools::ErrorCode::ProcessCancelledByUser)
			.export_values();


	/**
	 * AutoSegmentationTools
	 */

	m.def("labelConnectedComponents", &CCCoreLib::AutoSegmentationTools::labelConnectedComponents, "theCloud"_a,
	      "level"_a,
	      "sixConnexity"_a = false, "progressCb"_a = nullptr, "inputOctree"_a = nullptr);

	m.def("extractConnectedComponents", &CCCoreLib::AutoSegmentationTools::extractConnectedComponents);

	m.def("GetInstance", &GetInstance);
}