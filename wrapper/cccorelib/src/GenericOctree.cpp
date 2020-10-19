#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

#include <GenericOctree.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_GenericOctree(py::module &cccorelib)
{
    py::class_<CCCoreLib::GenericOctree>(cccorelib, "GenericOctree");
}