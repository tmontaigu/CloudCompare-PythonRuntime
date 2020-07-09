#include <pybind11/pybind11.h>

#include <GenericProgressCallback.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_GenericProgressCallback(py::module &cccorelib)
{
	py::class_<CCCoreLib::GenericProgressCallback>(cccorelib, "GenericProgressCallback")
			.def("update", &CCCoreLib::GenericProgressCallback::update)
			.def("setMethodTitle", &CCCoreLib::GenericProgressCallback::setMethodTitle)
			.def("setInfo", &CCCoreLib::GenericProgressCallback::setInfo)
			.def("start", &CCCoreLib::GenericProgressCallback::start)
			.def("stop", &CCCoreLib::GenericProgressCallback::stop)
			.def("isCancelRequested", &CCCoreLib::GenericProgressCallback::isCancelRequested)
			.def("textCanBeEdited", &CCCoreLib::GenericProgressCallback::textCanBeEdited);

	py::class_<CCCoreLib::NormalizedProgress>(cccorelib, "NormalizedProgress")
			//.def(py::init<CCCoreLib::GenericProgressCallback, unsigned, unsigned>(), "callback"_a, "totalSteps"_a, "totalPercentage"_a=100)
			.def("scale", &CCCoreLib::NormalizedProgress::scale, "totalSteps"_a, "totalPercentage"_a = 100,
			     "updateCurrentProgress"_a = false)
			.def("reset", &CCCoreLib::NormalizedProgress::reset)
			.def("oneStep", &CCCoreLib::NormalizedProgress::oneStep)
			.def("steps", &CCCoreLib::NormalizedProgress::steps);
}