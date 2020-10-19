#include <pybind11/pybind11.h>

#include <ChamferDistanceTransform.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ChamferDistanceTransform(py::module &cccorelib)
{

    using GRID3Dus = CCCoreLib::Grid3D<unsigned short>;
    py::class_<GRID3Dus> PyGrid3Dus(cccorelib, "Grid3Dus");
    PyGrid3Dus.def(py::init<>());
    PyGrid3Dus.def("size", &GRID3Dus::size);
    PyGrid3Dus.def("isInitialized", &GRID3Dus::isInitialized);
    PyGrid3Dus.def("init", &GRID3Dus::init);

    py::class_<CCCoreLib::ChamferDistanceTransform, GRID3Dus>(cccorelib, "ChamferDistanceTransform")
        .def_readonly_static("MAX_DIST", &CCCoreLib::ChamferDistanceTransform::MAX_DIST)
        .def("init", &CCCoreLib::ChamferDistanceTransform::init)
        .def("propagateDistance", &CCCoreLib::ChamferDistanceTransform::propagateDistance);
}