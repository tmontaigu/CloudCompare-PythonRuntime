#include <pybind11/pybind11.h>

#include <Polyline.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_Polyline(py::module &cccorelib)
{
    py::class_<CCCoreLib::Polyline, CCCoreLib::ReferenceCloud>(cccorelib, "Polyline")
        .def(py::init<CCCoreLib::GenericIndexedCloudPersist *>(), "associatedCloud"_a)
        .def("isClosed", &CCCoreLib::Polyline::isClosed)
        .def("setClosed", &CCCoreLib::Polyline::setClosed, "state"_a);
}