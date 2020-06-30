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



#include <QtGui>
#include <iostream>

#include "PythonPlugin.h"
#include "QPythonREPL.h"
#include "ActionA.h"

#include <Python.h>


/// Returns a newly allocated array (null terminated) from a QString
wchar_t *qstring_to_wchar_array(const QString &string) {
    auto *wcharArray = new wchar_t[string.size() + 1];
    int len = string.toWCharArray(wcharArray);
    if ( len > string.size())
    {
        throw std::logic_error("len mismatch");
    }
    wcharArray[len] = '\0';
    return wcharArray;
}


// Useful link: https://docs.python.org/3/c-api/init.html#initialization-finalization-and-threads
PythonPlugin::PythonPlugin(QObject *parent)
        : QObject(parent), ccStdPluginInterface(":/CC/plugin/PythonPlugin/info.json") {
    QDir pythonEnvDirPath(QApplication::applicationDirPath() + "/plugins/Python");
    if ( pythonEnvDirPath.exists() )
    {
        QString qPythonHome = pythonEnvDirPath.path();
        m_pythonHome.reset(qstring_to_wchar_array(qPythonHome));
        Py_SetPythonHome(m_pythonHome.get());

        // FIXME:
        //  ';' as separator is only for windows, linux & macos uses ':'
        QString qPythonPath = QString("%1/DLLs;%1/lib;%1/Lib/site-packages").arg(qPythonHome);
        m_pythonPath.reset(qstring_to_wchar_array(qPythonPath));
        Py_SetPath(m_pythonPath.get());
    } else
    {
        throw std::runtime_error("Python environment not found, plugin wasn't correctly installed");
    }

    const wchar_t *pythonHome = Py_GetPythonHome();
    if ( pythonHome != nullptr )
    {
        size_t errPos{0};
        char *cPythonHome = Py_EncodeLocale(pythonHome, &errPos);
        if ( cPythonHome )
        {
            ccLog::Print("[PythonPlugin] PythonHome is set to: %s", cPythonHome);
            PyMem_Free(cPythonHome);
        } else
        {
            ccLog::Print("[PythonPlugin]Failed to convert the PythonHome path\n");
        }
    } else
    {
        ccLog::Print("[PythonPlugin] PythonHome is not set\n");
    }

    const wchar_t *pythonPath = Py_GetPath();
    if ( pythonPath != nullptr )
    {
        size_t errPos{0};
        char *cPythonPath = Py_EncodeLocale(pythonPath, &errPos);
        if ( cPythonPath )
        {
            ccLog::Print("[PythonPlugin] PythonPath is set to: %s", cPythonPath);
        } else
        {
            ccLog::Print("[PythonPlugin] Failed to convert the PythonPath\n");
        }
    } else
    {
        ccLog::Print("[PythonPlugin] PythonPath is not set\n");
    }

    py::initialize_interpreter();
}

// This method should enable or disable your plugin actions
// depending on the currently selected entities ('selectedEntities').
void PythonPlugin::onNewSelection(const ccHObject::Container &selectedEntities) {
}

// This method returns all the 'actions' your plugin can perform.
// getActions() will be called only once, when plugin is loaded.
QList<QAction *> PythonPlugin::getActions() {
    if ( !m_action )
    {
        m_action = new QAction("Run script", this);
        m_action->setToolTip("Run a Python Script");
        m_action->setIcon(getIcon());

        connect(m_action, &QAction::triggered, this, [this]() {
            Python::runScript(m_app);
        });
        m_action->setEnabled(true);
    }

    if ( !m_repl_action )
    {
        m_repl_action = new QAction("Show REPL", this);
        m_repl_action->setToolTip("Start Python REPL");
        m_repl_action->setIcon(QIcon(":/CC/plugin/PythonPlugin/images/repl-icon.png"));
        connect(m_repl_action, &QAction::triggered, this, &PythonPlugin::showRepl);
        m_repl_action->setEnabled(true);
    }

    return {m_action, m_repl_action};
}

void PythonPlugin::showRepl() {
    if ( m_repl == nullptr )
    {
        Python::setMainAppInterfaceInstance(m_app);
        m_repl = new ui::QPythonREPL();
    }
    m_repl->show();
}

PythonPlugin::~PythonPlugin() {
    py::finalize_interpreter();
    Python::unsetMainAppInterfaceInstance();
}
