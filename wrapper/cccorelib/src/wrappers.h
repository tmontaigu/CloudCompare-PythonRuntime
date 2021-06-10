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

#ifndef CLOUDCOMPAREPROJECTS_WRAPPERS_H
#define CLOUDCOMPAREPROJECTS_WRAPPERS_H

#include <CCTypes.h>
#include <PointCloudTpl.h>

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace pybind11::literals;

template <class T> using observer_ptr = std::unique_ptr<T, py::nodelete>;

namespace PyCC
{
inline void NoOpDelete(void *) {}

template <class T> py::array_t<T> VectorAsNumpyArray(std::vector<T> &vector)
{
    // https://stackoverflow.com/questions/44659924/returning-numpy-arrays-via-pybind11
    // https://github.com/pybind/pybind11/issues/1042
    auto capsule = py::capsule(vector.data(), NoOpDelete);
    return py::array(vector.size(), vector.data(), capsule);
}

template <class PointCloudType>
void addPointsFromArrays(PointCloudType &self,
                         py::array_t<PointCoordinateType> &xs,
                         py::array_t<PointCoordinateType> &ys,
                         py::array_t<PointCoordinateType> &zs)
{
    if (xs.size() != ys.size() || xs.size() != zs.size())
    {
        throw py::value_error("xs, ys, zs must have the same size");
    }

    self.reserve(self.size() + xs.size());

    auto xs_it = xs.begin();
    auto ys_it = ys.begin();
    auto zs_it = zs.begin();

    for (; xs_it != xs.end();)
    {
        self.addPoint({xs_it->cast<PointCoordinateType>(),
                       ys_it->cast<PointCoordinateType>(),
                       zs_it->cast<PointCoordinateType>()});
        ++xs_it;
        ++ys_it;
        ++zs_it;
    }
}
} // namespace PyCC

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
        .def("renameScalarField",                                                                            \
             &CCCoreLib::PointCloudTpl<T, StringType>::renameScalarField,                                    \
             "index"_a,                                                                                      \
             "newName"_a)                                                                                    \
        .def(                                                                                                \
            "addScalarField",                                                                                \
            [](CCCoreLib::PointCloudTpl<T, StringType> &self,                                                \
               const char *sfName,                                                                           \
               const py::object &maybe_values = py::none()) {                                                \
                int idx = self.addScalarField(sfName);                                                       \
                if (idx == -1)                                                                               \
                {                                                                                            \
                    throw std::runtime_error("Failed to add scalar field");                                  \
                }                                                                                            \
                if (!maybe_values.is_none())                                                                 \
                {                                                                                            \
                    py::array_t<ScalarType> values(maybe_values);                                            \
                    if (values.size() != self.size())                                                        \
                    {                                                                                        \
                        throw py::value_error("value must have the same len as the poinc cloud size");       \
                    }                                                                                        \
                    auto values_u = values.unchecked<1>();                                                   \
                    CCCoreLib::ScalarField *sf = self.getScalarField(idx);                                   \
                    for (ssize_t i{0}; i < values.size(); ++i)                                               \
                    {                                                                                        \
                        (*sf)[i] = values_u(i);                                                              \
                    }                                                                                        \
                }                                                                                            \
                return idx;                                                                                  \
            },                                                                                               \
            "name"_a,                                                                                        \
            "values"_a = py::none())                                                                         \
        .def("deleteScalarField", &CCCoreLib::PointCloudTpl<T, StringType>::deleteScalarField, "index"_a)    \
        .def("deleteAllScalarFields", &CCCoreLib::PointCloudTpl<T, StringType>::deleteAllScalarFields)       \
        .def("addPoint", &CCCoreLib::PointCloudTpl<T, StringType>::addPoint, "P"_a)                          \
        .def("addPoints", &PyCC::addPointsFromArrays<CCCoreLib::PointCloudTpl<T, StringType>>)               \
        .def("__len__", &CCCoreLib::PointCloudTpl<T, StringType>::size);

#endif // CLOUDCOMPAREPROJECTS_WRAPPERS_H
