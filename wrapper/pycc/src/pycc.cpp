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


#include <CloudSamplingTools.h>
#include <ccDrawableObject.h>
#include <ccHObject.h>
#include <ccHObjectCaster.h>
#include <ccPointCloud.h>
#include <ccScalarField.h>

#include <AutoSegmentationTools.h>
#include <BoundingBox.h>
#include <GenericProgressCallback.h>
#include <GeometricalAnalysisTools.h>
#include <PointCloud.h>
#include <QCoreApplication>
#include <QException>
#include <QMainWindow>
#include <QtConcurrent>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <ccGenericMesh.h>
#include <ccMainAppInterface.h>
#include <ccProgressDialog.h>
#include <ccColorTypes.h>
#include <ccAdvancedTypes.h>

#include "Runtime.h"
#include "casters.h"
#include "ccGUIPythonInstance.h"

#include <FileIOFilter.h>
#include <ccCommandLineInterface.h>

#undef slots
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;
using namespace pybind11::literals;

class PyThreadStateGuard
{
  public:
    explicit PyThreadStateGuard(PyInterpreterState *interpState)
    {
        pCurrentState = PyThreadState_New(interpState);
        PyEval_AcquireThread(pCurrentState);
    }

    virtual ~PyThreadStateGuard()
    {
        PyThreadState_Clear(pCurrentState);
        PyEval_ReleaseThread(pCurrentState);

        PyThreadState_Delete(pCurrentState);
    }

  private:
    PyThreadState *pCurrentState{nullptr};
};

struct PyThreadStateReleaser
{
    explicit PyThreadStateReleaser() : state(PyEval_SaveThread()) {}

    virtual ~PyThreadStateReleaser()
    {
        PyEval_RestoreThread(state);
    }

    PyThreadState *state{nullptr};
};

class MyException : public QException
{
  public:
    explicit MyException(const std::exception &err) : e(err) {}

    void raise() const override
    {
        throw *this;
    }

    QException *clone() const override
    {
        return new MyException(*this);
    }

    const char *what() const noexcept override
    {
        return e.what();
    }

    std::exception error() const
    {
        return e;
    }

  private:
    std::exception e;
};

py::object call_fn(PyThreadState *main_state, py::object callable, py::args args, py::kwargs kwargs)
{
    PyThreadStateGuard threadStateGuard{main_state->interp};
    try
    {
        return callable(*args, **kwargs);
    }
    catch (const std::exception &e)
    {
        throw MyException(e);
    }
}

void define_ccCommandLine(py::module &);
void define_ccDrawableObject(py::module &);
void define_ccScalarField(py::module &);
void define_ccObject(py::module &);
void define_ccGenericPointCloud(py::module &);
void define_ccPointCloud(py::module &);
void define_ccGenericGLDisplay(py::module &);
void define_ccGLWindow(py::module &);
void define_ccGenericMesh(py::module &);
void define_ccMesh(py::module &);
void define_ccGenericPrimitive(py::module &);
void define_ccSphere(py::module &);
void define_ccGLMatrix(py::module &);
void define_ccGUIPythonInstance(py::module &);
void define_ccPlane(py::module &);
void define_ccTorus(py::module &);
void define_ccBox(py::module &);
void define_ccDish(py::module &);
void define_ccCone(py::module &);
void define_ccCylinder(py::module& );
void define_ccProgressDialog(py::module&);
void define_ccLog(py::module&);
void define_ccInteractor(py::module &);
void define_cc2DLabel(py::module &);

template <class T> using observer_ptr = std::unique_ptr<T, py::nodelete>;



void define_pycc(py::module &m)
{
    py::module::import("cccorelib");

    py::class_<QWidget>(m, "QWidget");
    py::class_<QOpenGLWidget>(m, "QOpenGLWidget");
    py::class_<QProgressDialog, QWidget>(m, "QProgressDialog");
    py::class_<QMainWindow, QWidget>(m, "QMainWindow");
    py::class_<QFont>(m, "QFont")
        .def("__repr__", [](const QFont& self)
        {
            return std::string("<QFont(") + self.toString().toStdString() + ")>";
         });
    py::class_<QSize>(m, "QSize")
        .def(py::init<>())
        .def(py::init<int, int>(), "width"_a, "height"_a)
        .def("height", &QSize::height)
        .def("width", &QSize::width)
        .def("isEmpty", &QSize::isEmpty)
        .def("isValid", &QSize::isValid)
        .def("__repr__", [](const QSize& self) {
            return std::string("<QSize(") + std::to_string(self.width()) + ", " + std::to_string(self.height()) + ")>";
        });

    py::class_<QPointF>(m, "QPointF");

    m.doc() = R"pbdoc(
        Python module exposing some CloudCompare functions
    )pbdoc";

    /**********************************
     * qCC_db
     **********************************/

    py::class_<ccColor::Rgba>(m, "Rgba");


    define_ccGenericGLDisplay(m);
    define_ccGLWindow(m);
    define_ccScalarField(m);
    define_ccGLMatrix(m);
    define_ccInteractor(m);



    define_ccDrawableObject(m);
    define_ccObject(m);


    using VectorCompressedNormType = std::vector<CompressedNormType>;
    using ccArrayCompressedNormType = ccArray<CompressedNormType, 1, CompressedNormType>;

    // We can't bind_vector, at runtime we get an error that _VectorCompressedNormType
    // is already defined, that is because CompressedNormType is unsigned int
    // and std::vector<unsigned int> is already bound in define_KdTree
    //py::bind_vector<VectorCompressedNormType>(m, "_VectorCompressedNormType", py::module_local(true));

    py::class_<ccArrayCompressedNormType,
            VectorCompressedNormType,
            CCShareable,
            ccHObject,
            observer_ptr<ccArrayCompressedNormType>>
            (m, "_NormsIndexesArrayType");
    py::class_<NormsIndexesTableType, ccArrayCompressedNormType>(m, "NormsIndexesTableType");

    define_ccGenericMesh(m);
    define_ccMesh(m);
    define_ccGenericPrimitive(m);
    define_ccSphere(m);
    define_ccPlane(m);
    define_ccTorus(m);
    define_ccBox(m);
    define_ccDish(m);
    define_ccCone(m);
    define_ccCylinder(m);
    define_cc2DLabel(m);

    define_ccGenericPointCloud(m);
    define_ccPointCloud(m);

    define_ccLog(m);
    define_ccProgressDialog(m);

    py::class_<ccGlobalShiftManager> PyccGlobalShiftManager(m, "ccGlobalShiftManager");

    py::enum_<ccGlobalShiftManager::Mode>(PyccGlobalShiftManager, "Mode")
        .value("NO_DIALOG", ccGlobalShiftManager::Mode::NO_DIALOG)
        .value("NO_DIALOG_AUTO_SHIFT", ccGlobalShiftManager::Mode::NO_DIALOG_AUTO_SHIFT)
        .value("DIALOG_IF_NECESSARY", ccGlobalShiftManager::Mode::DIALOG_IF_NECESSARY)
        .value("ALWAYS_DISPLAY_DIALOG", ccGlobalShiftManager::Mode::ALWAYS_DISPLAY_DIALOG)
        .export_values();

    py::class_<FileIOFilter> PyFileIOFilter(m, "FileIOFilter");
    PyFileIOFilter.def_static(
        "LoadFromFile",
        [](const QString &filename, FileIOFilter::LoadParameters &parameters) {
            CC_FILE_ERROR result = CC_FERR_NO_ERROR;
            ccHObject *newGroup = FileIOFilter::LoadFromFile(filename, parameters, result);
            return newGroup;
        },
        py::return_value_policy::reference);

    py::class_<FileIOFilter::LoadParameters>(PyFileIOFilter, "LoadParameters")
        .def(py::init<>())
        .def_readwrite("shiftHandlingMode", &FileIOFilter::LoadParameters::shiftHandlingMode)
        .def_readwrite("alwaysDisplayLoadDialog", &FileIOFilter::LoadParameters::alwaysDisplayLoadDialog)
        .def_readwrite("coordinatesShiftEnabled", &FileIOFilter::LoadParameters::coordinatesShiftEnabled)
        .def_readwrite("coordinatesShift",
                       &FileIOFilter::LoadParameters::coordinatesShift,
                       py::return_value_policy::reference)
        .def_readwrite("preserveShiftOnSave", &FileIOFilter::LoadParameters::preserveShiftOnSave)
        .def_readwrite("autoComputeNormals", &FileIOFilter::LoadParameters::autoComputeNormals)
        .def_readwrite(
            "parentWidget", &FileIOFilter::LoadParameters::parentWidget, py::return_value_policy::reference)
        .def_readwrite("sessionStart", &FileIOFilter::LoadParameters::sessionStart);

    define_ccGUIPythonInstance(m);
    define_ccCommandLine(m);

    // Use leading __ to give hints that user should not import this in their scripts
    m.def("__GetREPLInstance", &GetREPLInstance, py::return_value_policy::reference);
    m.def("GetGUIInstance", &GetInstance, py::return_value_policy::reference);
    m.def("GetCmdLineInstance", &GetCmdLineInstance, py::return_value_policy::reference);
    m.def(
        "GetInstance",
        []() -> py::object {
            auto guiInstance = GetInstance();
            if (guiInstance)
            {
                return py::cast(guiInstance);
            }
            else
            {
                return py::cast(GetCmdLineInstance());
            }
        },
        py::return_value_policy::reference);

    m.def("ProcessEvents", []() { QCoreApplication::processEvents(); });

    m.def("RunInThread", [](py::object callable, py::args args, py::kwargs kwargs) {
        PyThreadStateReleaser stateReleaser{};
        QEventLoop loop;
        QFutureWatcher<py::object> watcher;
        QFuture<py::object> future = QtConcurrent::run(call_fn, stateReleaser.state, callable, args, kwargs);
        watcher.setFuture(future);
        QObject::connect(&watcher, &decltype(watcher)::finished, &loop, &decltype(loop)::quit);
        loop.exec();

        return future.result();
    });

    m.def("RunThread", [](py::object thread) {
        py::object isAliveMethod = thread.attr("is_alive");
        thread.attr("start")();
        while (isAliveMethod())
        {
            QCoreApplication::processEvents();
        }
    });
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
