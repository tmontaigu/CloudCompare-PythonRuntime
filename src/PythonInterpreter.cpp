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

#include "PythonInterpreter.h"
#include "PythonStdErrOutRedirect.h"
#include "Utilities.h"

#include <pybind11/embed.h>

#include <QApplication>
#include <QDir>
#include <QListWidgetItem>
#include <QTextCodec>

#include <ccLog.h>

#ifdef Q_OS_UNIX
#include <cstdio>
#include <dlfcn.h>
#endif

// seems like gcc defines macro with these names
#undef major
#undef minor

namespace py = pybind11;



//================================================================================

PythonInterpreter::PythonInterpreter(QObject *parent) : QObject(parent) {}

bool PythonInterpreter::executeFile(const std::string &filepath)
{
    if (m_isExecuting)
    {
        return false;
    }
    Q_EMIT executionStarted();

    bool success{true};
    try
    {
        py::object pyStdout =
            py::module::import("ccinternals").attr("ccConsoleOutput")("[PythonStdout] ");
        py::object pyStderr =
            py::module::import("ccinternals").attr("ccConsoleOutput")("[PythonStderr] ");
        PyStdErrOutStreamRedirect r{pyStdout, pyStderr};
        py::eval_file(filepath);
    }
    catch (const std::exception &e)
    {
        ccLog::Warning(e.what());
        success = false;
    }

    Q_EMIT executionFinished();
    return success;
}

void PythonInterpreter::executeCodeWithState(const std::string &code,
                                             QListWidget *output,
                                             State &state)
{
    Q_ASSERT(output != nullptr);
    if (m_isExecuting)
    {
        return;
    }

    Q_EMIT executionStarted();
    m_isExecuting = true;
    const QColor orange(255, 100, 0);

    try
    {
        py::object newStdout = py::module::import("ccinternals").attr("ConsoleREPL")(output);
        py::object newStderr =
            py::module::import("ccinternals").attr("ConsoleREPL")(output, orange);
        PyStdErrOutStreamRedirect redirect{newStdout, newStderr};
        py::exec(code);
    }
    catch (const std::exception &e)
    {
        auto message = new QListWidgetItem(e.what());
        message->setForeground(Qt::red);
        output->addItem(message);
    }

    m_isExecuting = false;
    Q_EMIT executionFinished();
}

void PythonInterpreter::executeCode(const std::string &code, QListWidget *output)
{
    State tmpState;
    executeCodeWithState(code, output, tmpState);
}

void PythonInterpreter::initialize(const PythonConfig& config)
{
#ifdef Q_OS_WIN32
    m_config = config.pythonCompatiblePaths();
    Q_ASSERT(m_config.isSet());
#if PY_MINOR_VERSION == 6
    Py_SetPythonHome(const_cast<wchar_t *>(m_config.pythonHome()));
#else
    Py_SetPythonHome(m_config.pythonHome());
#endif
    Py_SetPath(m_config.pythonPath());
#endif
#ifdef Q_OS_UNIX
    // Work-around issue: undefined symbol: PyExc_RecursionError
    // when trying to import numpy in the intepreter
    // e.g: https://github.com/numpy/numpy/issues/14946
    // https://stackoverflow.com/questions/49784583/numpy-import-fails-on-multiarray-extension-library-when-called-from-embedded-pyt
    // This workaround is weak

    const auto displaydlopenError = []()
    {
        char *error = dlerror();
        if (error)
        {
            ccLog::Warning("[PythonPlugin] dlopen error:", error);
        }
    };

    char soName[25];
    snprintf(soName, 24, "libpython%d.%d.so", PythonVersion.major, PythonVersion.minor);
    m_libPythonHandle = dlopen(soName, RTLD_LAZY | RTLD_GLOBAL);
    if (!m_libPythonHandle)
    {
        displaydlopenError();
        snprintf(soName, 24, "libpython%d.%dm.so", PythonVersion.major, PythonVersion.minor);
        m_libPythonHandle = dlopen(soName, RTLD_LAZY | RTLD_GLOBAL);
        if (!m_libPythonHandle)
        {
            displaydlopenError();
        }
    }
#endif
    py::initialize_interpreter();
}

bool PythonInterpreter::IsInitialized()
{
    return Py_IsInitialized();
}

void PythonInterpreter::finalize()
{
    if (Py_IsInitialized())
    {
        py::finalize_interpreter();
#ifdef Q_OS_UNIX
        if (m_libPythonHandle)
        {
            dlclose(m_libPythonHandle);
            m_libPythonHandle = nullptr;
        }
#else
        Q_UNUSED(this);
#endif
    }
}


bool PythonInterpreter::isExecuting() const
{
    return m_isExecuting;
}

const PythonConfigPaths &PythonInterpreter::config() const
{
    return m_config;
}