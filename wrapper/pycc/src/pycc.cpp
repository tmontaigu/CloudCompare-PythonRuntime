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

#include <CCTypes.h>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QProgressDialog>
#include <QWidget>

#include "casters.h"

#undef slots
#include <pybind11/numpy.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_qcc_db(py::module &m);
void define_pycc_runtime(py::module &);
void define_qcc_io(py::module &);

template <class T> using observer_ptr = std::unique_ptr<T, py::nodelete>;

template <typename T>
typename std::enable_if<std::is_same<T, double>::value, void>::type
define_scalar_type(py::module &m)
{
    m.attr("ScalarType") = py::dtype("float64");
}

template <typename T>
typename std::enable_if<std::is_same<T, float>::value, void>::type define_scalar_type(py::module &m)
{
    m.attr("ScalarType") = py::dtype("float32");
}

void define_pycc(py::module &m)
{
    py::module::import("cccorelib");

    py::class_<QWidget>(m, "QWidget");
    py::class_<QOpenGLWidget>(m, "QOpenGLWidget");
    py::class_<QProgressDialog, QWidget>(m, "QProgressDialog");
    py::class_<QMainWindow, QWidget>(m, "QMainWindow");
    py::class_<QFont>(m, "QFont").def("__repr__", [](const QFont &self) {
        return std::string("<QFont(") + self.toString().toStdString() + ")>";
    });
    py::class_<QSize>(m, "QSize")
        .def(py::init<>())
        .def(py::init<int, int>(), "width"_a, "height"_a)
        .def("height", &QSize::height)
        .def("width", &QSize::width)
        .def("isEmpty", &QSize::isEmpty)
        .def("isValid", &QSize::isValid)
        .def("__repr__", [](const QSize &self) {
            return std::string("<QSize(") + std::to_string(self.width()) + ", " +
                   std::to_string(self.height()) + ")>";
        });

    m.attr("PointCoordinateType") = py::dtype("float32");
    define_scalar_type<ScalarType>(m);

    py::class_<QPointF>(m, "QPointF");

    m.doc() = R"pbdoc(
        Python module exposing some CloudCompare functions
    )pbdoc";

    define_qcc_db(m);
    define_qcc_io(m);
#ifdef DEFINE_PYCC_RUNTIME
    define_pycc_runtime(m);
#endif
}

#ifdef USE_EMBEDDED_MODULES
#include <pybind11/embed.h>
PYBIND11_EMBEDDED_MODULE(pycc, m)
{
    define_pycc(m);
}
#else
PYBIND11_MODULE(pycc, m)
{
    define_pycc(m);
}
#endif
