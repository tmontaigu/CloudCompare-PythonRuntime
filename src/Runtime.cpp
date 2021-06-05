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

#undef slots

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>

#include "ccGuiPythonInstance.h"
#include "ccMainAppInterface.h"

#include "Runtime.h"

#include <Consoles.h>
#include <PrivateRuntime.h>

class ccCommandLineInterface;

namespace py = pybind11;
using namespace pybind11::literals;

ccGuiPythonInstance *s_pythonInstance{nullptr};

ccCommandLineInterface *s_cmdLineInstance{nullptr};

ccGuiPythonInstance *GetInstance() noexcept
{
    return s_pythonInstance;
}

ccCommandLineInterface *GetCmdLineInstance() noexcept
{
    return s_cmdLineInstance;
}

namespace Python
{
void setMainAppInterfaceInstance(ccMainAppInterface *appInterface) noexcept(false)
{
    if (s_pythonInstance == nullptr)
    {
        s_pythonInstance = new ccGuiPythonInstance(appInterface);
    }
}

void unsetMainAppInterfaceInstance() noexcept
{
    if (s_pythonInstance != nullptr)
    {
        delete s_pythonInstance;
        s_pythonInstance = nullptr;
    }
}

void setCmdLineInterfaceInstance(ccCommandLineInterface *cmdLine) noexcept
{
    if (s_cmdLineInstance == nullptr)
    {
        s_cmdLineInstance = cmdLine;
    }
}

void unsetCmdLineInterfaceInstance() noexcept
{
    if (s_cmdLineInstance != nullptr)
    {
        s_cmdLineInstance = nullptr;
    }
}

} // namespace Python

PYBIND11_EMBEDDED_MODULE(ccinternals, m)
{
    py::enum_<Qt::GlobalColor>(m, "GlobalColor");

    py::class_<QColor>(m, "QColor");

    py::class_<QListWidget, std::unique_ptr<QListWidget, py::nodelete>>(m, "QListWidget");

    py::class_<ccConsoleOutput>(m, "ccConsoleOutput")
        .def(py::init<>())
        .def(py::init<const char *>(), "prefix"_a)
        .def("write", &ccConsoleOutput::write)
        .def("flush", &ccConsoleOutput::flush);

    py::class_<ConsoleRepl>(m, "ConsoleREPL")
        .def(py::init<QListWidget *, Qt::GlobalColor>())
        .def(py::init<QListWidget *, QColor>())
        .def(py::init<QListWidget *>())
        .def("write", &ConsoleRepl::write)
        .def("flush", &ConsoleRepl::flush);
}