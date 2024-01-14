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

namespace py = pybind11;
using namespace pybind11::literals;

void define_ScalarField(py::module &cccorelib)
{
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
             &CCCoreLib::ScalarField::size,
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
        .def("reserve", &CCCoreLib::ScalarField::reserve, "count"_a, R"doc(
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
            [](CCCoreLib::ScalarField &self, size_t count, ScalarType valueForNewElements)
            {
                // pybind11 will convert exceptions
                self.resize(count, valueForNewElements);
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
        .def("getValue",
             static_cast<ScalarType &(CCCoreLib::ScalarField::*)(std::size_t)>(
                 &CCCoreLib::ScalarField::getValue),
             "index"_a,
             R"doc(
    Returns the value at the given index.

    Only supports index in [0..self.size()[

    Raises
    ------
    IndexError on invalid index
)doc")
        .def("setValue",
             &CCCoreLib::ScalarField::setValue,
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
            [](CCCoreLib::ScalarField &self) { return PyCC::VectorAsNumpyArray(self); },
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
        .def("__getitem__",
             static_cast<ScalarType &(CCCoreLib::ScalarField::*)(std::size_t)>(
                 &CCCoreLib::ScalarField::getValue))
        .def("__setitem__", &CCCoreLib::ScalarField::setValue)
        .def("__repr__",
             [](const CCCoreLib::ScalarField &self)
             { return std::string("<ScalarField(name=") + self.getName() + ")>"; });
}
