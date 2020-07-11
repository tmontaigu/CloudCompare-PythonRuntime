#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

#include "wrappers.h"

#include <ScalarField.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ScalarField(py::module &cccorelib)
{
	py::class_<CCCoreLib::ScalarField, observer_ptr<CCCoreLib::ScalarField>>
			(cccorelib, "ScalarField")
			.def(py::init<const char *>(), "name"_a)
			.def("setName", &CCCoreLib::ScalarField::setName)
			.def("getName", &CCCoreLib::ScalarField::getName)
			.def_static("NaN", &CCCoreLib::ScalarField::NaN)
			.def("computeMeanAndVariance", &CCCoreLib::ScalarField::computeMeanAndVariance, "mean"_a,
			     "variance"_a = nullptr)
			.def("computeMinAndMax", &CCCoreLib::ScalarField::computeMinAndMax)
			.def_static("ValidValue", &CCCoreLib::ScalarField::ValidValue, "value"_a)
			.def("flagValueAsInvalid", &CCCoreLib::ScalarField::flagValueAsInvalid, "index"_a)
			.def("getMin", &CCCoreLib::ScalarField::getMin)
			.def("getMax", &CCCoreLib::ScalarField::getMax)
			.def("fill", &CCCoreLib::ScalarField::fill, "fillValue"_a = 0)
			.def("reserveSafe", &CCCoreLib::ScalarField::reserveSafe, "count"_a)
			.def("resizeSafe", &CCCoreLib::ScalarField::resizeSafe, "count"_a, "initNewElements"_a = false,
			     "valueForNewElements"_a = 0)
			.def("asArray", [](CCCoreLib::ScalarField &self)
			{
				return PyCC::VectorAsNumpyArray(self);
			})
			.def("__repr__", [](const CCCoreLib::ScalarField &self)
			{
				return std::string("<ScalarField(name=") + self.getName() + ")>";
			});

}