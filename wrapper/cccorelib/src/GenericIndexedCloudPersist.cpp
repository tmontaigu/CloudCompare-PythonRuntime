#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

#include <GenericIndexedCloudPersist.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_GenericIndexedCloudPersist(py::module &cccorelib)
{
    py::class_<CCCoreLib::GenericIndexedCloudPersist, CCCoreLib::GenericIndexedCloud>(
        cccorelib, "GenericIndexedCloudPersist")
        .def("getPointPersistentPtr",
             &CCCoreLib::GenericIndexedCloudPersist::getPointPersistentPtr,
             py::return_value_policy::reference);
}