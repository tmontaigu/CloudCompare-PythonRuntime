#include <pybind11/functional.h>
#include <pybind11/pybind11.h>

#include <GenericCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_GenericCloud(py::module &cccorelib)
{
    py::class_<CCCoreLib::GenericCloud>(cccorelib, "GenericCloud")
        .def("size", &CCCoreLib::GenericCloud::size)
        .def("forEach", &CCCoreLib::GenericCloud::forEach, "action"_a)
        .def("getBoundingBox", &CCCoreLib::GenericCloud::getBoundingBox)
        .def("testVisibility", &CCCoreLib::GenericCloud::testVisibility)
        .def("placeIteratorAtBeginning", &CCCoreLib::GenericCloud::placeIteratorAtBeginning)
        .def("getNextPoint", &CCCoreLib::GenericCloud::getNextPoint, py::return_value_policy::reference)
        .def("enableScalarField", &CCCoreLib::GenericCloud::enableScalarField)
        .def("isScalarFieldEnabled", &CCCoreLib::GenericCloud::enableScalarField)
        .def("setPointScalarValue", &CCCoreLib::GenericCloud::getPointScalarValue)
        .def("getPointScalarValue", &CCCoreLib::GenericCloud::getPointScalarValue);
}