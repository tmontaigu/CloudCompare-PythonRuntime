#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

#include <SimpleMesh.h>
#include <GenericIndexedMesh.h>
#include <GenericIndexedCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;


void define_SimpleMesh(py::module &cccorelib)
{
	py::class_<CCCoreLib::SimpleMesh, CCCoreLib::GenericIndexedMesh>(cccorelib, "SimpleMesh")
			.def(py::init<CCCoreLib::GenericIndexedCloud*, bool>(), "theVertices"_a, "linkVerticesWithMesh"_a = false)
			.def("capacity", &CCCoreLib::SimpleMesh::capacity)
			.def("vertices", &CCCoreLib::SimpleMesh::vertices)
			.def("clear", &CCCoreLib::SimpleMesh::clear)
			.def("addTriangle", &CCCoreLib::SimpleMesh::addTriangle, "i1"_a, "i2"_a, "i3"_a)
			.def("reserve", &CCCoreLib::SimpleMesh::reserve, "n"_a)
			.def("resize", &CCCoreLib::SimpleMesh::resize, "n"_a);
}