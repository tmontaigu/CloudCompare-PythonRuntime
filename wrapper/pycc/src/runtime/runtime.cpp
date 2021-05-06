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

#include <ccCommandLineInterface.h>

#include <QtConcurrent>

#undef slots
#include <pybind11/pybind11.h>

#include "Runtime.h"
#include "threading.h"
#include "ccGUIPythonInstance.h"


namespace py = pybind11;
using namespace pybind11::literals;


void define_ccGUIPythonInstance(py::module &);
void define_ccCommandLine(py::module &);


void define_pycc_runtime(py::module& m) {
    define_ccGUIPythonInstance(m);
    define_ccCommandLine(m);


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
}