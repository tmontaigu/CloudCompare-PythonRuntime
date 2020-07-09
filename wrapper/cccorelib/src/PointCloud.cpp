#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

#include <PointCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;


void define_PointCloud(py::module &cccorelib) {
	py::class_<CCCoreLib::PointCloud, CCCoreLib::GenericIndexedCloudPersist>(cccorelib, "PointCloud");
}