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

#ifndef CLOUDCOMPAREPROJECTS_QPYTHONREPL_H
#define CLOUDCOMPAREPROJECTS_QPYTHONREPL_H

#include <QMainWindow>

#include <string>

#include "PythonInterpreter.h"


class PythonInterpreter;
class Ui_QPythonREPL;
class QPlainTextEdit;

class QPythonREPL;

/// Simple history system for the REPL.
class History
{
  public:
    explicit History() = default;

    void add(QString &&cmd);

    size_t size() const;

    bool empty() const;

    const QString &older();

    const QString &newer();

  private:
    QVector<QString> m_commands;
    QVector<QString>::const_reverse_iterator m_current;
};

/// Class used by the REPL to handle key presses
class KeyPressEater : public QObject
{
    Q_OBJECT
  public:
    explicit KeyPressEater(QPythonREPL *repl, QObject *parent = nullptr);

  protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

  private:
    QPythonREPL *m_repl{nullptr};
};

/// Homemade REPL (Read Print Eval Loop)
class QPythonREPL : public QMainWindow
{
    friend KeyPressEater;
    Q_OBJECT

  public:
    explicit QPythonREPL(PythonInterpreter *interpreter, QMainWindow *parent = nullptr);

    void executeCode(const QString &pythonCode);

    ~QPythonREPL() override;

  protected:
    QPlainTextEdit *codeEdit();
    QListWidget *outputDisplay();

    void reset();
    void importNeededPackages();
    void setupUI();

  private:
    History m_history{};
    std::string m_buf;
    Ui_QPythonREPL *m_ui{nullptr};
    PythonInterpreter *m_interpreter{nullptr};
    PythonInterpreter::State m_state;
};


#endif // CLOUDCOMPAREPROJECTS_QPYTHONREPL_H
