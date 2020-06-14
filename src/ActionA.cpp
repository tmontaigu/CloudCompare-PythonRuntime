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
#include "PythonStdErrOutRedirect.h"
#include "exposed.h"
#include <FileIOFilter.h>
#include <QWidget>

namespace py = pybind11;

static ccMainAppInterface *s_appInterface{nullptr};

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

ccHObject::Container GetSelectedEntities()
{
	if (s_appInterface == nullptr)
	{
		throw std::runtime_error("missing MainAppInterface, is the Python Plugin running ?");
	} else
	{
		return s_appInterface->getSelectedEntities();
	}
}

void RemoveFromDB(ccHObject *obj)
{
	if (s_appInterface == nullptr)
	{
		throw std::runtime_error("missing MainAppInterface, is the Python Plugin running ?");
	} else
	{
		s_appInterface->removeFromDB(obj, false);
	}
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

ccHObject *GetDBRoot()
{
	if (s_appInterface)
	{
		return s_appInterface->dbRootObject();
	} else
	{
		throw std::runtime_error("missing MainAppInterface, is the Python Plugin running ?");
	}
}

void SetSelectedInDB(ccHObject *obj, bool selected)
{
	if (s_appInterface)
	{
		s_appInterface->setSelectedInDB(obj, selected);
	} else
	{
		throw std::runtime_error("missing MainAppInterface, is the Python Plugin running ?");
	}
}

ccHObject *LoadFile(const char *filename)
{
	CCVector3d loadCoordinatesShift(0, 0, 0);
	bool loadCoordinatesTransEnabled = false;
	FileIOFilter::LoadParameters parameters;
	parameters.alwaysDisplayLoadDialog = true;
	parameters.shiftHandlingMode = ccGlobalShiftManager::NO_DIALOG;
	parameters.coordinatesShift = &loadCoordinatesShift;
	parameters.coordinatesShiftEnabled = &loadCoordinatesTransEnabled;
	ccLog::Warning("%f %f %f", loadCoordinatesShift.x, loadCoordinatesShift.y, loadCoordinatesShift.z);
	parameters.parentWidget = (QWidget *) s_appInterface->getMainWindow();

	CC_FILE_ERROR result = CC_FERR_NO_ERROR;
	ccHObject *newGroup = FileIOFilter::LoadFromFile(filename, parameters, result);
	ThrowForFileError(result);

	if (newGroup)
	{
		s_appInterface->addToDB(newGroup);
	}
	return newGroup;
}


/// Class implementing 'write' to be able to act like
/// a Python file object in order to be able to
/// output messages from Python's print to the CloudCompare console
/// instead of stdout & stderr
class ccConsoleOutput
{
public:
	ccConsoleOutput() = default;

	void write(const char *messagePart)
	{
		if (s_appInterface == nullptr)
		{
			return;
		}

		size_t len = strlen(messagePart);
		const char* messageEnd = messagePart + len;
		const char* start = messagePart;

		const char *newLinePos;
		while((newLinePos = strchr(start, '\n')) != nullptr)
		{
			m_currentMessage += QString::fromUtf8(start, static_cast<int>(newLinePos - start));
			s_appInterface->dispToConsole(m_currentMessage);
			m_currentMessage.clear();
			start = newLinePos + 1;
		}

		if (start != messageEnd)
		{
			m_currentMessage += QString::fromUtf8(start, static_cast<int>(messageEnd - start));
		}
	}

private:
	QString m_currentMessage{};
};

PYBIND11_EMBEDDED_MODULE(ccinternals, m)
{
	py::class_<ccConsoleOutput>(m, "ccConsoleOutput")
			.def(py::init<>())
			.def("write", &ccConsoleOutput::write);
}

namespace Python
{
	// This is an example of an action's method called when the corresponding action
	// is triggered (i.e. the corresponding icon or menu entry is clicked in CC's
	// main interface). You can access most of CC's components (database,
	// 3D views, console, etc.) via the 'appInterface' variable.
	void performActionA(ccMainAppInterface *appInterface)
	{
		if (appInterface == nullptr)
		{
			// The application interface should have already been initialized when the plugin is loaded
			Q_ASSERT(false);
			return;
		}

		if (s_appInterface == nullptr)
		{
			s_appInterface = appInterface;
		}

		try
		{
			PyStdErrOutStreamRedirect redirect{};
			py::eval_file("script.py");
		} catch (const std::exception &e)
		{
			appInterface->dispToConsole(QString("[Python] %1").arg(e.what()), ccMainAppInterface::ERR_CONSOLE_MESSAGE);
		}
	}
}
