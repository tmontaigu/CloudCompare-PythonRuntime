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

#ifndef PYTHON_PLUGIN_PYTHON_INTERPRETER_H
#define PYTHON_PLUGIN_PYTHON_INTERPRETER_H

#include <QObject>

#include <memory>

#undef slots
#include <pybind11/pybind11.h>

class QListWidget;
struct PyVenvCfg;

/// Holds strings of the PythonHome & PythonPath
/// Which are variables that needs to be properly set
/// in order to have a functioning Python environment
class PythonConfigPaths final
{
  public:
    /// Default ctor, does not initialize pythonHome & pythonPath
    PythonConfigPaths() = default;

#ifdef Q_OS_WIN32
    /// Initialize the paths to point to where the Python
    /// environment was bundled on a Windows installation
    static PythonConfigPaths WindowsBundled();

    /// Initialize the paths to use the conda environment stored at condaPrefix
    static PythonConfigPaths WindowsCondaEnv(const char *condaPrefix);

    /// Initialize the paths to use the python venv stored at venvPrefix.
    ///
    /// \param venvPrefix Path on disk to the root of the venv
    /// \param cfg options that were read from the pyvenv.cfg file
    /// \return the configured paths
    static PythonConfigPaths WindowsVenv(const char *venvPrefix, const PyVenvCfg &cfg);
#endif
    /// returns true if both paths are non empty
    bool isSet() const;

    /// Returns the pythonHome
    const wchar_t *pythonHome() const;

    /// Returns the pythonPath
    const wchar_t *pythonPath() const;

  private:
    std::unique_ptr<wchar_t[]> m_pythonHome{};
    std::unique_ptr<wchar_t[]> m_pythonPath{};
};

/// PythonInterpreter, There should be only one, and it is managed by the PythonPlugin.
/// It centralizes the execution of python scripts.
///
/// Only one script can be executed at a time.
/// Classes that allow user to start the execution of a script should connect the
/// executionStarted & executionFinished to give feedback on
/// when a script is running and whether a new script can be ran
class PythonInterpreter final : public QObject
{
    Q_OBJECT

  public:
    /// Variable state
    struct State
    {
        State() : globals(pybind11::globals()), locals(){};
        pybind11::dict globals;
        pybind11::dict locals;
    };

  public:
    explicit PythonInterpreter(QObject *parent = nullptr);
    bool isExecuting() const;
    void initialize();
    void finalize();
    static bool IsInitialized();
    const PythonConfigPaths &config() const;

    /// Execution functions (and slots)
  public Q_SLOTS:
    bool executeFile(const std::string &filePath);
    void executeCode(const std::string &code, QListWidget *output);
    void executeCodeWithState(const std::string &code, QListWidget *output, State &state);

  Q_SIGNALS:
    void executionStarted();
    void executionFinished();

#ifdef Q_OS_WIN32
  private:
    void configureEnvironment();
#endif

  private:
    bool m_isExecuting{false};

    PythonConfigPaths m_config;

#ifdef Q_OS_UNIX
    void *m_libPythonHandle{nullptr};
#endif
};

#endif // PYTHON_PLUGIN_PYTHON_INTERPRETER_H
