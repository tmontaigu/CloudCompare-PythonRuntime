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

#ifndef PYTHON_PLUGIN_PYTHON_INTERPRETER_H
#define PYTHON_PLUGIN_PYTHON_INTERPRETER_H

#include "PythonConfig.h"
#include <QObject>

#include <memory>

#undef slots
#include <pybind11/eval.h>
#include <pybind11/pybind11.h>

class QListWidget;
class PythonConfig;

/// PythonInterpreter.
///
/// There should be only one, and it is managed by the PythonPlugin.
/// It centralizes the execution of python scripts.
///
/// Only one script can be executed at a time.
///
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
        State();
        pybind11::dict globals;
        pybind11::object locals;
    };

  public:
    explicit PythonInterpreter(QObject *parent = nullptr);
    bool isExecuting() const;
    void initialize(const PythonConfig &config);
    void finalize();
    static bool IsInitialized();
    const PythonConfigPaths &config() const;

    /// Execution functions (and slots)
  public Q_SLOTS:
    bool executeFile(const std::string &filePath);
    void executeCode(const std::string &code, QListWidget *output);
    void executeCodeWithState(const std::string &code,
                              QListWidget *output,
                              PythonInterpreter::State &state);
    void executeStatementWithState(const std::string &code,
                                   QListWidget *output,
                                   PythonInterpreter::State &state);
    void executeFunction(const pybind11::object &function);

  Q_SIGNALS:
    void executionStarted();
    void executionFinished();

  private:
    template <pybind11::eval_mode mode>
    void executeCodeString(const std::string &code,
                           QListWidget *output,
                           PythonInterpreter::State &state);

  private:
    bool m_isExecuting{false};

    PythonConfigPaths m_config;

#ifdef Q_OS_UNIX
    void *m_libPythonHandle{nullptr};
#endif
};

#endif // PYTHON_PLUGIN_PYTHON_INTERPRETER_H
