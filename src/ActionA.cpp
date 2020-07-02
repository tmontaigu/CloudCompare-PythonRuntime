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

#undef slots

#include <pybind11/embed.h>

#include "ccMainAppInterface.h"

#include "exposed.h"
#include <FileIOFilter.h>
#include <QWidget>
#include <Consoles.h>

#include "QPythonREPL.h"

namespace py = pybind11;

ccPythonInstance *s_pythonInstance{nullptr};

static void ThrowForFileError(CC_FILE_ERROR error)
{
	switch (error)
	{
		case CC_FERR_NO_ERROR:
			break;
		case CC_FERR_BAD_ARGUMENT:
			throw std::runtime_error("Bad argument");
		case CC_FERR_UNKNOWN_FILE:
			throw std::runtime_error("Unknown file");
		case CC_FERR_WRONG_FILE_TYPE:
			throw std::runtime_error("Wrong file type");
		case CC_FERR_WRITING:
			throw std::runtime_error("Error when writing");
		case CC_FERR_READING:
			throw std::runtime_error("Error when reading");
		case CC_FERR_NO_SAVE:
			throw std::runtime_error("Nothing to save");
		case CC_FERR_NO_LOAD:
			throw std::runtime_error("Nothing to load");
		case CC_FERR_BAD_ENTITY_TYPE:
			throw std::runtime_error("Bad entity type");
		case CC_FERR_CANCELED_BY_USER:
			throw std::runtime_error("Canceled by user");
		case CC_FERR_NOT_ENOUGH_MEMORY:
			throw std::runtime_error("Not enough memory");
		case CC_FERR_MALFORMED_FILE:
			throw std::runtime_error("Malformed File");
		case CC_FERR_CONSOLE_ERROR:
			throw std::runtime_error("The error has been logged in the console");
		case CC_FERR_BROKEN_DEPENDENCY_ERROR:
			throw std::runtime_error("Broken depencdency");
		case CC_FERR_FILE_WAS_WRITTEN_BY_UNKNOWN_PLUGIN:
			throw std::runtime_error("File was written by unknown plugin");
		case CC_FERR_THIRD_PARTY_LIB_FAILURE:
			throw std::runtime_error("Third party lib failure");
		case CC_FERR_THIRD_PARTY_LIB_EXCEPTION:
			throw std::runtime_error("Third party lib exception");
		case CC_FERR_NOT_IMPLEMENTED:
			throw std::runtime_error("Not implemented");
	}
}

ccPythonInstance *GetInstance()
{
	return s_pythonInstance;
}

void PrintMessage(const char *message)
{
	ccLog::Print(message);
}

void PrintWarning(const char *message)
{
	ccLog::Warning(message);
}

void PrintError(const char *message)
{
	ccLog::Error(message);
}


PYBIND11_EMBEDDED_MODULE(ccinternals, m)
{
	py::class_<ccConsoleOutput>(m, "ccConsoleOutput")
			.def(py::init<>())
			.def("write", &ccConsoleOutput::write);

	py::class_<QListWidget, std::unique_ptr<QListWidget, py::nodelete>>(m, "QListWidget");

	py::class_<ConsoleREPL>(m, "ConsoleREPL")
			.def(py::init<QListWidget *>())
			.def("write", &ConsoleREPL::write);
}

namespace Python
{
	void setMainAppInterfaceInstance(ccMainAppInterface *appInterface)
	{
		if (s_pythonInstance == nullptr)
		{
			s_pythonInstance = new ccPythonInstance(appInterface);
		}
	}

	void unsetMainAppInterfaceInstance()
	{
		if (s_pythonInstance != nullptr)
		{
			delete s_pythonInstance;
			s_pythonInstance = nullptr;
		}
	}
}

ccPythonInstance::ccPythonInstance(ccMainAppInterface *app) : m_app(app)
{
	if (m_app == nullptr)
	{
		throw std::invalid_argument("nullptr received for the app");
	}
}

ccHObject *ccPythonInstance::loadFile(const char *filename)
{
	CCVector3d loadCoordinatesShift(0, 0, 0);
	bool loadCoordinatesTransEnabled = false;
	FileIOFilter::LoadParameters parameters;
	parameters.alwaysDisplayLoadDialog = true;
	parameters.shiftHandlingMode = ccGlobalShiftManager::NO_DIALOG;
	parameters.coordinatesShift = &loadCoordinatesShift;
	parameters.coordinatesShiftEnabled = &loadCoordinatesTransEnabled;
	parameters.parentWidget = (QWidget *) m_app->getMainWindow();

	CC_FILE_ERROR result = CC_FERR_NO_ERROR;
	ccHObject *newGroup = FileIOFilter::LoadFromFile(filename, parameters, result);
	ThrowForFileError(result);

	if (newGroup)
	{
		m_app->addToDB(newGroup);
		m_app->refreshAll();
		m_app->updateUI();
	}
	return newGroup;
}


