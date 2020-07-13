#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>

#include <KdTree.h>
#include <GenericProgressCallback.h>

namespace py = pybind11;
using namespace pybind11::literals;

#include "cccorelib.h"


void define_KdTree(py::module &cccorelib) {


	py::class_<CCCoreLib::KDTree>
			a(cccorelib, "KDTree");
	py::bind_vector<std::vector<unsigned>>(a, "IndicesVector");


	a.def(py::init<>())
			.def("buildFromCloud", &CCCoreLib::KDTree::buildFromCloud, "cloud"_a, "progressCb"_a = nullptr)
			.def("getAssociatedCloud", &CCCoreLib::KDTree::getAssociatedCloud, py::return_value_policy::reference)
			.def("findNearestNeighbour", [](CCCoreLib::KDTree &self, py::sequence &queryPoint, ScalarType maxDist) {
				PointCoordinateType point[3];
				point[0] = queryPoint[0].cast<PointCoordinateType>();
				point[1] = queryPoint[1].cast<PointCoordinateType>();
				point[2] = queryPoint[2].cast<PointCoordinateType>();

				unsigned nearestIndex{0};
				py::object ret = py::none();
				if ( self.findNearestNeighbour(point, nearestIndex, maxDist))
				{
					ret = py::cast(nearestIndex);
				}
				return ret;
			}, "queryPoint"_a, "maxDist"_a)
			.def("findPointBelowDistance", [](CCCoreLib::KDTree &self, py::sequence &queryPoint, ScalarType maxDist) {
				PointCoordinateType point[3];
				point[0] = queryPoint[0].cast<PointCoordinateType>();
				point[1] = queryPoint[1].cast<PointCoordinateType>();
				point[2] = queryPoint[2].cast<PointCoordinateType>();
				return self.findPointBelowDistance(point, maxDist);

			}, "queryPoint"_a, "maxDist"_a)
			.def("findPointsLyingToDistance",
			     [](CCCoreLib::KDTree &self, py::sequence &queryPoint, ScalarType distance, ScalarType tolerance,
			        std::vector<unsigned> &points) {
				     PointCoordinateType point[3];
				     point[0] = queryPoint[0].cast<PointCoordinateType>();
				     point[1] = queryPoint[1].cast<PointCoordinateType>();
				     point[2] = queryPoint[2].cast<PointCoordinateType>();
				     return self.findPointsLyingToDistance(point, distance, tolerance, points);
			     }, "queryPoint"_a, "distance"_a, "tolerance"_a, "points"_a);
}