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

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <ccGLMatrix.h>

#include "wrappers.h"

namespace py = pybind11;
using namespace pybind11::literals;

template <typename ccGLMatrixType, typename Inner>
static void define_ccGLMatrixClass(py::module &m, const char *name)
{
    py::class_<ccGLMatrixType>(m, name)
        .def(py::init<>())
        .def(py::init<const Vector3Tpl<Inner> &,
                      const Vector3Tpl<Inner> &,
                      const Vector3Tpl<Inner> &,
                      const Vector3Tpl<Inner> &>(),
             "X"_a,
             "Y"_a,
             "Z"_a,
             "Tr"_a)
        .def_static("Interpolate", &ccGLMatrixType::Interpolate, "coef"_a, "glMat1"_a, "glMat2"_a)
        // For some unknown reason doing
        // .def_static("FromToRotation", &ccGLMatrixType::FromViewDirAndUpDir, "forward"_a, "up"_a)
        // creates a function that will create a runtime error when used:
        // ```
        // TypeError: Unable to convert function return value to a Python type! The signature was
        // (forward: cccorelib.CCVector3d, up: cccorelib.CCVector3d) -> ccGLMatrixTpl<double>
        // ```
        // We have to use a fully typed lambda for it to work.
        // We also provide this function as a __init__ alias
        .def_static(
            "FromViewDirAndUpDir",
            [](const Vector3Tpl<Inner> &a, const Vector3Tpl<Inner> &b) -> ccGLMatrixType
            { return ccGLMatrixType::FromViewDirAndUpDir(a, b); },
            "forward"_a,
            "up"_a)
        .def(py::init(&ccGLMatrixType::FromViewDirAndUpDir), py::kw_only(), "forward"_a, "up"_a)
        // Same problem and solution as above
        .def_static(
            "FromToRotation",
            [](const Vector3Tpl<Inner> &a, const Vector3Tpl<Inner> &b) -> ccGLMatrixType
            { return ccGLMatrixType::FromToRotation(a, b); },
            "from"_a,
            "to"_a)
        .def(py::init(&ccGLMatrixType::FromToRotation), py::kw_only(), "from"_a, "to"_a)
        .def("xRotation", &ccGLMatrixType::xRotation)
        .def("yRotation", &ccGLMatrixType::yRotation)
        .def("zRotation", &ccGLMatrixType::zRotation)
        .def("toZero", &ccGLMatrixType::toZero)
        .def("toIdentity", &ccGLMatrixType::toIdentity)
        .def("clearTranslation", &ccGLMatrixType::clearTranslation)
        .def("getTranslationAsVec3D", &ccGLMatrixType::getTranslationAsVec3D)
        .def("initFromParameters",
             static_cast<void (ccGLMatrixType::*)(
                 Inner, const Vector3Tpl<Inner> &, const Vector3Tpl<Inner> &)>(
                 &ccGLMatrixType::initFromParameters),
             "alpha_rad"_a,
             "axis3D"_a,
             "t3D"_a)
        .def("initFromParameters",
             static_cast<void (ccGLMatrixType::*)(Inner, Inner, Inner, const Vector3Tpl<Inner> &)>(
                 &ccGLMatrixType::initFromParameters),
             "phi_rad"_a,
             "theta_rad"_a,
             "psi_rad"_a,
             "t3D"_a)
        // getParameters, but there are 2 overloads which return their values
        .def("setTranslation",
             static_cast<void (ccGLMatrixType::*)(const Vector3Tpl<Inner> &)>(
                 &ccGLMatrixType::setTranslation),
             "Tr"_a)
        .def("getColumnAsVec3D", &ccGLMatrixType::getColumnAsVec3D, "index"_a)
        .def("setColumn",
             static_cast<void (ccGLMatrixType::*)(unsigned index, const Vector3Tpl<Inner> &)>(
                 &ccGLMatrixType::setColumn),
             "index"_a,
             "v"_a)
        //        .def("setColumn",
        //             static_cast<void (ccGLMatrixType::*)(unsigned index, const Tuple4Tpl<Inner>
        //             &)>(
        //                 &ccGLMatrixType::setColumn),
        //             "index"_a,
        //             "v"_a)
        //        .def(py::self * py::self)
        .def(py::self * Vector3Tpl<Inner>())
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= py::self)
        .def(py::self += Vector3Tpl<Inner>())
        .def(py::self -= Vector3Tpl<Inner>())
        .def("__mul__",
             [](ccGLMatrixType &self, ccGLMatrixType &other) -> ccGLMatrixType
             { return self * other; })
        .def("__getitem__",
             [](const ccGLMatrixType &self, const std::pair<unsigned, unsigned> &item)
             { return self(item.first, item.second); })
        // apply and so on
        .def("shiftRotationCenter", &ccGLMatrixType::shiftRotationCenter, "vec"_a)
        .def("transpose", &ccGLMatrixType::transpose)
        .def("invert", &ccGLMatrixType::invert)
        .def("inverse", &ccGLMatrixType::inverse)
        .def("scaleRotation", &ccGLMatrixType::scaleRotation, "coef"_a)
        .def("scaleRow", &ccGLMatrixType::scaleRow, "rowIndex"_a, "coef"_a)
        .def("scaleColumn", &ccGLMatrixType::scaleColumn, "rowIndex"_a, "coef"_a)
        .def("asArray",
             [](ccGLMatrixType &self) {
                 return PyCC::SpanAsNumpyArray(self.data(), {4, 4});
             });
}

void define_ccGLMatrix(py::module &m)
{
    define_ccGLMatrixClass<ccGLMatrix, float>(m, "ccGLMatrix");
    define_ccGLMatrixClass<ccGLMatrixd, double>(m, "ccGLMatrixd");
}
