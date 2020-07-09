#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>


#include <KdTree.h>
#include <GenericProgressCallback.h>

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MAKE_OPAQUE(std::vector<unsigned>);

void define_KdTree(py::module &cccorelib) {
	py::class_<CCCoreLib::KDTree>
			(cccorelib, "KDTree")
			.def(py::init<>())
			.def("buildFromCloud", &CCCoreLib::KDTree::buildFromCloud, "cloud"_a, "progressCb"_a = nullptr)
			.def("getAssociatedCloud", &CCCoreLib::KDTree::getAssociatedCloud, py::return_value_policy::reference)
			.def("findNearestNeighbour", &CCCoreLib::KDTree::findNearestNeighbour, "queryPoint"_a,
			     "nearestPointIndex"_a, "maxDist"_a)
			.def("findPointBelowDistance", &CCCoreLib::KDTree::findPointBelowDistance, "queryPoint"_a,
			     "maxDist"_a)
			.def("findPointsLyingToDistance", &CCCoreLib::KDTree::findPointsLyingToDistance, "queryPoint"_a,
			     "distance"_a, "tolerance"_a, "points"_a);
}