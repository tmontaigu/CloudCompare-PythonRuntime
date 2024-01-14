// ##########################################################################
// #                                                                        #
// #                CLOUDCOMPARE PLUGIN: PythonRuntime                       #
// #                                                                        #
// #  This program is free software; you can redistribute it and/or modify  #
// #  it under the terms of the GNU General Public License as published by  #
// #  the Free Software Foundation; version 2 of the License.               #
// #                                                                        #
// #  This program is distributed in the hope that it will be useful,       #
// #  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
// #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
// #  GNU General Public License for more details.                          #
// #                                                                        #
// #                   COPYRIGHT: Thomas Montaigu                           #
// #                                                                        #
// ##########################################################################

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <GenericProgressCallback.h>
#include <TrueKdTree.h>

#include "cccorelib.h"

namespace py = pybind11;
using namespace pybind11::literals;

using CCCoreLib::TrueKdTree;

void define_TrueKdTree(py::module &cccorelib)
{

    py::class_<TrueKdTree> PyTrueKdTree(cccorelib, "TrueKdTree");
    py::class_<TrueKdTree::BaseNode> PyBaseNode(PyTrueKdTree, "BaseNode");
    py::class_<TrueKdTree::Node, TrueKdTree::BaseNode> PyNode(PyTrueKdTree, "Node");
    py::class_<TrueKdTree::Leaf, TrueKdTree::BaseNode> PyLeaf(PyTrueKdTree, "Leaf");
    py::bind_vector<TrueKdTree::LeafVector>(PyTrueKdTree, "LeafVector");

    PyTrueKdTree
        .def_property_readonly_static("X_DIM",
                                      [](const py::object & /* self */) { return TrueKdTree::X_DIM; })
        .def_property_readonly_static("Y_DIM",
                                      [](const py::object & /* self */) { return TrueKdTree::Y_DIM; })
        .def_property_readonly_static("Z_DIM",
                                      [](const py::object & /* self */) { return TrueKdTree::Z_DIM; })
        .def_property_readonly_static("NODE_TYPE",
                                      [](const py::object & /* self */) { return TrueKdTree::NODE_TYPE; })
        .def_property_readonly_static("LEAF_TYPE",
                                      [](const py::object & /* self */) { return TrueKdTree::LEAF_TYPE; })
        .def(py::init<CCCoreLib::GenericIndexedCloudPersist *>(), "cloud"_a)
        .def("associatedCloud", &TrueKdTree::associatedCloud, py::return_value_policy::reference)
        .def("build",
             &TrueKdTree::build,
             "maxError"_a,
             "errorMeasure"_a = CCCoreLib::DistanceComputationTools::RMS,
             "minPointCountPerCell"_a = 3,
             "maxPointCountPerCell"_a = 0,
             "progressCb"_a = nullptr)
        .def("clear", &TrueKdTree::clear)
        .def("getMaxErrorType", &TrueKdTree::getMaxErrorType)
        .def("getLeaves", &TrueKdTree::getLeaves, "leaves"_a);

    PyBaseNode.def(py::init<uint8_t>(), "nodeType"_a)
        .def("isNode", &TrueKdTree::BaseNode::isNode)
        .def("isLeaf", &TrueKdTree::BaseNode::isLeaf)
        .def_readwrite("parent", &TrueKdTree::BaseNode::parent, py::return_value_policy::reference);

    PyNode.def_readwrite("splitValue", &TrueKdTree::Node::splitValue, py::return_value_policy::reference)
        .def_readwrite("leftChild", &TrueKdTree::Node::leftChild, py::return_value_policy::reference)
        .def_readwrite("rightChild", &TrueKdTree::Node::rightChild, py::return_value_policy::reference)
        .def_readwrite("splitDim", &TrueKdTree::Node::splitDim)
        .def(py::init<>());

    PyLeaf
        .def_readwrite("points", &TrueKdTree::Leaf::points, py::return_value_policy::reference)
        //        .def_readwrite("planeEq",
        //                       &TrueKdTree::Leaf::planeEq) // FIXME
        .def_readwrite("error", &TrueKdTree::Leaf::error)
        .def_readwrite("userData", &TrueKdTree::Leaf::userData)
        .def(py::init(
                 [](CCCoreLib::ReferenceCloud *set, const py::sequence &planeEquation, ScalarType error)
                 {
                     PointCoordinateType planeEq[4] = {0};
                     planeEq[0] = planeEquation[0].cast<PointCoordinateType>();
                     planeEq[1] = planeEquation[1].cast<PointCoordinateType>();
                     planeEq[2] = planeEquation[2].cast<PointCoordinateType>();
                     planeEq[3] = planeEquation[3].cast<PointCoordinateType>();

                     return TrueKdTree::Leaf(set, nullptr, error);
                 }),
             "set"_a,
             "planeEquation"_a,
             "_error"_a);
}
