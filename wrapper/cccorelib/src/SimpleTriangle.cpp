#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

#include <GenericTriangle.h>
#include <SimpleTriangle.h>

namespace py = pybind11;
using namespace pybind11::literals;


void define_SimpleTriangle(py::module &cccorelib)
{
	py::class_<CCCoreLib::SimpleRefTriangle, CCCoreLib::GenericTriangle>(cccorelib, "SimpleRefTriangle")
			.def(py::init<>())
			.def(py::init<
					const CCVector3 *,
					const CCVector3 *,
					const CCVector3 *>(), "_A"_a, "_B"_a, "_C"_a);

	py::class_<CCCoreLib::SimpleTriangle, CCCoreLib::GenericTriangle>(cccorelib, "SimpleTriangle")
	        .def(py::init<>())
	        .def(py::init<const CCVector3&, const CCVector3&, const CCVector3&>());
}