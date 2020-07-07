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

#include "wrappers.h"
#include "casters.h"
#include "Runtime.h"

#include <FileIOFilter.h>


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
	explicit PyThreadStateReleaser() : state(PyEval_SaveThread())
	{}

	virtual ~PyThreadStateReleaser()
	{
		PyEval_RestoreThread(state);
	}


	PyThreadState *state{nullptr};
};

class MyException : public QException
{
public:
	explicit MyException(const std::exception &err) : e(err)
	{}

	void raise() const override
	{ throw *this; }

	QException *clone() const override
	{ return new MyException(*this); }

	const char *what() const override
	{
		return e.what();
	}

	std::exception error() const
	{ return e; }

private:
	std::exception e;
};

py::object call_fn(PyThreadState *main_state, py::object callable, py::args args, py::kwargs kwargs)
{
	PyThreadStateGuard threadStateGuard{main_state->interp};
	try
	{
		return callable(*args, **kwargs);
	} catch (const std::exception &e)
	{
		throw MyException(e);
	}
}


PYBIND11_MODULE(pycc, m)
{
	py::module::import("cccorelib");

	m.doc() = R"pbdoc(
        Python module exposing some CloudCompare functions
    )pbdoc";


	/**********************************
	 * qCC_db
	 **********************************/

	py::class_<ccScalarField, CCCoreLib::ScalarField, observer_ptr < ccScalarField>>
	(m, "ccScalarField");

	py::class_<ccGenericGLDisplay>(m, "ccGenericGLDisplay");

	py::class_<ccDrawableObject>(m, "ccDrawableObject")
			.def("isVisible", &ccDrawableObject::isVisible)
			.def("setVisible", &ccDrawableObject::setVisible)
			.def("toggleVisibility", &ccDrawableObject::toggleVisibility)
			.def("isVisibilityLocked", &ccDrawableObject::isVisiblityLocked)
			.def("lockVisibility", &ccDrawableObject::lockVisibility)
			.def("isSelected", &ccDrawableObject::isSelected)
			.def("setSelected", &ccDrawableObject::setSelected)
			.def("hasColors", &ccDrawableObject::hasColors)
			.def("colorsShown", &ccDrawableObject::colorsShown)
			.def("showColors", &ccDrawableObject::showColors)
			.def("toggleColors", &ccDrawableObject::toggleColors)
			.def("hasNormals", &ccDrawableObject::hasNormals)
			.def("normalsShown", &ccDrawableObject::normalsShown)
			.def("showNormals", &ccDrawableObject::showNormals)
			.def("toggleNormals", &ccDrawableObject::toggleNormals)
			.def("hasDisplayedScalarField", &ccDrawableObject::hasDisplayedScalarField)
			.def("hasScalarFields", &ccDrawableObject::hasScalarFields)
			.def("showSF", &ccDrawableObject::showSF)
			.def("toggleSF", &ccDrawableObject::toggleSF)
			.def("sfShown", &ccDrawableObject::sfShown)
			.def("showNameIn3D", &ccDrawableObject::showNameIn3D)
			.def("nameShownIn3D", &ccDrawableObject::nameShownIn3D)
			.def("toggleShowName", &ccDrawableObject::toggleShowName);

	// TODO Metadata { get & set }
	py::class_<ccObject>(m, "ccObject")
			.def("getName", &ccObject::getName)
			.def("setName", &ccObject::setName)
//			.def("getClassID", &ccObject::getClassID)
			.def("getUniqueID", &ccObject::getUniqueID)
			.def("isEnabled", &ccObject::isEnabled)
			.def("setEnabled", &ccObject::setEnabled)
			.def("toggleActivation", &ccObject::toggleActivation)
			.def("isLocked", &ccObject::isLocked)
			.def("setLocked", &ccObject::setLocked)
			.def("isLeaf", &ccObject::isLeaf)
			.def("isCustom", &ccObject::isCustom)
			.def("isHierarchy", &ccObject::isHierarchy);

	py::class_<ccHObject, ccObject, ccDrawableObject>(m, "ccHObject")
			.def("isGroup", &ccHObject::isGroup)
			.def("getParent", &ccHObject::getParent, py::return_value_policy::reference)
					// Children management
			.def("getChildrenNumber", &ccHObject::getChildrenNumber)
			.def("getChildCountRecursive", &ccHObject::getChildCountRecursive)
			.def("getChild", &ccHObject::getChild, py::return_value_policy::reference)
			.def("find", &ccHObject::find, py::return_value_policy::reference);


	py::class_<ccGenericPointCloud, ccHObject, CCCoreLib::GenericIndexedCloudPersist>(m, "ccGenericPointCloud");

	py::class_<ccPointCloud, ccGenericPointCloud>(m, "ccPointCloud")
			// ScalarField management
			.def("getNumberOfScalarField", &ccPointCloud::getNumberOfScalarFields)
			.def("getScalarFieldName", &ccPointCloud::getScalarFieldName)
			.def("getScalarFieldIndexByName", &ccPointCloud::getScalarFieldIndexByName)
			.def("getCurrentDisplayedScalarField", &ccPointCloud::getCurrentDisplayedScalarField)
			.def("getCurrentDisplayedScalarFieldIndex", &ccPointCloud::getCurrentDisplayedScalarFieldIndex)
			.def("getScalarField", &ccPointCloud::getScalarField)
			.def("setCurrentDisplayedScalarField", &ccPointCloud::setCurrentDisplayedScalarField)
			.def("partialClone", [](const ccPointCloud *self, const CCCoreLib::ReferenceCloud *selection,
			                        ccPythonInstance *pythonInstance)
			{
				// TODO use opt param to check errs
				ccPointCloud *cloned = self->partialClone(selection);
				pythonInstance->addToDB(cloned);
				return cloned;
			}, py::return_value_policy::reference)
			.def("__repr__", [](const ccPointCloud &self)
			{
				return std::string("<ccPointCloud(") + self.getName().toStdString() + ", " +
				       std::to_string(self.size()) + " points)>";
			});

	py::class_<QProgressDialog>(m, "QProgressDialog");

	py::class_<ccProgressDialog, QProgressDialog, CCCoreLib::GenericProgressCallback>(m, "ccProgressDialog")
			.def(py::init<bool>(), "cancelButton"_a = false);

	py::class_<ccPythonInstance, observer_ptr < ccPythonInstance>>
	(m, "ccPythonInstance")
			.def("haveSelection", &ccPythonInstance::haveSelection)
			.def("haveOneSelection", &ccPythonInstance::haveOneSelection)
			.def("getSelectedEntities", &ccPythonInstance::getSelectedEntities, py::return_value_policy::reference)
			.def("setSelectedInDB", &ccPythonInstance::setSelectedInDB)
			.def("dbRootObject", &ccPythonInstance::dbRootObject, py::return_value_policy::reference)
			.def("addToDB", &ccPythonInstance::addToDB, "obj"_a, "updateZoom"_a= false, "autoExpandDBTree"_a=true, "checkDimensions"_a = false, "autoRedraw"_a = true)
			.def("redrawAll", &ccPythonInstance::redrawAll)
			.def("refreshAll", &ccPythonInstance::refreshAll)
			.def("enableAll", &ccPythonInstance::enableAll)
			.def("disableAll", &ccPythonInstance::disableAll)
			.def("updateUI", &ccPythonInstance::updateUI)
			.def("freezeUI", &ccPythonInstance::freezeUI)
			.def("loadFile", &ccPythonInstance::loadFile, py::return_value_policy::reference);


	m.def("GetInstance", &GetInstance);

	m.def("ProcessEvents", []()
	{
		QCoreApplication::processEvents();
	});


	m.def("RunInThread", [](py::object callable, py::args args, py::kwargs kwargs)
	{
		PyThreadStateReleaser stateReleaser{};
		QEventLoop loop;
		QFutureWatcher<py::object> watcher;
		QFuture<py::object> future = QtConcurrent::run(call_fn, stateReleaser.state, callable, args, kwargs);
		watcher.setFuture(future);
		QObject::connect(&watcher, &decltype(watcher)::finished, &loop, &decltype(loop)::quit);
		loop.exec();

		return future.result();
	});

	m.def("RunThread", [](py::object thread)
	{
		py::object isAliveMethod = thread.attr("is_alive");
		thread.attr("start")();
		while (isAliveMethod())
		{
			QCoreApplication::processEvents();
		}
	});

	py::class_<ccGlobalShiftManager> PyccGlobalShiftManager(m, "ccGlobalShiftManager");

	py::enum_<ccGlobalShiftManager::Mode> (PyccGlobalShiftManager, "Mode")
	        .value("NO_DIALOG", ccGlobalShiftManager::Mode::NO_DIALOG)
			.value("NO_DIALOG_AUTO_SHIFT", ccGlobalShiftManager::Mode::NO_DIALOG_AUTO_SHIFT)
			.value("DIALOG_IF_NECESSARY", ccGlobalShiftManager::Mode::DIALOG_IF_NECESSARY)
			.value("ALWAYS_DISPLAY_DIALOG", ccGlobalShiftManager::Mode::ALWAYS_DISPLAY_DIALOG)
			.export_values();


	py::class_<FileIOFilter> PyFileIOFilter (m, "FileIOFilter");
	PyFileIOFilter.def_static("LoadFromFile", [](const QString &filename,
	                                             FileIOFilter::LoadParameters &parameters) {
			CC_FILE_ERROR result = CC_FERR_NO_ERROR;
			ccHObject *newGroup = FileIOFilter::LoadFromFile(filename, parameters, result);
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
}