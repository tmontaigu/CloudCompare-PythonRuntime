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

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

#include "wrappers.h"

#include <ScalarField.h>
#include <stdio.h>

namespace py = pybind11;
using namespace pybind11::literals;

class ScalarFieldView
{
  public:
    explicit ScalarFieldView(CCCoreLib::ScalarField &field)
        : m_field(field), m_start(0), m_stop(field.currentSize()), m_step(1), m_len(field.currentSize())
    {
    }

    explicit ScalarFieldView(const ScalarFieldView view, const py::slice &slice) : ScalarFieldView(view)
    {
        applySlicing(slice);
    }

    explicit ScalarFieldView(CCCoreLib::ScalarField &field, const py::slice &slice) : ScalarFieldView(field)
    {
        applySlicing(slice);
    }

    py::object get_item(const py::object &index)
    {
        if (py::isinstance<py::int_>(index))
        {
            const size_t actualIndex = (index.cast<size_t>() * m_step) + m_start;
            if (actualIndex >= m_stop)
            {
                throw py::index_error("invalid index");
            }
            return py::cast(m_field.getValue(actualIndex));
        }

        if (py::isinstance<py::slice>(index))
        {
            const auto slice = index.cast<py::slice>();
            return py::cast(ScalarFieldView(m_field, slice));
        }

        if (py::isinstance<py::sequence>(index))
        {
            const auto sequence = index.cast<py::sequence>();
            py::array_t<double> result(sequence.size());
            auto setter = result.mutable_unchecked<1>();
            for (size_t i = 0; i < sequence.size(); ++i)
            {
                const size_t actualIndex = (sequence[i].cast<size_t>() * m_step) + m_start;
                if (actualIndex >= m_stop)
                {
                    throw py::index_error("invalid index");
                }
                setter[i] = m_field.getValue(actualIndex);
            }
            return result;
        }

        throw py::index_error("Invalid index type");
    }

    void set_item(const py::object &index, const py::object &value)
    {
        if (py::isinstance<py::int_>(index))
        {
            const size_t actualIndex = (index.cast<size_t>() * m_step) + m_start;
            if (actualIndex >= m_stop)
            {
                throw py::index_error("invalid index");
            }
            m_field.setValue(actualIndex, value.cast<double>());
            return;
        }

        if (py::isinstance<py::slice>(index))
        {
            const auto slice = index.cast<py::slice>();

            // view[some_slice] = some_unique_value
            if (py::isinstance<py::float_>(value) || py::isinstance<py::int_>(value))
            {
                const auto floatValue = value.cast<double>();
                for (size_t i = 0; i < m_len; ++i)
                {
                    const size_t actualIndex = (i * m_step) + m_start;
                    if (actualIndex >= m_stop)
                    {
                        throw py::index_error("invalid index");
                    }
                    m_field.setValue(actualIndex, floatValue);
                }
                return;
            }

            const auto sequence = value.cast<py::sequence>();
            if (sequence.size() != m_len)
            {
                throw py::index_error("Incorrect number of new values");
            }

            for (size_t i = 0; i < m_len; ++i)
            {
                const size_t actualIndex = (i * m_step) + m_start;
                if (actualIndex >= m_stop)
                {
                    throw py::index_error("invalid index");
                }
                m_field.setValue(actualIndex, sequence[i].cast<double>());
            }

            return;
        }

        if (py::isinstance<py::sequence>(index))
        {
            const auto sequence = index.cast<py::sequence>();

            // view[some_sequence] = some_unique_value
            if (py::isinstance<py::float_>(value) || py::isinstance<py::int_>(value))
            {
                const auto floatValue = value.cast<double>();
                for (size_t i = 0; i < m_len; ++i)
                {
                    m_field.setValue((i * m_step) + m_start, floatValue);
                }
                return;
            }
            if (sequence.size() != m_len)
            {
                throw py::index_error("Incorrect number of new values");
            }

            for (size_t i = 0; i < m_len; ++i)
            {
                m_field.setValue((i * m_step) + m_start, sequence[i].cast<double>());
            }
        }
    }

    size_t len() const
    {
        return m_len;
    }

    void applySlicing(const py::slice &slice)
    {
        size_t start, stop, step, len = 0;
        if (!slice.compute(m_len, &start, &stop, &step, &len))
        {
            throw py::index_error();
        }

        m_len = len;
        m_start += start;
        m_stop = stop;
        m_step += step;
    }

  private:
    CCCoreLib::ScalarField &m_field;
    size_t m_start;
    size_t m_stop;
    size_t m_step;
    size_t m_len;
};

void define_ScalarField(py::module &cccorelib)
{
    py::class_<ScalarFieldView>(cccorelib, "ScalarFieldView")
        .def("__len__", &ScalarFieldView::len)
        .def("__getitem__", &ScalarFieldView::get_item, py::return_value_policy::reference_internal)
        .def("__setitem__", &ScalarFieldView::set_item);

    py::class_<CCCoreLib::ScalarField, CCShareable, CCShareableHolder<CCCoreLib::ScalarField>>(cccorelib,
                                                                                               "ScalarField",
                                                                                               R"doc(
    ScalarField

    .. note::
        Note that cccorelib.ScalarField uses float while Python only uses doubles
        which means a loss of precision will happen.

    Getting / Setting values can be done via :meth:`cccorelib.ScalarField.getValue`
    or :meth:`cccorelib.ScalarField.setValue`.
    Alternatively, it is possible to use bracket operator.

    Use :meth:`cccorelib.ScalarField.asArray` to be able to use the
    scalar field as a normal numpy array.


    Example
    -------

    .. code:: Python

        scalar_field = cccorelib.ScalarField("Codification")
)doc")
        .def_static("NaN", &CCCoreLib::ScalarField::NaN)

        .def(py::init<const char *>(), "name"_a, R"doc(
    Creates a ScalarField with the given name

    Example
    -------

    .. code:: Python

        scalar_field = cccorelib.ScalarField("Codification")
        scalar_field.resize(3)

        scalar_field.setValue(1, 1.0)
        scalar_field[2] = 2.0

        assert scalar_field.getValue(0) == scalar_field[0]
        assert scalar_field.getValue(1) == scalar_field[1]
        assert scalar_field.getValue(2) == scalar_field[2]
)doc")
        .def("setName", &CCCoreLib::ScalarField::setName, R"doc(
    Sets the name of the scalar field

    >>> import cccorelib
    >>> sf = cccorelib.ScalarField("name")
    >>> sf.setName("other_name")
    >>> sf.getName() == "other_name"
    True
)doc")
        .def("getName", &CCCoreLib::ScalarField::getName, R"doc(
    Returns the name of the scalar field

    >>> import cccorelib
    >>> sf = cccorelib.ScalarField("name")
    >>> sf.getName()
    'name'
 )doc")
        .def("size",
             &CCCoreLib::ScalarField::currentSize,
             "Returns the number of elements (values) in the scalar field")
        .def("computeMeanAndVariance",
             &CCCoreLib::ScalarField::computeMeanAndVariance,
             "mean"_a,
             "variance"_a = nullptr)
        .def("computeMinAndMax", &CCCoreLib::ScalarField::computeMinAndMax, R"doc(
    Computes the Min and Max, this needs to be called before retrieving the values
    with getMin or getMax
)doc")
        .def("getMin", &CCCoreLib::ScalarField::getMin, R"doc(
    Returns the lowest value in the scalar field

    You need to call :meth:`cccorelib.ScalarField.computeMinAndMax` if the array has changed
    since last call.
)doc")
        .def("getMax", &CCCoreLib::ScalarField::getMax, R"doc(
    Returns the highest value in the scalar field

    You need to call :meth:`cccorelib.ScalarField.computeMinAndMax` if the array has changed
    since last call.
)doc")
        .def_static("ValidValue", &CCCoreLib::ScalarField::ValidValue, "value"_a)
        .def("flagValueAsInvalid", &CCCoreLib::ScalarField::flagValueAsInvalid, "index"_a)
        .def("fill",
             &CCCoreLib::ScalarField::fill,
             "fillValue"_a = 0,
             "Fills the scalar field with the given value")
        .def(
            "reserve",
            [](CCCoreLib::ScalarField &field, const size_t size) { field.reserve(size); },
            "count"_a,
            R"doc(
    Reserves space for ``count`` element, but does not change the size.

    Will raise an exception if allocation failed
)doc")
        .def("reserveSafe", &CCCoreLib::ScalarField::reserveSafe, "count"_a, R"doc(
    Reserves space for ``count`` element, but does not change the size.

    Will NOT raise an exception if allocation failed.
    Instead, it returns a bool to indicate success.

    Prefer use of :meth:`cccorelib.ScalarField.reserve`.
)doc")
        .def(
            "resize",
            [](CCCoreLib::ScalarField &self, const size_t count, const ScalarType valueForNewElements)
            {
                // pybind11 will convert exceptions
                self.resizeSafe(count, true /* always init values */, valueForNewElements);
            },
            "count"_a,
            "valueForNewElements"_a = 0,
            R"doc(
    Resize the scalar field

    Will raise an exception if allocation failed
)doc")
        .def("resizeSafe",
             &CCCoreLib::ScalarField::resizeSafe,
             "count"_a,
             "initNewElements"_a = false,
             "valueForNewElements"_a = 0,
             R"doc(
    Resize the scalar field

    Will NOT raise an exception if allocation failed.
    Instead, it returns a bool to indicate success.

    Prefer use of :meth:`cccorelib.ScalarField.resize`.
)doc")
        .def(
            "getValue",
            [](CCCoreLib::ScalarField &self, size_t index)
            {
                if (index >= self.currentSize())
                {
                    throw py::index_error("invalid index");
                }
                return self.getValue(index);
            },
            "index"_a,
            R"doc(
    Returns the value at the given index.

    Only supports index in [0..self.size()[

    Raises
    ------
    IndexError on invalid index
)doc")
        .def(
            "setValue",
            [](CCCoreLib::ScalarField &self, size_t index, double value)
            {
                if (index >= self.currentSize())
                {
                    throw py::index_error("invalid index");
                }
                self.setValue(index, value);
            },
            "index"_a,
            "value"_a,
            R"doc(
    Sets the value at the given index.

    Only supports index in [0..self.size()[

    Raises
    ------
    IndexError on invalid index
)doc")
        .def("addElement", &CCCoreLib::ScalarField::addElement, "value"_a, R"doc(
    Appends a value

    Example
    -------
    >>> import cccorelib
    >>> sf = cccorelib.ScalarField("name")
    >>> sf.size()
    0
    >>> sf.addElement(1)
    >>> sf.size()
    1
)doc")
        .def(
            "asArray",
            [](CCCoreLib::ScalarField &self) { return ScalarFieldView(self); },
            R"doc(
    Returns the scalar field viewed as a numpy array.

    This does not return a copy, so changes made to the array a reflected
    in the scalar field (and vice-versa).

    .. code:: Python

        scalar_field = cccorelib.ScalarField("Codification")
        scalar_field.resize(10)

        array = scalar_field.asArray()

        assert np.all(array == 0)
        # Changes made to the scalar_field reflects on the array
        scalar_field.fill(1.0)
        assert np.all(array == 1.0)

        # and vice-versa
        array[:] = 2.0
        assert scalar_field[0] == 2.0
)doc")
        .def(
            "__getitem__",
            [](CCCoreLib::ScalarField &self, const py::object &index)
            { return ScalarFieldView(self).get_item(index); },
            py::return_value_policy::reference_internal)
        .def(
            "toArray",
            [](const CCCoreLib::ScalarField &self)
            {
                py::array_t<double> copy(self.currentSize());
                auto access = copy.mutable_unchecked<1>();
                for (size_t i = 0; i < self.currentSize(); ++i)
                {
                    access[i] = self.getValue(i);
                }
                return copy;
            },
            R"doc(
        Copies the content into an array
)doc")
        .def(
            "__array__",
            [](const CCCoreLib::ScalarField &self)
            {
                // TODO this is a copy-paste from toArray, should only have 1 definition of this
                py::array_t<double> copy(self.currentSize());
                auto access = copy.mutable_unchecked<1>();
                for (size_t i = 0; i < self.currentSize(); ++i)
                {
                    access[i] = self.getValue(i);
                }
                return copy;
            },
            R"doc(
        Copies the content into an array
)doc")
        .def("__setitem__",
             [](CCCoreLib::ScalarField &self, const py::object &index, const py::object &value)
             { return ScalarFieldView(self).set_item(index, value); })
        .def("__len__", &CCCoreLib::ScalarField::currentSize)
        .def("__repr__",
             [](const CCCoreLib::ScalarField &self)
             { return std::string("<ScalarField(name=") + self.getName() + ")>"; });
}
