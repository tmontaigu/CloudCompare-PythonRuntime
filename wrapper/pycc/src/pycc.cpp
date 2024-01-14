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

#include <CCTypes.h>

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QProgressDialog>
#include <QWidget>

#include "casters.h"

#undef slots
#include <pybind11/numpy.h>
#include <pybind11/stl_bind.h>

#ifdef PYCC_STAND_ALONE
#include "PyPrintLogger.h"
#include <FileIOFilter.h>
#include <QApplication>
#include <QCoreApplication>
#endif

namespace py = pybind11;
using namespace pybind11::literals;

void define_qcc_db(py::module &);
void define_qcc_io(py::module &);
void load_pluginWrappers(py::module &);

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

// Some plugins in CloudCompare are written to always display some
// kind of dialogs.
//
// However, to display a dialog, a QApplication must be running
// which is not the case in stand-alone build, thus we provide a function
// that allows user to quickly set up a QApplication.
//
// We do not set a QApplication up by default in stand alone
// as we think user might want to do it themselves if they plan on using PyQt5 for example
// but we may have to change that.
void InitQApplication()
{
#ifdef PYCC_STAND_ALONE
#ifdef Q_OS_WIN32
    const py::module_ os = py::module_::import("os");
    const py::module_ sysconfig = py::module_::import("sysconfig");
    const QString site_packages = sysconfig.attr("get_path")("purelib").cast<QString>();
    const QString platform_path = QString("%1\\pycc\\platforms").arg(site_packages);
    const std::string std_platform_path = platform_path.toStdString();
    os.attr("environ")["QT_QPA_PLATFORM_PLUGIN_PATH"] = std_platform_path;
#endif
    // TODO use sys.argv ? and if then we don't need the env var thing on windows
    static char fakeArg0[] = "pycc";
    static char *fakeArgv[] = {&fakeArg0[0], NULL};
    static int argc = 1;
    if (QCoreApplication::instance() == nullptr)
    {
        new QApplication(argc, &fakeArgv[0]);
    }
#endif
    // Whe not built as stand alone, the QApplication is CloudCompare
    // so, we have nothing to do.
}

void import_pyccRuntime(py::module &m)
{
#if PYBIND11_VERSION_MINOR >= 5
    const py::module_ pycc_runtime = py::module_::import("pycc_runtime");
#else
    const py::module pycc_runtime = py::module::import("pycc_runtime");
#endif
    const auto runtime_module_content = pycc_runtime.attr("__dict__").cast<py::dict>();
    const auto pycc_module_content = m.attr("__dict__").cast<py::dict>();

    for (const auto &entry : runtime_module_content)
    {
        pycc_module_content[entry.first] = entry.second;
    }
}

void define_someQtThings(py::module &m)
{
    py::class_<QWidget>(m, "QWidget");
    py::class_<QOpenGLWidget>(m, "QOpenGLWidget");
    py::class_<QProgressDialog, QWidget>(m, "QProgressDialog");
    py::class_<QMainWindow, QWidget>(m, "QMainWindow");
    py::class_<QFont>(m, "QFont")
        .def("__repr__",
             [](const QFont &self)
             { return std::string("<QFont(") + self.toString().toStdString() + ")>"; });
    py::class_<QSize>(m, "QSize")
        .def(py::init<>())
        .def(py::init<int, int>(), "width"_a, "height"_a)
        .def("height", &QSize::height)
        .def("width", &QSize::width)
        .def("isEmpty", &QSize::isEmpty)
        .def("isValid", &QSize::isValid)
        .def("__repr__",
             [](const QSize &self)
             {
                 return std::string("<QSize(") + std::to_string(self.width()) + ", " +
                        std::to_string(self.height()) + ")>";
             });

    py::class_<QPointF>(m, "QPointF");
    // TODO: https://doc.qt.io/qt-5/qt.html#CursorShape-enum
    py::enum_<Qt::CursorShape>(m, "QtCursorShape");
}

void define_pycc(py::module &m)
{
    m.doc() = R"pbdoc(
        Python module exposing some CloudCompare functions
    )pbdoc";

    py::module::import("cccorelib");

#ifdef PYCC_STAND_ALONE
    // When pycc is a standalone python module, there are
    // some CloudCompare facilities we have to initialize ourselves
    // to have a more complete experience
    ccLog::RegisterInstance(new PyPrintLogger);
    FileIOFilter::InitInternalFilters();

    // TODO at module exit definition to cleanup ?
#endif

    define_someQtThings(m);

    m.attr("PointCoordinateType") = py::dtype("float32");
    define_scalar_type<ScalarType>(m);

    define_qcc_db(m);
    define_qcc_io(m);

    m.def("InitQApplication", &InitQApplication);

    try
    {
        load_pluginWrappers(m);
    }
    catch (std::exception &e)
    {
        ccLog::Print("Failed to import wrapped plugins: ", e.what());
    }

#ifdef DEFINE_PYCC_RUNTIME
    try
    {
        import_pyccRuntime(m);
    }
    catch (std::exception &e)
    {
        ccLog::Print("Failed to import pycc_runtime content: ", e.what());
    }
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
