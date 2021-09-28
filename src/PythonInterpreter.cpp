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
#include "Runtime/Consoles.h"
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

static py::dict CreateGlobals()
{
    py::dict globals;
    globals["__name__"] = "__main__";
    // Someday we should require pybind11 > 2.6 and use py::detail::ensure_builtins_in_globals ?
    globals["__builtins__"] = PyEval_GetBuiltins();
    return globals;
}

PythonInterpreter::State::State() : globals(CreateGlobals()), locals(){};

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
        const auto movePolicy = py::return_value_policy::move;
        py::object newStdout = py::cast(ccConsoleOutput(), movePolicy);
        py::object newStderr = py::cast(ccConsoleOutput(), movePolicy);
        PyStdErrOutStreamRedirect r{newStdout, newStderr};

        py::dict globals = CreateGlobals();
        globals["__file__"] = filepath;
        py::eval_file(filepath, globals);
    }
    catch (const std::exception &e)
    {
        ccLog::Warning(e.what());
        success = false;
    }

    Q_EMIT executionFinished();
    return success;
}

template <pybind11::eval_mode mode>
void PythonInterpreter::executeCodeString(const std::string &code,
                                          QListWidget *output,
                                          State &state)
{
    if (m_isExecuting)
    {
        return;
    }

    Q_EMIT executionStarted();
    m_isExecuting = true;
    const QColor orange(255, 100, 0);

    try
    {
        if (output != nullptr)
        {
            const auto movePolicy = py::return_value_policy::move;
            py::object newStdout = py::cast(ListWidgetConsole(output), movePolicy);
            py::object newStderr = py::cast(ListWidgetConsole(output, orange), movePolicy);
            PyStdErrOutStreamRedirect redirect{newStdout, newStderr};
            py::eval<mode>(code, state.globals, state.locals);
        }
        else
        {
            PyStdErrOutStreamRedirect redirect;
            py::eval<mode>(code, state.globals, state.locals);
        }
    }
    catch (const std::exception &e)
    {
        if (output)
        {
            auto message = new QListWidgetItem(e.what());
            message->setForeground(Qt::red);
            output->addItem(message);
        }
        else
        {
            ccLog::Error(e.what());
        }
    }

    m_isExecuting = false;
    Q_EMIT executionFinished();
}

void PythonInterpreter::executeCodeWithState(const std::string &code,
                                             QListWidget *output,
                                             State &state)
{
    executeCodeString<py::eval_mode::eval_statements>(code, output, state);
}

void PythonInterpreter::executeStatementWithState(const std::string &code,
                                                  QListWidget *output,
                                                  State &state)
{
    executeCodeString<py::eval_mode::eval_single_statement>(code, output, state);
}

void PythonInterpreter::executeCode(const std::string &code, QListWidget *output)
{
    State tmpState;
    executeCodeWithState(code, output, tmpState);
}

void PythonInterpreter::executeFunction(const pybind11::object &function)
{
    if (m_isExecuting)
    {
        return;
    }

    m_isExecuting = true;
    Q_EMIT executionStarted();
    try
    {
        py::gil_scoped_acquire scopedGil;
        PyStdErrOutStreamRedirect scopedRedirect;
        function();
    }
    catch (const std::exception &e)
    {
        ccLog::Error("Failed to start Python actions: %s", e.what());
    }
    m_isExecuting = false;
    Q_EMIT executionFinished();
}

void PythonInterpreter::initialize(const PythonConfig &config)
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

    const auto displaydlopenError = []() {
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
    // Make sure this module is imported
    // so that we can later easily construct our consoles.
    py::module::import("ccinternals");
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
