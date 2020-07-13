#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <CCGeom.h>
#include <BoundingBox.h>
#include <AutoSegmentationTools.h>
#include <GenericProgressCallback.h>
#include <GeometricalAnalysisTools.h>
#include <PointCloud.h>
#include <ReferenceCloud.h>
#include <Grid3D.h>
#include <ChamferDistanceTransform.h>
#include <TrueKdTree.h>


namespace py = pybind11;
using namespace pybind11::literals;
PYBIND11_MAKE_OPAQUE(CCCoreLib::ReferenceCloudContainer);

//#define QUOTE(str) #str
//#define EXPAND_AND_QUOTE(str) QUOTE(str)

void define_AutoSegmentationTools(py::module &);

void define_BoundingBox(py::module &);

void define_CCGeom(py::module &);

void define_CCMath(py::module &);


void define_ChamferDistanceTransform(py::module &);

void define_CloudSamplingTools(py::module &);

void define_DgmOctree(py::module &);

void define_DgmOctreeReferenceCloud(py::module &);

void define_GenericCloud(py::module &);

void define_TrueKdTree(py::module &);

void define_GenericDistribution(py::module &);

void define_StatisticalTestingTools(py::module &);

void define_GenericTriangle(py::module &);

void define_SimpleTriangle(py::module &);

void define_GenericIndexedCloud(py::module &);

void define_GenericProgressCallback(py::module &);

void define_GeometricalAnalysisTools(py::module &);

void define_GenericMesh(py::module &);

void define_SimpleMesh(py::module &cccorelib);

void define_GenericIndexedMesh(py::module &);

void define_WeibullDistribution(py::module &);

void define_ScalarField(py::module &cccorelib);

void define_ScalarFieldTools(py::module &cccorelib);

void define_RegistrationTools(py::module &cccorelib);

void define_ReferenceCloud(py::module &cccorelib);

void define_Polyline(py::module &cccorelib);

void define_PointProjectionTools(py::module &cccorelib);

void define_GenericIndexedCloudPersist(py::module &cccorelib);

void define_PointCloud(py::module &cccorelib);

void define_GenericOctree(py::module &cccorelib);

void define_KdTree(py::module &cccorelib);

void define_LocalModel(py::module &cccorelib);

void define_ManualSegmentationTools(py::module &);

void define_CCMiscTools(py::module&);

PYBIND11_MODULE(cccorelib, m)
{
	/* Constants */
	m.attr("SQRT_3") = CCCoreLib::SQRT_3;

	define_CCGeom(m);
	define_CCMath(m);
	define_BoundingBox(m);


	define_GenericCloud(m);
	define_GenericIndexedCloud(m);
	define_GenericIndexedCloudPersist(m);
	define_PointCloud(m);

	define_GenericTriangle(m);
	define_GenericMesh(m);
	define_GenericIndexedMesh(m);

	define_ReferenceCloud(m);
	define_Polyline(m);

	define_GenericDistribution(m);
	define_WeibullDistribution(m);

	define_GenericProgressCallback(m);

	define_GenericOctree(m);
	define_DgmOctree(m);
	define_DgmOctreeReferenceCloud(m);

	define_ScalarField(m);
	define_SimpleMesh(m);
	define_SimpleTriangle(m);

	define_CCMiscTools(m);
	define_AutoSegmentationTools(m);
	define_ManualSegmentationTools(m);
	define_ScalarFieldTools(m);
	define_ChamferDistanceTransform(m);
	define_CloudSamplingTools(m);
	define_GeometricalAnalysisTools(m);
	define_StatisticalTestingTools(m);
	define_PointProjectionTools(m);
	define_RegistrationTools(m);

	define_KdTree(m);
	define_TrueKdTree(m);

	define_LocalModel(m);

	m.def("delete", [](CCCoreLib::ReferenceCloud *self) {
		delete self;
	});

	// Conjugate Gradient
	// Delaunay2dMesh
	// DistanceComputationTools
	// Error function
	// FastMarching
	// FastMarchingFor...
	// RayAndBox
	// SaitoSquared...
	// SquareMatrix


	py::bind_vector<CCCoreLib::ReferenceCloudContainer>(m, "ReferenceCloudContainer");
}
