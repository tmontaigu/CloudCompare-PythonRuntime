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

#include <QCoreApplication>
#include <QEventLoop>
#include <QtConcurrent>

#include <ccCommandLineInterface.h>
#include <ccMainAppInterface.h>

#include "Consoles.h"
#include "Runtime.h"
#include "Threading.h"
#include "ccGuiPythonInstance.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace Runtime
{

/// Interface that 'class-style' PythonPlugin must implement to be loaded
class PythonPluginInterface
{
  public:
    virtual ~PythonPluginInterface(){};
    /// Called automatically after the plugin is instantiated.
    /// In this function, the plugin implementer should register actions
    /// it wishes to expose to the user.
    virtual std::vector<Runtime::RegisteredPlugin::Action> getActions() = 0;
};

// Trampoline to allow Python classes to inherit our interface
class PythonPluginTrampoline : public PythonPluginInterface
{
  public:
    PythonPluginTrampoline() : PythonPluginInterface() {}

    std::vector<Runtime::RegisteredPlugin::Action> getActions() override
    {
        PYBIND11_OVERLOAD_PURE(std::vector<Runtime::RegisteredPlugin::Action>,
                               PythonPluginInterface /* Parent class */,
                               registerActions /* function name */);
    }
};

static ccGuiPythonInstance *s_pythonInstance{nullptr};

static ccCommandLineInterface *s_cmdLineInstance{nullptr};

ccGuiPythonInstance *GetInstance() noexcept
{
    return s_pythonInstance;
}

ccCommandLineInterface *GetCmdLineInstance() noexcept
{
    return s_cmdLineInstance;
}

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

} // namespace Runtime

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

    py::class_<ListWidgetConsole>(m, "ListWidgetConsole")
        .def(py::init<QListWidget *, Qt::GlobalColor>())
        .def(py::init<QListWidget *, QColor>())
        .def(py::init<QListWidget *>())
        .def("write", &ListWidgetConsole::write)
        .def("flush", &ListWidgetConsole::flush);
}

void define_ccGUIPythonInstance(py::module &);
void define_ccCommandLine(py::module &);

PYBIND11_EMBEDDED_MODULE(pycc_runtime, m)
{
    define_ccGUIPythonInstance(m);
    define_ccCommandLine(m);

    py::class_<Runtime::PythonPluginInterface, Runtime::PythonPluginTrampoline>(
        m, "PythonPluginInterface")
        .def(py::init<>())
        .def("getActions", &Runtime::PythonPluginInterface::getActions);

    m.def("ProcessEvents", []() { QCoreApplication::processEvents(); });

    m.def(
        "RunInThread",
        [](py::object callable, py::args args, py::kwargs kwargs) {
            PyThreadStateReleaser stateReleaser{};
            QEventLoop loop;
            QFutureWatcher<py::object> watcher;
            QObject::connect(&watcher, &decltype(watcher)::finished, &loop, &decltype(loop)::quit);
            QFuture<py::object> future =
                QtConcurrent::run(call_fn, stateReleaser.state, callable, args, kwargs);
            watcher.setFuture(future);
            loop.exec();

            return future.result();
        },
        "callable"_a);

    m.def("RunThread", [](py::object thread) {
        py::object isAliveMethod = thread.attr("is_alive");
        thread.attr("start")();
        while (isAliveMethod())
        {
            QCoreApplication::processEvents();
        }
    });

    // Use leading __ to give hints that user should not import this in their scripts
    m.def("GetGUIInstance", &Runtime::GetInstance, py::return_value_policy::reference);
    m.def("GetCmdLineInstance", &Runtime::GetCmdLineInstance, py::return_value_policy::reference);
    m.def(
        "GetInstance",
        []() -> py::object {
            auto guiInstance = Runtime::GetInstance();
            if (guiInstance)
            {
                return py::cast(guiInstance);
            }
            else
            {
                return py::cast(Runtime::GetCmdLineInstance());
            }
        },
        py::return_value_policy::reference);

    py::class_<Runtime::RegisteredPlugin::Action>(m, "Action")
        .def(py::init<QString, py::object>(), "name"_a, "target"_a);
}
