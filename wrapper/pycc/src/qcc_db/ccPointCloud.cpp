//##########################################################################
//#                                                                        #
//#                CLOUDCOMPARE PLUGIN: PythonPlugin                       #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                   COPYRIGHT: Thomas Montaigu                           #
//#                                                                        #
//##########################################################################

#include "../casters.h"

#include <ccPointCloud.h>
#include <ccScalarField.h>
#include <vector>
#include <type_traits>


#undef slots
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>


static_assert(std::is_same<PointCoordinateType, float>::value,
    "PointCoordinateType is neither double or float");
static_assert(sizeof(CCVector3) == sizeof(PointCoordinateType) * 3, "Unexpected layout for CCVector3");


namespace py = pybind11;
using namespace pybind11::literals;

#define DEFINE_POINTCLOUDTPL(T, StringType, module, name)                                                    \
    py::class_<CCCoreLib::PointCloudTpl<T, StringType>, T>(module, name)                                     \
        .def("size", &CCCoreLib::PointCloudTpl<T, StringType>::size)                                         \
        .def("forEach", &CCCoreLib::PointCloudTpl<T, StringType>::forEach, "action"_a)                       \
        .def("getBoundingBox",                                                                               \
             &CCCoreLib::PointCloudTpl<T, StringType>::getBoundingBox,                                       \
             "bbMin"_a,                                                                                      \
             "bbMax"_a)                                                                                      \
        .def("getNextPoint",                                                                                 \
             &CCCoreLib::PointCloudTpl<T, StringType>::getNextPoint,                                         \
             py::return_value_policy::reference)                                                             \
        .def("enableScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::enableScalarField)               \
        .def("isScalarFieldEnabled", &CCCoreLib::PointCloudTpl<T, StringType>::isScalarFieldEnabled)         \
        .def("setPointScalarValue",                                                                          \
             &CCCoreLib::PointCloudTpl<T, StringType>::setPointScalarValue,                                  \
             "pointIndex"_a,                                                                                 \
             "value"_a)                                                                                      \
        .def("getPointScalarValue",                                                                          \
             &CCCoreLib::PointCloudTpl<T, StringType>::getPointScalarValue,                                  \
             "pointIndex"_a)                                                                                 \
        .def("resize", &CCCoreLib::PointCloudTpl<T, StringType>::resize, "newCount"_a)                       \
        .def("reserve", &CCCoreLib::PointCloudTpl<T, StringType>::reserve, "newCapacity"_a)                  \
        .def("reset", &CCCoreLib::PointCloudTpl<T, StringType>::reset)                                       \
        .def("invalidateBoundingBox", &CCCoreLib::PointCloudTpl<T, StringType>::invalidateBoundingBox)       \
        .def("getNumberOfScalarFields", &CCCoreLib::PointCloudTpl<T, StringType>::getNumberOfScalarFields)   \
        .def("getScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::getScalarField, "index"_a)          \
        .def("getScalarFieldName", &CCCoreLib::PointCloudTpl<T, StringType>::getScalarFieldName, "index"_a)  \
        .def("getScalarFieldIndexByName",                                                                    \
             &CCCoreLib::PointCloudTpl<T, StringType>::getScalarFieldIndexByName,                            \
             "name"_a)                                                                                       \
        .def("getCurrentInScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::getCurrentInScalarField)   \
        .def("getCurrentOutScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::getCurrentOutScalarField) \
        .def("setCurrentInScalarField",                                                                      \
             &CCCoreLib::PointCloudTpl<T, StringType>::setCurrentInScalarField,                              \
             "index"_a)                                                                                      \
        .def("getCurrentInScalarFieldIndex",                                                                 \
             &CCCoreLib::PointCloudTpl<T, StringType>::getCurrentInScalarFieldIndex)                         \
        .def("setCurrentOutScalarField",                                                                     \
             &CCCoreLib::PointCloudTpl<T, StringType>::setCurrentOutScalarField,                             \
             "index"_a)                                                                                      \
        .def("getCurrentOutScalarFieldIndex",                                                                \
             &CCCoreLib::PointCloudTpl<T, StringType>::getCurrentOutScalarFieldIndex)                        \
        .def("setCurrentScalarField",                                                                        \
             &CCCoreLib::PointCloudTpl<T, StringType>::setCurrentScalarField,                                \
             "index"_a)                                                                                      \
        .def("addScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::addScalarField, "uniqueName"_a)     \
        .def("renameScalarField",                                                                            \
             &CCCoreLib::PointCloudTpl<T, StringType>::renameScalarField,                                    \
             "index"_a,                                                                                      \
             "newName"_a)                                                                                    \
        .def("deleteScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::deleteScalarField, "index"_a)    \
        .def("deleteAllScalarFields", &CCCoreLib::PointCloudTpl<T, StringType>::deleteAllScalarFields)       \
        .def("addPoint", &CCCoreLib::PointCloudTpl<T, StringType>::addPoint, "P"_a);

void define_ccPointCloud(py::module &m)
{
    DEFINE_POINTCLOUDTPL(ccGenericPointCloud, QString, m, "__ccGenericPointCloudTpl");
    py::class_<ccPointCloud,
               CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>,
               std::unique_ptr<ccPointCloud, py::nodelete>>(m, "ccPointCloud")
        .def(py::init<QString, unsigned>(),
             "name"_a = QString(),
             "uniqueID"_a = []() { return ccUniqueIDGenerator::InvalidUniqueID; }(),
             py::return_value_policy::reference)
        // features deletion/clearing
        .def("partialClone", &ccPointCloud::partialClone, "reference"_a, "warnings"_a = nullptr)
        // features allocation/resize
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
        .def("points",
             [](ccPointCloud &self)
             {
                 if (self.size() > 0)
                 {
                     // FIXME, idealy, ccPointCloud would have a .points() method returning a ref
                     // to the std::vector of points, and we would avoit the const cast
                     auto *ptr = const_cast<CCVector3 *>(self.getPoint(0));
                     auto capsule = py::capsule(ptr,
                                                [](void *)
                                                {
                                                });
                     return py::array("3f", self.size(), ptr, capsule);
                 }
                 else
                 {
                     return py::array();
                 }
             })
        .def("__repr__",
             [](const ccPointCloud &self)
             {
                 return std::string("<ccPointCloud(") + "'" + self.getName().toStdString() + "', " +
                        std::to_string(self.size()) + " points)>";
             });
}
