#include <pybind11/pybind11.h>

#include <ErrorFunction.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ErrorFunction(py::module &cccorelib)
{
    cccorelib.attr("c_erfRelativeError") = CCCoreLib::c_erfRelativeError;
    py::class_<CCCoreLib::ErrorFunction>(cccorelib, "ErrorFunction")
        .def_static("erfc", &CCCoreLib::ErrorFunction::erfc, "x"_a)
        .def_static("erf", &CCCoreLib::ErrorFunction::erf, "x"_a);
}