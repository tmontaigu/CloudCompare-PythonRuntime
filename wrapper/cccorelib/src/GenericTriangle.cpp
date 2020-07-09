#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <GenericTriangle.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_GenericTriangle(py::module& cccorelib) {
	py::class_<CCCoreLib::GenericTriangle>(cccorelib, "GenericTriangle")
	        .def("_getA", &CCCoreLib::GenericTriangle::_getA, py::return_value_policy::reference)
			.def("_getB", &CCCoreLib::GenericTriangle::_getB, py::return_value_policy::reference)
			.def("_getC", &CCCoreLib::GenericTriangle::_getC, py::return_value_policy::reference);
}