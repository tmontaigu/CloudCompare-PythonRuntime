#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <GenericDistribution.h>
#include <GenericCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;

#include "cccorelib.h"

void define_GenericDistribution(py::module &cccorelib)
{

	py::class_<CCCoreLib::GenericDistribution>
			GenericDistribution(cccorelib, "GenericDistribution");
	GenericDistribution
			.def("getName", &CCCoreLib::GenericDistribution::getName, py::return_value_policy::reference)
			.def("isValid", &CCCoreLib::GenericDistribution::isValid)
			.def("computeParameters", &CCCoreLib::GenericDistribution::computeParameters, "values"_a)
//			.def("computeP", &CCCoreLib::GenericDistribution::computeP, "x"_a)
			.def("computePfromZero", &CCCoreLib::GenericDistribution::computePfromZero, "x"_a)
//			.def("computeP", &CCCoreLib::GenericDistribution::computeP, "x1"_a, "x2"_a)
			.def("computeChi2Dist", &CCCoreLib::GenericDistribution::computeChi2Dist, "GenericDistribution", "Yk"_a,
			     "numberOfClasses"_a, "histo"_a = nullptr);
//			.def(py::init<>());

	py::bind_vector<CCCoreLib::GenericDistribution::ScalarContainer>(GenericDistribution, "ScalarContainer");
}