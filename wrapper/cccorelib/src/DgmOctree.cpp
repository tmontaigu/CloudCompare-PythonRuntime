#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>

#include <DgmOctree.h>
#include <GenericProgressCallback.h>

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MAKE_OPAQUE(CCCoreLib::DgmOctree::NeighbourCellsSet);
PYBIND11_MAKE_OPAQUE(CCCoreLib::DgmOctree::NeighboursSet);


void define_DgmOctree(py::module &cccorelib)
{
	py::class_<CCCoreLib::DgmOctree, CCCoreLib::GenericOctree> PyDgmOctree(cccorelib, "DgmOctree");
	PyDgmOctree.def_static("GET_BIT_SHIFT", &CCCoreLib::DgmOctree::GET_BIT_SHIFT, "level"_a)
			.def_static("OCTREE_LENGTH", &CCCoreLib::DgmOctree::OCTREE_LENGTH, "level"_a)
			.def_readonly_static("MAX_OCTREE_LEVEL", &CCCoreLib::DgmOctree::MAX_OCTREE_LEVEL)
			.def_readonly_static("MAX_OCTREE_LENGTH", &CCCoreLib::DgmOctree::MAX_OCTREE_LENGTH)
			.def_readonly_static("INVALID_CELL_CODE", &CCCoreLib::DgmOctree::INVALID_CELL_CODE)
			.def("clear", &CCCoreLib::DgmOctree::clear)
			.def("build", [](CCCoreLib::DgmOctree &self, CCCoreLib::GenericProgressCallback *progressCb = nullptr)
			{ return self.build(); }, "progressCb"_a = nullptr)
			.def("build", [](CCCoreLib::DgmOctree &self,
			                 const CCVector3 &octreeMin,
			                 const CCVector3 &octreeMax,
			                 const CCVector3 *pointsMinFilter = nullptr,
			                 const CCVector3 *pointsMaxFilter = nullptr,
			                 CCCoreLib::GenericProgressCallback *progressCb = nullptr)
			     {
				     return self.build(octreeMax, octreeMax,
				                       pointsMinFilter,
				                       pointsMaxFilter, progressCb);
			     }, "octreeMin"_a, "octreeMax"_a, "pointsMinFilter"_a = nullptr, "pointsMaxFilter"_a = nullptr,
			     "progressCb"_a = nullptr)
			.def("getNumberOfProjectedPoints", &CCCoreLib::DgmOctree::getNumberOfProjectedPoints)
			.def("getOctreeMins", &CCCoreLib::DgmOctree::getOctreeMins)
			.def("getOctreeMaxs", &CCCoreLib::DgmOctree::getOctreeMaxs)
			.def("getBoundingBox", &CCCoreLib::DgmOctree::getBoundingBox, "bbMin"_a, "bbMax"_a)
			.def("getMinFillIndexes", &CCCoreLib::DgmOctree::getMinFillIndexes, "level"_a,
			     py::return_value_policy::reference)
			.def("getMaxFillIndexes", &CCCoreLib::DgmOctree::getMaxFillIndexes, "level"_a,
			     py::return_value_policy::reference)
			.def("getCellSize", &CCCoreLib::DgmOctree::getCellSize, "level"_a)
//			.def("getCellDistanceFromBorders", )
//			.def("getCellDistanceFromBorders", )
			;

	py::bind_vector<CCCoreLib::DgmOctree::NeighbourCellsSet>(PyDgmOctree, "NeighbourCellsSet");
	py::bind_vector<CCCoreLib::DgmOctree::NeighboursSet>(PyDgmOctree, "NeighboursSet");

	py::class_<CCCoreLib::DgmOctree::PointDescriptor>(PyDgmOctree, "PointDescriptor")
			.def_readwrite("point", &CCCoreLib::DgmOctree::PointDescriptor::point)
			.def_readwrite("pointIndex", &CCCoreLib::DgmOctree::PointDescriptor::pointIndex)
			.def_readwrite("squareDistd", &CCCoreLib::DgmOctree::PointDescriptor::squareDistd)
			.def(py::init<>())
			.def(py::init<const CCVector3 *, unsigned>())
			.def(py::init<const CCVector3 *, unsigned, double>())
			.def_static("distComp", &CCCoreLib::DgmOctree::PointDescriptor::distComp);

	py::class_<CCCoreLib::DgmOctree::CellDescriptor>(PyDgmOctree, "CellDescriptor")
			.def_readwrite("center", &CCCoreLib::DgmOctree::CellDescriptor::center)
			.def_readwrite("index", &CCCoreLib::DgmOctree::CellDescriptor::index)
			.def(py::init<>())
			.def(py::init<const CCVector3 &, unsigned>());

	py::class_<CCCoreLib::DgmOctree::NearestNeighboursSearchStruct>(PyDgmOctree, "NearestNeighboursSearchStruct")
			.def_readwrite("queryPoint", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::queryPoint)
			.def_readwrite("level", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::level)
			.def_readwrite("minNumberOfNeighbors",
			               &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::minNumberOfNeighbors)
			.def_readwrite("cellPos",
			               &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::cellPos)
			.def_readwrite("cellCenter", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::cellCenter)
			.def_readwrite("maxSearchSquareDistd",
			               &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::maxSearchSquareDistd)
			.def_readwrite("minimalCellsSetToVisit",
			               &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::minimalCellsSetToVisit)
			.def_readwrite("pointsInNeighbourhood",
			               &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::pointsInNeighbourhood)
			.def_readwrite("alreadyVisitedNeighbourhoodSize",
			               &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::alreadyVisitedNeighbourhoodSize)
			.def_readwrite("theNearestPointIndex",
			               &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::theNearestPointIndex)
			.def(py::init<>());
}
