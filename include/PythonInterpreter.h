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

#ifndef CLOUDCOMPAREPROJECTS_PYTHONINTERPRETER_H
#define CLOUDCOMPAREPROJECTS_PYTHONINTERPRETER_H

#include <QObject>

#include <memory>

#undef slots
#include <pybind11/pybind11.h>

class QListWidget;
struct PyVenvCfg;

/// Holds strings of the PythonHome & PythonPath
/// Which are variables that needs to be properly set
/// in order to have a functioning Python environment
class PythonConfigPaths
{
  public:
    /// Default ctor, does not initialize pythonHome & pythonPath
    PythonConfigPaths() = default;

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

class PythonInterpreter : public QObject
{
    Q_OBJECT

  public:
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
    static bool isInitialized();
    static void finalize();

    bool executeFile(const std::string& filePath);

  public Q_SLOTS:
    void executeCode(const std::string &code, QListWidget *output);
    void executeCodeWithState(const std::string &code, QListWidget *output, State &state);

  Q_SIGNALS:
    void executionStarted();
    void executionFinished();

  private:
    void configureEnvironment();

  private:
    bool m_isExecuting{false};

    PythonConfigPaths m_config;
};

#endif // CLOUDCOMPAREPROJECTS_PYTHONINTERPRETER_H
