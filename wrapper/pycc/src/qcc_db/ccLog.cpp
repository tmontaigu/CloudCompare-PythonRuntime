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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include "../casters.h"

#include <ccLog.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccLog(py::module &m)
{
    py::class_<ccLog> PyccLog(m, "ccLog", R"pbdoc(
    Class to log messages in CloudCompare's console.

    Use one of the static method to log a message
)pbdoc");
    PyccLog.def_static("TheInstance", &ccLog::TheInstance, py::return_value_policy::reference);
    PyccLog.def_static("LogMessage", &ccLog::LogMessage, "message"_a, "level"_a, R"pbdoc(
    Logs a message with the given level.

    Parameters
    ----------
    message: str
        The message to log
    level: pycc.ccLog.MessageLevelFlags
        The severity level of the message

    Example
    -------

    >>> import pycc
    >>> pycc.ccLog.LogMessage("Hello, world", pycc.ccLog.MessageLevelFlags.LOG_STANDARD)
)pbdoc");
    PyccLog.def_static(
        "Print", static_cast<bool (*)(const QString &)>(&ccLog::Print), "message"_a, R"pbdoc(
    Logs a message with standard severity level.

    Parameters
    ----------
    message: str
        The message to log

    Example
    -------

    >>> import pycc
    >>> pycc.ccLog.Print("Hello, world")
    True
)pbdoc");
    PyccLog.def_static(
        "Warning", static_cast<bool (*)(const QString &)>(&ccLog::Warning), "message"_a, R"pbdoc(
    Logs a warning message

    Parameters
    ----------
    message: str
        The message to log

    Example
    -------

    >>> import pycc
    >>> pycc.ccLog.Warning("Oops something bad happenned")
    False
)pbdoc");
    PyccLog.def_static(
        "Error", static_cast<bool (*)(const QString &)>(&ccLog::Error), "message"_a, R"pbdoc(
    Logs an error message

    This will also display a dialog

    Parameters
    ----------
    message: str
        The message to log

    Example
    -------

    >>> import pycc
    >>> pycc.ccLog.Error("Oops something even worse happenned")
    False
)pbdoc");

    py::enum_<ccLog::MessageLevelFlags>(PyccLog, "MessageLevelFlags")
        .value("LOG_STANDARD", ccLog::MessageLevelFlags::LOG_STANDARD)
        .value("LOG_DEBUG", ccLog::MessageLevelFlags::LOG_DEBUG)
        .value("LOG_WARNING", ccLog::MessageLevelFlags::LOG_WARNING)
        .value("LOG_ERROR", ccLog::MessageLevelFlags::LOG_ERROR);
}
