#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <TrueKdTree.h>
#include <GenericProgressCallback.h>

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MAKE_OPAQUE(CCCoreLib::TrueKdTree::LeafVector);

void define_TrueKdTree(py::module& cccorelib) {
	py::class_<CCCoreLib::TrueKdTree> PyTrueKdTree(cccorelib, "TrueKdTree");

	py::bind_vector<CCCoreLib::TrueKdTree::LeafVector>(PyTrueKdTree, "LeafVector");

	PyTrueKdTree.def_readonly_static("X_DIM", &CCCoreLib::TrueKdTree::X_DIM)
			.def_readonly_static("Y_DIM", &CCCoreLib::TrueKdTree::Y_DIM)
			.def_readonly_static("Z_DIM", &CCCoreLib::TrueKdTree::Z_DIM)
			.def_readonly_static("NODE_TYPE", &CCCoreLib::TrueKdTree::NODE_TYPE)
			.def_readonly_static("LEAF_TYPE", &CCCoreLib::TrueKdTree::LEAF_TYPE)
			.def(py::init<CCCoreLib::GenericIndexedCloudPersist *>(), "cloud"_a)
//			.def("associatedCloud", &CCCoreLib::TrueKdTree::associatedCloud, py::return_value_policy::reference)
//			.def("build", &CCCoreLib::TrueKdTree::build, "maxError"_a,
//			     "errorMeasure"_a = CCCoreLib::DistanceComputationTools::RMS,
//			     "minPointCountPerCell"_a = 3,
//			     "maxPointCountPerCell"_a = 0,
//			     "progressCb"_a = nullptr)
			.def("clear", &CCCoreLib::TrueKdTree::clear)
			.def("getMaxErrorType", &CCCoreLib::TrueKdTree::getMaxErrorType)
			.def("getLeaves", &CCCoreLib::TrueKdTree::getLeaves, "leaves"_a);

	py::class_<CCCoreLib::TrueKdTree::BaseNode>(PyTrueKdTree, "BaseNode")
			.def(py::init<uint8_t>(), "nodeType"_a)
			.def("isNode", &CCCoreLib::TrueKdTree::BaseNode::isNode)
			.def("isLeaf", &CCCoreLib::TrueKdTree::BaseNode::isLeaf)
			.def_readwrite("parent", &CCCoreLib::TrueKdTree::BaseNode::parent, py::return_value_policy::reference);

	py::class_<CCCoreLib::TrueKdTree::Node, CCCoreLib::TrueKdTree::BaseNode>(PyTrueKdTree, "Node")
			.def_readwrite("splitValue", &CCCoreLib::TrueKdTree::Node::splitValue, py::return_value_policy::reference)
			.def_readwrite("leftChild", &CCCoreLib::TrueKdTree::Node::leftChild, py::return_value_policy::reference)
			.def_readwrite("rightChild", &CCCoreLib::TrueKdTree::Node::rightChild, py::return_value_policy::reference)
			.def_readwrite("splitDim", &CCCoreLib::TrueKdTree::Node::splitDim)
			.def(py::init<>());

	py::class_<CCCoreLib::TrueKdTree::Leaf>(PyTrueKdTree, "Leaf")
			.def_readwrite("points", &CCCoreLib::TrueKdTree::Leaf::points, py::return_value_policy::reference)
//			.def_readwrite("planeEq",
//			               &CCCoreLib::TrueKdTree::Leaf::planeEq) // FIXME
			.def_readwrite("error", &CCCoreLib::TrueKdTree::Leaf::error)
			.def_readwrite("userData", &CCCoreLib::TrueKdTree::Leaf::userData);
//			.def(py::init<CCCoreLib::ReferenceCloud *, const PointCoordinateType[], ScalarType>(), "set"_a,
//			     "planeEquation"_a, "_error"_a);
}