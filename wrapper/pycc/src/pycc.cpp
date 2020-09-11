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
//#                             COPYRIGHT: Thomas Montaigu                 #
//#                                                                        #
//##########################################################################

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <ccHObject.h>
#include <ccHObjectCaster.h>
#include <ccPointCloud.h>
#include <ccScalarField.h>
#include <ccDrawableObject.h>
#include <CloudSamplingTools.h>

#include <BoundingBox.h>
#include <ccMainAppInterface.h>
#include <AutoSegmentationTools.h>
#include <GeometricalAnalysisTools.h>
#include <GenericProgressCallback.h>
#include <ccProgressDialog.h>
#include <PointCloud.h>
#include <QCoreApplication>
#include <QtConcurrent>
#include <QException>
#include <ccGenericMesh.h>

#include "casters.h"
#include "Runtime.h"
#include "ccGUIPythonInstance.h"

#include <FileIOFilter.h>
#include <ccCommandLineInterface.h>


namespace py = pybind11;
using namespace pybind11::literals;


class PyThreadStateGuard {
public:
	explicit PyThreadStateGuard(PyInterpreterState *interpState) {
		pCurrentState = PyThreadState_New(interpState);
		PyEval_AcquireThread(pCurrentState);
	}

	virtual ~PyThreadStateGuard() {
		PyThreadState_Clear(pCurrentState);
		PyEval_ReleaseThread(pCurrentState);

		PyThreadState_Delete(pCurrentState);
	}

private:
	PyThreadState *pCurrentState{nullptr};
};

struct PyThreadStateReleaser {
	explicit PyThreadStateReleaser() : state(PyEval_SaveThread()) {}

	virtual ~PyThreadStateReleaser() {
		PyEval_RestoreThread(state);
	}


	PyThreadState *state{nullptr};
};

class MyException : public QException {
public:
	explicit MyException(const std::exception &err) : e(err) {}

	void raise() const override { throw *this; }

	QException *clone() const override { return new MyException(*this); }

	const char *what() const noexcept override {
		return e.what();
	}

	std::exception error() const { return e; }

private:
	std::exception e;
};

py::object call_fn(PyThreadState *main_state, py::object callable, py::args args, py::kwargs kwargs) {
	PyThreadStateGuard threadStateGuard{main_state->interp};
	try
	{
		return callable(*args, **kwargs);
	} catch (const std::exception &e)
	{
		throw MyException(e);
	}
}


void define_ccCommandLine(py::module&);
void define_ccDrawableObject(py::module&);
void define_ccScalarField(py::module&);
void define_ccObject(py::module&);
void define_ccGenericPointCloud(py::module&);
void define_ccPointCloud(py::module&);


void define_ccGUIPythonInstance(py::module&);

template<class T>
using observer_ptr = std::unique_ptr<T, py::nodelete>;

PYBIND11_MODULE(pycc, m) {
	py::module::import("cccorelib");

	m.doc() = R"pbdoc(
        Python module exposing some CloudCompare functions
    )pbdoc";


	/**********************************
	 * qCC_db
	 **********************************/

	py::class_<ccGenericGLDisplay>(m, "ccGenericGLDisplay");

	define_ccScalarField(m);

	define_ccDrawableObject(m);
	define_ccObject(m);

	define_ccGenericPointCloud(m);
	define_ccPointCloud(m);

	py::class_<QProgressDialog>(m, "QProgressDialog");

	py::class_<ccProgressDialog, QProgressDialog, CCCoreLib::GenericProgressCallback>(m, "ccProgressDialog")
			.def(py::init<bool>(), "cancelButton"_a = false);


	py::class_<ccGlobalShiftManager> PyccGlobalShiftManager(m, "ccGlobalShiftManager");

	py::enum_<ccGlobalShiftManager::Mode>(PyccGlobalShiftManager, "Mode")
			.value("NO_DIALOG", ccGlobalShiftManager::Mode::NO_DIALOG)
			.value("NO_DIALOG_AUTO_SHIFT", ccGlobalShiftManager::Mode::NO_DIALOG_AUTO_SHIFT)
			.value("DIALOG_IF_NECESSARY", ccGlobalShiftManager::Mode::DIALOG_IF_NECESSARY)
			.value("ALWAYS_DISPLAY_DIALOG", ccGlobalShiftManager::Mode::ALWAYS_DISPLAY_DIALOG)
			.export_values();


	py::class_<FileIOFilter> PyFileIOFilter(m, "FileIOFilter");
	PyFileIOFilter.def_static("LoadFromFile", [](const QString &filename,
	                                             FileIOFilter::LoadParameters &parameters) {
		CC_FILE_ERROR result = CC_FERR_NO_ERROR;
		ccHObject * newGroup = FileIOFilter::LoadFromFile(filename, parameters, result);
		return newGroup;
	}, py::return_value_policy::reference);

	py::class_<FileIOFilter::LoadParameters>(PyFileIOFilter, "LoadParameters")
			.def(py::init<>())
			.def_readwrite("shiftHandlingMode", &FileIOFilter::LoadParameters::shiftHandlingMode)
			.def_readwrite("alwaysDisplayLoadDialog", &FileIOFilter::LoadParameters::alwaysDisplayLoadDialog)
			.def_readwrite("coordinatesShiftEnabled", &FileIOFilter::LoadParameters::coordinatesShiftEnabled)
			.def_readwrite("coordinatesShift", &FileIOFilter::LoadParameters::coordinatesShift)
			.def_readwrite("preserveShiftOnSave", &FileIOFilter::LoadParameters::preserveShiftOnSave)
			.def_readwrite("autoComputeNormals", &FileIOFilter::LoadParameters::autoComputeNormals)
//			.def_readwrite("parentWidget", &FileIOFilter::LoadParameters::parentWidget)
			.def_readwrite("sessionStart", &FileIOFilter::LoadParameters::sessionStart);


	define_ccGUIPythonInstance(m);
	define_ccCommandLine(m);

	m.def("GetInstance", &GetInstance, py::return_value_policy::reference);
	m.def("GetCmdLineInstance", &GetCmdLineInstance, py::return_value_policy::reference);

	m.def("ProcessEvents", []() {
		QCoreApplication::processEvents();
	});

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

        m.def("CreateObject", [](const char* type_name) {
                return GetInstance()->createObject(type_name);
        });
}