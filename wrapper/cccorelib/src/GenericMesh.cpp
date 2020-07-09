#include <pybind11/pybind11.h>

#include <GenericMesh.h>
#include <GenericTriangle.h>

namespace py = pybind11;
using namespace pybind11::literals;


void define_GenericMesh(py::module &cccorelib)
{
	py::class_<CCCoreLib::GenericMesh>(cccorelib, "GenericMesh")
			.def("size", &CCCoreLib::GenericMesh::size)
			.def("getBoundingBox", &CCCoreLib::GenericMesh::getBoundingBox, "bbMin"_a, "bbMax"_a)
			.def("placeIteratorAtBeginning", &CCCoreLib::GenericMesh::placeIteratorAtBeginning)
			.def("_getNextTriangle", &CCCoreLib::GenericMesh::_getNextTriangle);
}