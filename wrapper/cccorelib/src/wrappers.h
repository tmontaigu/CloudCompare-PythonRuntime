// ##########################################################################
// #                                                                        #
// #                CLOUDCOMPARE PLUGIN: PythonRuntime                       #
// #                                                                        #
// #  This program is free software; you can redistribute it and/or modify  #
// #  it under the terms of the GNU General Public License as published by  #
// #  the Free Software Foundation; version 2 of the License.               #
// #                                                                        #
// #  This program is distributed in the hope that it will be useful,       #
// #  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
// #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
// #  GNU General Public License for more details.                          #
// #                                                                        #
// #                   COPYRIGHT: Thomas Montaigu                           #
// #                                                                        #
// ##########################################################################

#ifndef PYTHON_PLUGIN_WRAPPERS_H
#define PYTHON_PLUGIN_WRAPPERS_H

#include <CCShareable.h>
#include <CCTypes.h>
#include <PointCloudTpl.h>

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <stdexcept>
#include <type_traits>

namespace py = pybind11;
using namespace pybind11::literals;

/// A unique_ptr that never free its ptr.
template <class T> using observer_ptr = std::unique_ptr<T, py::nodelete>;

/// A holder type for any type that inherits CCShareable
///
/// CCShareable is CC's ref counted mechanism.
/// It works by inheritance + manual call to link/release.
template <class T> class CCShareableHolder
{
  public:
    static_assert(std::is_base_of<CCShareable, T>::value == true, "T must be a subclass of CCShareable");

    CCShareableHolder() = default;

    explicit CCShareableHolder(T *obj) : m_ptr(obj)
    {
        if (m_ptr)
        {
            m_ptr->link();
        }
    }

    T *get()
    {
        return m_ptr;
    }

    const T *get() const
    {
        return m_ptr;
    }

    ~CCShareableHolder()
    {
        if (m_ptr)
        {
            m_ptr->release();
            m_ptr = nullptr;
        }
    }

  private:
    T *m_ptr = nullptr;
};

PYBIND11_DECLARE_HOLDER_TYPE(T, CCShareableHolder<T>);

namespace PyCC
{
inline void NoOpDelete(void *) {}

template <class T> py::array_t<T> SpanAsNumpyArray(T *data, py::array::ShapeContainer shape)
{
    auto capsule = py::capsule(data, NoOpDelete);
    return py::array(shape, data, capsule);
}

template <class T> py::array_t<T> SpanAsNumpyArray(T *data, size_t len)
{
    auto capsule = py::capsule(data, NoOpDelete);
    return py::array(len, data, capsule);
}

template <class T> py::array_t<T> VectorAsNumpyArray(std::vector<T> &vector)
{
    // https://stackoverflow.com/questions/44659924/returning-numpy-arrays-via-pybind11
    // https://github.com/pybind/pybind11/issues/1042
    return SpanAsNumpyArray(vector.data(), vector.size());
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

    py::ssize_t numToReserve = self.size() + xs.size();
    if (numToReserve > std::numeric_limits<unsigned int>::max())
    {
        throw std::out_of_range(std::to_string(numToReserve) + " cannot be casted to unsigned int");
    }
    self.reserve(static_cast<unsigned int>(numToReserve));

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

static const constexpr char ADD_SCALAR_FIELD_DOCSTRING[] = R"doc(
     Adds a scalar field with the given name to the point cloud.

    Parameters
    ----------
    name: str
        name of the scalar field that will be added.
    values: optional, numpy.array, list of float
        values to use when initializing the new scalar field

    Raises
    ------
    RuntimeError if the point cloud already has a scalar field with the given ``name``
    ValueError if values are provided don't have the same length (size) as the point cloud

)doc";

static const constexpr char SIZE_SCALAR_FIELD_DOCSTRING[] = R"doc(
    Returns the size (number of points) in the point cloud.

    ``len`` also works as an alias to size.

    .. code:: Python

        pc = pycc.ccPointCloud("name")
        assert len(pc) == pc.size()
)doc";

#define DEFINE_POINTCLOUDTPL(T, StringType, module, name)                                                    \
    py::class_<CCCoreLib::PointCloudTpl<T, StringType>, T>(module, name)                                     \
        .def("size", &CCCoreLib::PointCloudTpl<T, StringType>::size, SIZE_SCALAR_FIELD_DOCSTRING)            \
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
        .def("getNumberOfScalarFields",                                                                      \
             &CCCoreLib::PointCloudTpl<T, StringType>::getNumberOfScalarFields,                              \
             "Returns the number of scalar field of the point cloud")                                        \
        .def(                                                                                                \
            "getScalarField",                                                                                \
            &CCCoreLib::PointCloudTpl<T, StringType>::getScalarField,                                        \
            "index"_a,                                                                                       \
            R"doc(                                                                                           \
    Returns the scalar field identified by its index.                                                        \
                                                                                                             \
    If index is invalid, None is returned                                                                    \
)doc")                                                                                                       \
        .def(                                                                                                \
            "getScalarFieldName",                                                                            \
            &CCCoreLib::PointCloudTpl<T, StringType>::getScalarFieldName,                                    \
            "index"_a,                                                                                       \
            R"doc(                                                                                           \
    Returns the name of the scalar field identified by the index.                                            \
                                                                                                             \
    If index is invalid, -1 is returned                                                                      \
)doc")                                                                                                       \
        .def(                                                                                                \
            "getScalarFieldIndexByName",                                                                     \
            &CCCoreLib::PointCloudTpl<T, StringType>::getScalarFieldIndexByName,                             \
            "name"_a,                                                                                        \
            R"doc(                                                                                           \
     Returns the scalar field identified by its name.                                                        \
                                                                                                             \
    If no scalar field has the given name, None is returned                                                  \
)doc")                                                                                                       \
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
               const py::object &maybe_values = py::none())                                                  \
            {                                                                                                \
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
                    for (py::ssize_t i{0}; i < values.size(); ++i)                                           \
                    {                                                                                        \
                        (*sf)[i] = values_u(i);                                                              \
                    }                                                                                        \
                }                                                                                            \
                return idx;                                                                                  \
            },                                                                                               \
            "name"_a,                                                                                        \
            "values"_a = py::none(),                                                                         \
            ADD_SCALAR_FIELD_DOCSTRING)                                                                      \
        .def(                                                                                                \
            "deleteScalarField",                                                                             \
            &CCCoreLib::PointCloudTpl<T, StringType>::deleteScalarField,                                     \
            "index"_a,                                                                                       \
            R"doc(                                                                                           \
     Removes the scalar field identified by the index          .                                             \
                                                                                                             \
    .. warning::                                                                                             \
        This operation may modify the scalar fields order                                                    \
        (especially if the deleted SF is not the last one).                                                  \
        However current IN & OUT scalar fields will stay up-to-date                                          \
        (while their index may change).                                                                      \
                                                                                                             \
    Does nothing if index is invalid                                                                         \
)doc")                                                                                                       \
        .def("deleteAllScalarFields",                                                                        \
             &CCCoreLib::PointCloudTpl<T, StringType>::deleteAllScalarFields,                                \
             "Deletes all scalar fields associated to this cloud")                                           \
        .def(                                                                                                \
            "addPoint",                                                                                      \
            &CCCoreLib::PointCloudTpl<T, StringType>::addPoint,                                              \
            "P"_a,                                                                                           \
            R"doc(                                                                                           \
    Adds a 3D point to the point cloud                                                                       \
                                                                                                             \
    .. note::                                                                                                \
        For better performances it is better to use :meth:`.addPoints`.                                      \
)doc")                                                                                                       \
        .def(                                                                                                \
            "addPoints",                                                                                     \
            &PyCC::addPointsFromArrays<CCCoreLib::PointCloudTpl<T, StringType>>,                             \
            "xs"_a,                                                                                          \
            "ys"_a,                                                                                          \
            "zs"_a,                                                                                          \
            R"doc(                                                                                           \
    Takes values from xs, yz, zs array and add them as points of the point cloud.                            \
                                                                                                             \
    Raises                                                                                                   \
    ------                                                                                                   \
    Value error if xs,ys,zs do not have the same length                                                      \
)doc")                                                                                                       \
        .def("__len__", &CCCoreLib::PointCloudTpl<T, StringType>::size);

#endif // PYTHON_PLUGIN_WRAPPERS_H
