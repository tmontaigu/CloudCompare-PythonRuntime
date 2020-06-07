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


#ifndef CLOUDCOMPAREPROJECTS_PYTHONSTDERROUTREDIRECT_H
#define CLOUDCOMPAREPROJECTS_PYTHONSTDERROUTREDIRECT_H

#include <pybind11/pybind11.h>

namespace py = pybind11;

// https://github.com/pybind/pybind11/issues/1622
class PyStdErrOutStreamRedirect {
    py::object m_stdout;
    py::object m_stderr;
    py::object m_stdout_buffer;
    py::object m_stderr_buffer;
public:
    PyStdErrOutStreamRedirect() {
        auto sysm = py::module::import("sys");
        m_stdout = sysm.attr("stdout");
        m_stderr = sysm.attr("stderr");
        auto ccConsoleOutput = py::module::import("ccinternals").attr("ccConsoleOutput");
        m_stdout_buffer = ccConsoleOutput();
        m_stderr_buffer = ccConsoleOutput();
        sysm.attr("stdout") = m_stdout_buffer;
        sysm.attr("stderr") = m_stderr_buffer;
    }

    ~PyStdErrOutStreamRedirect() {
        auto sysm = py::module::import("sys");
        sysm.attr("stdout") = m_stdout;
        sysm.attr("stderr") = m_stderr;
    }
};

#endif //CLOUDCOMPAREPROJECTS_PYTHONSTDERROUTREDIRECT_H
