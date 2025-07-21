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

#ifndef PYTHON_PLUGIN_PYTHON_STDERR_OUT_REDIRECT_H
#define PYTHON_PLUGIN_PYTHON_STDERR_OUT_REDIRECT_H

#include <pybind11/pybind11.h>

#include "Utilities.h"
#include "ccLog.h"

namespace py = pybind11;

/// This class redirects python's stdout and stderr
/// to output of your choice.
///
/// # Example
/// ```cpp
/// {
///     PyStdErrOutStreamRedirect redirect{};
///     // python's print are redirected
/// }
/// // python's print are no longer redirected
/// ```
// largely taken from https://github.com/pybind/pybind11/issues/1622
class PyStdErrOutStreamRedirect
{
    py::object m_savedStdout;
    py::object m_savedStderr;

  public:
    /// Default constructor
    ///
    /// Will redirect stdout & stderr to be written in CloudCompare's own console
    PyStdErrOutStreamRedirect()
    {
        auto sysm = py::module::import("sys");
        m_savedStdout = sysm.attr("stdout");
        m_savedStderr = sysm.attr("stderr");
        auto ccConsoleOutput = py::module::import("ccinternals").attr("ccConsoleOutput");
        sysm.attr("stdout") = ccConsoleOutput();
        sysm.attr("stderr") = ccConsoleOutput();
    }

    /// Creates a stream redirection
    ///
    /// stdout_obj & stderr_obj must have a 'file-object' interface
    /// and provide at least a `write(string_message)` and `flush()`
    /// methods.
    ///
    /// \param stdout_obj python object to redirect *stdout* to
    /// \param stderr_obj python object to redirect *stderr* to
    PyStdErrOutStreamRedirect(py::object &&stdout_obj, py::object &&stderr_obj)
    {
        auto sysm = py::module::import("sys");
        m_savedStdout = sysm.attr("stdout");
        m_savedStderr = sysm.attr("stderr");

        sysm.attr("stdout") = stdout_obj;
        sysm.attr("stderr") = stderr_obj;
    }

    ~PyStdErrOutStreamRedirect() noexcept
    {
        try
        {
            const auto sysm = py::module::import("sys");
            sysm.attr("stdout") = m_savedStdout;
            sysm.attr("stderr") = m_savedStderr;
        }
        catch (const std::exception &)
        {
            plgWarning() << "PyStdErrOutStreamRedirect: failed to restore sys.stdout & sys.stderr";
        }
    }
};

#endif // PYTHON_PLUGIN_PYTHON_STDERR_OUT_REDIRECT_H
