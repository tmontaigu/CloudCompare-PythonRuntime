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

#include "PythonInterpreter.h"
#include "PythonStdErrOutRedirect.h"
#include "Runtime/Consoles.h"
#include "Utilities.h"

#include <pybind11/embed.h>

#include <QApplication>
#include <QDir>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QTextCodec>

#include <ccLog.h>

#ifdef Q_OS_LINUX
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

    plgDebug() << "Initializing the interpreter with: " << config;

#ifdef Q_OS_LINUX
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
            plgWarning() << "dlopen error: " << error;
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
    if (config.type() != PythonConfig::Type::System)
    {
        // We use PEP 0587 to init the interpreter.
        // The changes introduced in this PEP allows to handle the error
        // when the interpreter could not be initialized.
        //
        // Before that the python interpreter would simply exit the program
        // and that could be a bad user experience
        //
        // https://www.python.org/dev/peps/pep-0587/
        // https://docs.python.org/3/c-api/init_config.html#init-python-config
        m_config = config.pythonCompatiblePaths();
        PyStatus status;

        PyConfig pyConfig;
        PyConfig_InitPythonConfig(&pyConfig);
        pyConfig.isolated = 1;

        status = PyConfig_SetString(&pyConfig, &pyConfig.home, m_config.pythonHome());
        if (PyStatus_Exception(status))
        {
            PyConfig_Clear(&pyConfig);
            throw std::runtime_error(status.err_msg);
        }

        status = PyConfig_SetString(&pyConfig, &pyConfig.pythonpath_env, m_config.pythonPath());
        if (PyStatus_Exception(status))
        {
            PyConfig_Clear(&pyConfig);
            throw std::runtime_error(status.err_msg);
        }

        status = PyConfig_Read(&pyConfig);
        if (PyStatus_Exception(status))
        {
            PyConfig_Clear(&pyConfig);
            throw std::runtime_error(status.err_msg);
        }

        status = Py_InitializeFromConfig(&pyConfig);
        if (PyStatus_Exception(status))
        {
            PyConfig_Clear(&pyConfig);
            throw std::runtime_error(status.err_msg);
        }
    }
    else
    {
        Py_Initialize();
    }

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
#ifdef Q_OS_LINUX
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
