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
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <ccBBox.h>
#include <ccGLMatrix.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccBBox(py::module &m)
{
    py::class_<ccBBox, CCCoreLib::BoundingBox>(m, "ccBBox", R"pbdoc(
    Bounding box structure

    supports multiplication (*) with :py:class:`pycc.ccGLMatrix` to apply a transformation.

    Examples
    --------

    >>> import pycc
    >>> emtpy_bbox = pycc.ccBBox()

    >>> import cccorelib
    >>> import pycc
    >>> bb_min = cccorelib.CCVector3(0.0, 0.0, 0.0)
    >>> bb_max = cccorelib.CCVector3(10.0, 15.0, 10.0)
    >>> bbox = pycc.ccBBox(bb_min, bb_max)
)pbdoc")
        .def(py::init<>())
        .def(py::init<const CCVector3 &, const CCVector3 &, bool>(),
             "bbMinCorner"_a,
             "bbMaxCorner"_a,
             "valid"_a)
        .def(
            "__mul__",
            [](ccBBox &self, const ccGLMatrix &mat) { return self * mat; },
            py::is_operator())
        .def(
            "__mul__",
            [](ccBBox &self, const ccGLMatrixd &mat) { return self * mat; },
            py::is_operator())
        .def("draw", &ccBBox::draw, "context"_a, "col"_a);
}
