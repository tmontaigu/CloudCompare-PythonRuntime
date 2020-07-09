#include <pybind11/pybind11.h>


#include <LocalModel.h>

namespace py = pybind11;
using namespace pybind11::literals;


void define_LocalModel(py::module &cccorelib) {
	py::class_<CCCoreLib::LocalModel> (cccorelib, "LocalModel")
	        //TODO
//			.def_static("New", &CCCoreLib::LocalModel::New, "type"_a, "subset"_a, "center"_a, "squaredRadius"_a)
			.def("getType", &CCCoreLib::LocalModel::getType)
			.def("getCenter", &CCCoreLib::LocalModel::getCenter)
			.def("getSquareSize", &CCCoreLib::LocalModel::getSquareSize)
			.def("computeDistanceFromModelToPoint", &CCCoreLib::LocalModel::computeDistanceFromModelToPoint, "P"_a, "nearestPoint"_a = nullptr);
}