#include <pybind11/pybind11.h>

#include <CCMath.h>


namespace py = pybind11;
using namespace pybind11::literals;

void define_CCMath(py::module &cccorelib)
{
	cccorelib.def("LessThanEpsilon", [](const double x)
	{ return CCCoreLib::LessThanEpsilon(x); });
	cccorelib.def("GreaterThanEpsilon", [](const double x)
	{ return CCCoreLib::GreaterThanEpsilon(x); });
	cccorelib.def("RadiansToDegrees", [](const double radians)
	{
		return CCCoreLib::RadiansToDegrees(radians);
	});
	cccorelib.def("DegreesToRadians", [](const double degrees)
	{
		return CCCoreLib::DegreesToRadians(degrees);
	});
}