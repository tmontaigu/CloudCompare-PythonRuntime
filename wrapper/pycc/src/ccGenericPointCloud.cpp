#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>

#include <ccGenericPointCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;


void define_ccGenericPointCloud(py::module &m) {
	py::class_<ccGenericPointCloud, ccShiftedObject, CCCoreLib::GenericIndexedCloudPersist>
	        (m, "ccGenericPointCloud");
}