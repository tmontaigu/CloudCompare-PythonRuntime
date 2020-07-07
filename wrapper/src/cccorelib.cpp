#include <pybind11/pybind11.h>
#include <CCGeom.h>
#include <BoundingBox.h>
#include <AutoSegmentationTools.h>
#include <GenericProgressCallback.h>
#include <GeometricalAnalysisTools.h>
#include <PointCloud.h>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <CCMath.h>
#include <CloudSamplingTools.h>
#include <ReferenceCloud.h>

#include "wrappers.h"

namespace py = pybind11;
using namespace pybind11::literals;
PYBIND11_MAKE_OPAQUE(CCCoreLib::ReferenceCloudContainer);

//#define QUOTE(str) #str
//#define EXPAND_AND_QUOTE(str) QUOTE(str)

#define DEFINE_VECTOR2TPL_TYPE(cppname, pyname, type_) py::class_<cppname>(m, pyname) \
                .def(py::init<>()) \
                .def(py::init<type_, type_>()) \
                .def_readwrite("x", &cppname::x) \
                .def_readwrite("y", &cppname::y) \
                .def("norm2", &cppname::norm2) \
                .def("norm", &cppname::norm) \
                .def("normalize", &cppname::normalize) \
                .def("dot", &cppname::dot) \
                .def("cross", &cppname::cross) \
                .def("__getitem__", [](const cppname &self, unsigned index) \
                { \
                    return self[index]; \
                }) \
                .def("__repr__", [](const cppname &self) { \
                    return std::string("<")+ pyname + "(" + std::to_string(self.x) + ", " + std::to_string(self.y) + ")>" ; \
                });


PYBIND11_MODULE(cccorelib, m)
{
	/* Constants */
	m.attr("SQRT_3") = CCCoreLib::SQRT_3;

	/* Geoms */

	DEFINE_VECTOR2TPL_TYPE(CCVector2, "CCVector2", PointCoordinateType);
	DEFINE_VECTOR2TPL_TYPE(CCVector2d, "CCVector2d", double);
//	DEFINE_VECTOR2TPL_TYPE(CCVector2i, "CCVector2i", int);


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


	py::class_<CCVector3>(m, "CCVector3")
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
			.def(py::init<>())
			.def(py::init<CCVector3, CCVector3>())
			.def("clear", &CCCoreLib::BoundingBox::clear)
			.def("add", &CCCoreLib::BoundingBox::add)
			.def("minCorner", [](const CCCoreLib::BoundingBox &self)
			{ return self.minCorner(); })
			.def("maxCorner", [](const CCCoreLib::BoundingBox &self)
			{ return self.maxCorner(); })
			.def("getCenter", &CCCoreLib::BoundingBox::getCenter)
			.def("getDiagVec", &CCCoreLib::BoundingBox::getDiagVec)
			.def("getDiagNorm", &CCCoreLib::BoundingBox::getDiagNorm)
			.def("getDiagNormd", &CCCoreLib::BoundingBox::getDiagNormd)
			.def("getMinBoxDim", &CCCoreLib::BoundingBox::getMinBoxDim)
			.def("getMaxBoxDim", &CCCoreLib::BoundingBox::getMaxBoxDim)
			.def("computeVolume", &CCCoreLib::BoundingBox::computeVolume)
			.def("setValidity", &CCCoreLib::BoundingBox::setValidity)
			.def("isValid", &CCCoreLib::BoundingBox::isValid)
			.def("minDistTo", &CCCoreLib::BoundingBox::minDistTo)
			.def("contains", &CCCoreLib::BoundingBox::contains);


	py::class_<CCCoreLib::GenericCloud>(m, "GenericCloud")
			.def("size", &CCCoreLib::GenericCloud::size)
			.def("getBoundingBox", &CCCoreLib::GenericCloud::getBoundingBox)
			.def("testVisibility", &CCCoreLib::GenericCloud::testVisibility)
			.def("placeIteratorAtBeginning", &CCCoreLib::GenericCloud::placeIteratorAtBeginning)
			.def("getNextPoint", &CCCoreLib::GenericCloud::getNextPoint, py::return_value_policy::reference)
			.def("enableScalarField", &CCCoreLib::GenericCloud::enableScalarField)
			.def("isScalarFieldEnabled", &CCCoreLib::GenericCloud::enableScalarField)
			.def("setPointScalarValue", &CCCoreLib::GenericCloud::getPointScalarValue)
			.def("getPointScalarValue", &CCCoreLib::GenericCloud::getPointScalarValue);


	py::class_<CCCoreLib::GenericIndexedCloud, CCCoreLib::GenericCloud>(m, "GenericIndexedCloud")
			.def("getPoint", [](CCCoreLib::GenericIndexedCloud &self, const unsigned index)
			{
				if ( index < self.size())
				{
					return *self.getPoint(index);
				} else
				{
					throw std::out_of_range(std::string("index ") + std::to_string(index) + " is out of range");
				}
			})
			.def("getPoint", [](CCCoreLib::GenericIndexedCloud &self, const unsigned index, CCVector3 &P)
			{
				if ( index < self.size())
				{
					self.getPoint(index, P);
				} else
				{
					throw std::out_of_range(std::string("index ") + std::to_string(index) + " is out of range");
				}
			});

	py::class_<CCCoreLib::GenericIndexedCloudPersist, CCCoreLib::GenericIndexedCloud>(m, "GenericIndexedCloudPersist");
	py::class_<CCCoreLib::ReferenceCloud, CCCoreLib::GenericIndexedCloudPersist>(m, "ReferenceCloud")
			.def("getPointGlobalIndex", &CCCoreLib::ReferenceCloud::getPointGlobalIndex)
			.def("getCurrentPointGlobalIndex", &CCCoreLib::ReferenceCloud::getCurrentPointGlobalIndex)
			.def("getCurrentPointScalarValue", &CCCoreLib::ReferenceCloud::getCurrentPointScalarValue)
			.def("setCurrentPointScalarValue", &CCCoreLib::ReferenceCloud::setCurrentPointScalarValue)
			.def("forwardIterator", &CCCoreLib::ReferenceCloud::forwardIterator)
			.def("addPointIndex", [](CCCoreLib::ReferenceCloud &self, unsigned firstIndex, unsigned lastIndex)
			{
				self.addPointIndex(firstIndex, lastIndex);
			})
			.def("addPointIndex", [](CCCoreLib::ReferenceCloud &self, unsigned globalIndex)
			{
				self.addPointIndex(globalIndex);
			})
			.def("setPointIndex", &CCCoreLib::ReferenceCloud::setPointIndex)
			.def("reserve", &CCCoreLib::ReferenceCloud::reserve)
			.def("resize", &CCCoreLib::ReferenceCloud::resize)
			.def("capacity", &CCCoreLib::ReferenceCloud::capacity)
			.def("swap", &CCCoreLib::ReferenceCloud::swap)
			.def("removeCurrentPointGlobalIndex", &CCCoreLib::ReferenceCloud::removeCurrentPointGlobalIndex)
			.def("removePointGlobalIndex", &CCCoreLib::ReferenceCloud::removePointGlobalIndex)
			.def("getAssociatedCloud", [](CCCoreLib::ReferenceCloud &self)
			{ return self.getAssociatedCloud(); }, py::return_value_policy::reference)
			.def("setAssociatedCloud", &CCCoreLib::ReferenceCloud::setAssociatedCloud)
			.def("add", &CCCoreLib::ReferenceCloud::add)
			.def("invalidateBoundingBox", &CCCoreLib::ReferenceCloud::invalidateBoundingBox);


	py::class_<CCCoreLib::PointCloud, CCCoreLib::GenericIndexedCloudPersist>(m, "PointCloud");


	py::bind_vector<CCCoreLib::ReferenceCloudContainer>(m, "ReferenceCloudContainer");


	py::class_<CCCoreLib::ScalarField, observer_ptr<CCCoreLib::ScalarField>>(m, "ScalarField")
			.def("getName", &CCCoreLib::ScalarField::getName)
			.def("setName", &CCCoreLib::ScalarField::setName)
			.def_static("NaN", &CCCoreLib::ScalarField::NaN)
			.def("computeMeanAndVariance", &CCCoreLib::ScalarField::computeMeanAndVariance)
			.def("computeMinAndMax", &CCCoreLib::ScalarField::computeMinAndMax)
			.def_static("ValidValue", &CCCoreLib::ScalarField::ValidValue)
			.def("flagValueAsInvalid", &CCCoreLib::ScalarField::flagValueAsInvalid)
			.def("getMin", &CCCoreLib::ScalarField::getMin)
			.def("getMax", &CCCoreLib::ScalarField::getMax)
			.def("fill", &CCCoreLib::ScalarField::fill)
			.def("reserveSafe", &CCCoreLib::ScalarField::reserveSafe)
			.def("resizeSafe", &CCCoreLib::ScalarField::resizeSafe)
			.def("asArray", [](CCCoreLib::ScalarField &self)
			{
				return PyCC::VectorAsNumpyArray(self);
			})
			.def("__repr__", [](const CCCoreLib::ScalarField &self)
			{
				return std::string("<ScalarField(name=") + self.getName() + ")>";
			});


	/**
	 * MiscTools
	 */

	py::class_<CCCoreLib::CCMiscTools> CCMiscTools(m, "CCMiscTools");
	CCMiscTools.def_static("EnlargeBox", &CCCoreLib::CCMiscTools::EnlargeBox);
	CCMiscTools.def_static("MakeMinAndMaxCubical", &CCCoreLib::CCMiscTools::MakeMinAndMaxCubical);
	CCMiscTools.def_static("ComputeBaseVectors", [](const CCVector3 &N,
	                                                CCVector3 &X,
	                                                CCVector3 &Y)
	{ CCCoreLib::CCMiscTools::ComputeBaseVectors(N, X, Y); });
//	CCMiscTools.def_static("TriBoxOverlap", &CCCoreLib::CCMiscTools::TriBoxOverlap);

	/** GenericProgress CallBack
	 *
	 */
	py::class_<CCCoreLib::GenericProgressCallback>(m, "GenericProgressCallback")
			.def("update", &CCCoreLib::GenericProgressCallback::update)
			.def("setMethodTitle", &CCCoreLib::GenericProgressCallback::setMethodTitle)
			.def("setInfo", &CCCoreLib::GenericProgressCallback::setInfo)
			.def("start", &CCCoreLib::GenericProgressCallback::start)
			.def("stop", &CCCoreLib::GenericProgressCallback::stop)
			.def("isCancelRequested", &CCCoreLib::GenericProgressCallback::isCancelRequested)
			.def("textCanBeEdited", &CCCoreLib::GenericProgressCallback::textCanBeEdited);

	py::class_<CCCoreLib::NormalizedProgress>(m, "NormalizedProgress")
			//.def(py::init<CCCoreLib::GenericProgressCallback, unsigned, unsigned>(), "callback"_a, "totalSteps"_a, "totalPercentage"_a=100)
			.def("scale", &CCCoreLib::NormalizedProgress::scale, "totalSteps"_a, "totalPercentage"_a = 100,
			     "updateCurrentProgress"_a = false)
			.def("reset", &CCCoreLib::NormalizedProgress::reset)
			.def("oneStep", &CCCoreLib::NormalizedProgress::oneStep)
			.def("steps", &CCCoreLib::NormalizedProgress::steps);


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

	m.def("frontPropagationBasedSegmentation", &CCCoreLib::AutoSegmentationTools::frontPropagationBasedSegmentation,
	      "theCloud"_a, "radius"_a, "minSeeDist"_a, "octreeLevel"_a, "theSegmentedLists"_a, "progressCb"_a = nullptr,
	      "inputOctree"_a = nullptr, "applyGaussianFilter"_a = false, "alpha"_a = 2.0f);


}
