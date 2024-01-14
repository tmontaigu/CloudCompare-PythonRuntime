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

#include <utility>

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
    py::object m_stdout;
    py::object m_stderr;
    py::object m_stdout_buffer;
    py::object m_stderr_buffer;

  public:
    /// Default constructor
    ///
    /// Will redirect stdout & stderr to be written in CloudCompare's own console
    PyStdErrOutStreamRedirect()
    {
        auto sysm = py::module::import("sys");
        m_stdout = sysm.attr("stdout");
        m_stderr = sysm.attr("stderr");
        auto ccConsoleOutput = py::module::import("ccinternals").attr("ccConsoleOutput");
        m_stdout_buffer = ccConsoleOutput();
        m_stderr_buffer = ccConsoleOutput();
        sysm.attr("stdout") = m_stdout_buffer;
        sysm.attr("stderr") = m_stderr_buffer;
    }

    /// Creates a stream redirection
    ///
    /// stdout_obj & stderr_obj must have a 'file-object' interface
    /// and provide at least a `write(string_message)` and `flush()`
    /// methods.
    ///
    /// \param stdout_obj python object to redirect *stdout* to
    /// \param stderr_obj python object to redirect *stderr* to
    PyStdErrOutStreamRedirect(py::object stdout_obj, py::object stderr_obj)
    {
        auto sysm = py::module::import("sys");
        m_stdout = sysm.attr("stdout");
        m_stderr = sysm.attr("stderr");
        m_stdout_buffer = std::move(stdout_obj);
        m_stderr_buffer = std::move(stderr_obj);
        sysm.attr("stdout") = m_stdout_buffer;
        sysm.attr("stderr") = m_stderr_buffer;
    }

    ~PyStdErrOutStreamRedirect() noexcept
    {
        try
        {
            const auto sysm = py::module::import("sys");
            sysm.attr("stdout") = m_stdout;
            sysm.attr("stderr") = m_stderr;
        }
        catch (const std::exception &)
        {
        }
    }
};

#endif // PYTHON_PLUGIN_PYTHON_STDERR_OUT_REDIRECT_H
