#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include <ccPointCloud.h>
#include <ccScalarField.h>
#include "casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

#define DEFINE_POINTCLOUDTPL(T, StringType, module, name) py::class_<CCCoreLib::PointCloudTpl<T, StringType>, T>(module, name) \
    .def("size", &CCCoreLib::PointCloudTpl<T, StringType>::size)                                                               \
    .def("forEach", &CCCoreLib::PointCloudTpl<T, StringType>::forEach, "action"_a)                                             \
    .def("getBoundingBox", &CCCoreLib::PointCloudTpl<T, StringType>::getBoundingBox, "bbMin"_a, "bbMax"_a)                     \
    .def("getNextPoint", &CCCoreLib::PointCloudTpl<T, StringType>::getNextPoint)                                               \
    .def("enableScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::enableScalarField)                                     \
    .def("isScalarFieldEnabled", &CCCoreLib::PointCloudTpl<T, StringType>::isScalarFieldEnabled)                               \
    .def("setPointScalarValue", &CCCoreLib::PointCloudTpl<T, StringType>::setPointScalarValue, "pointIndex"_a, "value"_a)      \
    .def("getPointScalarValue", &CCCoreLib::PointCloudTpl<T, StringType>::getPointScalarValue, "pointIndex"_a)                 \
    .def("resize", &CCCoreLib::PointCloudTpl<T, StringType>::resize, "newCount"_a)                                             \
    .def("reserve", &CCCoreLib::PointCloudTpl<T, StringType>::reserve, "newCapacity"_a)                                        \
    .def("reset", &CCCoreLib::PointCloudTpl<T, StringType>::reset)                                                             \
    .def("invalidateBoundingBox", &CCCoreLib::PointCloudTpl<T, StringType>::invalidateBoundingBox)                             \
    .def("getNumberOfScalarFields", &CCCoreLib::PointCloudTpl<T, StringType>::getNumberOfScalarFields)                         \
    .def("getScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::getScalarField, "index"_a)                                \
    .def("getScalarFieldName", &CCCoreLib::PointCloudTpl<T, StringType>::getScalarFieldName, "index"_a)                        \
    .def("getScalarFieldIndexByName", &CCCoreLib::PointCloudTpl<T, StringType>::getScalarFieldIndexByName, "name"_a)           \
    .def("getCurrentInScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::getCurrentInScalarField)                         \
    .def("getCurrentOutScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::getCurrentOutScalarField)                       \
    .def("setCurrentInScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::setCurrentInScalarField, "index"_a)              \
    .def("getCurrentInScalarFieldIndex", &CCCoreLib::PointCloudTpl<T, StringType>::getCurrentInScalarFieldIndex)               \
    .def("setCurrentOutScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::setCurrentOutScalarField, "index"_a)            \
    .def("getCurrentOutScalarFieldIndex", &CCCoreLib::PointCloudTpl<T, StringType>::getCurrentOutScalarFieldIndex)             \
    .def("addScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::addScalarField, "uniqueName"_a)                           \
    .def("renameScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::renameScalarField, "index"_a, "newName"_a)             \
    .def("deleteScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::deleteScalarField, "index"_a)                          \
    .def("deleteAllScalarFields", &CCCoreLib::PointCloudTpl<T, StringType>::deleteAllScalarFields)                             \
    .def("addPoint", &CCCoreLib::PointCloudTpl<T, StringType>::addPoint, "P"_a);


void define_ccPointCloud(py::module &m) {
	DEFINE_POINTCLOUDTPL(ccGenericPointCloud, QString, m, "__ccGenericPointCloudTpl");

	py::class_<
			ccPointCloud,
			CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>,
			std::unique_ptr<ccPointCloud, py::nodelete>
	>(m, "ccPointCloud")
			.def(py::init<QString, unsigned>(), "name"_a = QString(),
			     "uniqueID"_a = ccUniqueIDGenerator::InvalidUniqueID)
					//features deletion/clearing
					//features allocation/resize
			.def("reserveThePointsTable", &ccPointCloud::reserveThePointsTable, "_numberOfPoints"_a)
			.def("reserveTheRGBTable", &ccPointCloud::reserveThePointsTable)
			.def("reserveTheRGBTable", &ccPointCloud::reserveThePointsTable, "fillWithWhite"_a = false)
			.def("reserveTheNormsTable", &ccPointCloud::reserveTheNormsTable)
			.def("resizeTheNormsTable", &ccPointCloud::resizeTheNormsTable)
			.def("shrinkToFit", &ccPointCloud::shrinkToFit)
					// ScalarField management
			.def("getCurrentDisplayedScalarField", &ccPointCloud::getCurrentDisplayedScalarField)
			.def("getCurrentDisplayedScalarFieldIndex", &ccPointCloud::getCurrentDisplayedScalarFieldIndex)
			.def("setCurrentDisplayedScalarField", &ccPointCloud::setCurrentDisplayedScalarField, "index"_a)
			.def("sfColorScaleShown", &ccPointCloud::sfColorScaleShown)
			.def("showSFColorsScale", &ccPointCloud::showSFColorsScale, "state"_a)

			.def("__repr__", [](const ccPointCloud &self) {
				return std::string("<ccPointCloud(") + "'" + self.getName().toStdString() + "', " +
				       std::to_string(self.size()) + " points)>";
			});
}