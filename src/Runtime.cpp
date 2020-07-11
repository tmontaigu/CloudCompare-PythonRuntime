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

#include "ccGUIPythonInstance.h"
#include "ccMainAppInterface.h"


#include "Runtime.h"

#include <QWidget>
#include <Consoles.h>

#include "QPythonREPL.h"

class ccCommandLineInterface;

namespace py = pybind11;

ccGUIPythonInstance *s_pythonInstance{nullptr};
ccCommandLineInterface *s_cmdLineInstance{nullptr};


ccGUIPythonInstance *GetInstance() {
	return s_pythonInstance;
}

ccCommandLineInterface *GetCmdLineInstance() {
	return s_cmdLineInstance;
}

namespace Python {
	void setMainAppInterfaceInstance(ccMainAppInterface *appInterface) {
		if ( s_pythonInstance == nullptr )
		{
			s_pythonInstance = new ccGUIPythonInstance(appInterface);
		}
	}

	void unsetMainAppInterfaceInstance() {
		if ( s_pythonInstance != nullptr )
		{
			delete s_pythonInstance;
			s_pythonInstance = nullptr;
		}
	}

	void setCmdLineInterfaceInstance(ccCommandLineInterface *cmdLine) {
		if ( s_cmdLineInstance == nullptr )
		{
			s_cmdLineInstance = cmdLine;
		}
	}

	void unsetCmdLineInterfaceInstance() {
		if ( s_cmdLineInstance != nullptr )
		{
			s_cmdLineInstance = nullptr;
		}
	}
}

PYBIND11_EMBEDDED_MODULE(ccinternals, m) {
	py::class_<ccConsoleOutput>(m, "ccConsoleOutput")
			.def(py::init<>())
			.def("write", &ccConsoleOutput::write);

	py::class_<QListWidget, std::unique_ptr<QListWidget, py::nodelete>>(m, "QListWidget");

	py::class_<ConsoleREPL>(m, "ConsoleREPL")
			.def(py::init<QListWidget *>())
			.def("write", &ConsoleREPL::write);
}