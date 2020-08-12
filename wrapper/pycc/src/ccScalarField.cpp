#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>

#include <ccScalarField.h>

namespace py = pybind11;
using namespace pybind11::literals;


void define_ccScalarField(py::module &m) {
	py::class_<ccScalarField, CCCoreLib::ScalarField, std::unique_ptr<ccScalarField, py::nodelete>>
		(m, "ccScalarField");
}