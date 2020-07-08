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
#include <memory>

#include "PythonPlugin.h"
#include "QPythonREPL.h"
#include "PrivateRuntime.h"
#include "QPythonEditor.h"

#include <pybind11/embed.h>
#include <Python.h>

#define slots Q_SLOTS
#define signals Q_SIGNALS

#include <ccCommandLineInterface.h>


/// Returns a newly allocated array (null terminated) from a QString
wchar_t *QStringToWcharArray(const QString &string)
{
	auto *wcharArray = new wchar_t[string.size() + 1];
	int len = string.toWCharArray(wcharArray);
	if ( len > string.size())
	{
		throw std::logic_error("len mismatch");
	}
	wcharArray[len] = '\0';
	return wcharArray;
}


void logPythonPath()
{
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
}

void logPythonHome()
{
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
}


PythonConfigPaths::PythonConfigPaths()
{
	QDir pythonEnvDirPath(QApplication::applicationDirPath() + "/plugins/Python");
	if ( pythonEnvDirPath.exists())
	{
		QString qPythonHome = pythonEnvDirPath.path();
		m_pythonHome.reset(QStringToWcharArray(qPythonHome));

		// FIXME:
		//  ';' as separator is only for windows, linux & macos uses ':'
		QString qPythonPath = QString("%1/DLLs;%1/lib;%1/Lib/site-packages").arg(qPythonHome);
		m_pythonPath.reset(QStringToWcharArray(qPythonPath));
	} else
	{
		throw std::runtime_error("Python environment not found, plugin wasn't correctly installed");
	}
}


// Useful link: https://docs.python.org/3/c-api/init.html#initialization-finalization-and-threads
PythonPlugin::PythonPlugin(QObject *parent)
		: QObject(parent), ccStdPluginInterface(":/CC/plugin/PythonPlugin/info.json")
{

	try
	{
		m_pythonConfig = std::make_unique<PythonConfigPaths>();
		Py_SetPythonHome(m_pythonConfig->pythonHome());
		Py_SetPath(m_pythonConfig->pythonPath());
	} catch (const std::exception &)
	{}

	py::initialize_interpreter();

	m_editor = new QPythonEditor();
	connect(m_editor, &QPythonEditor::executionCalled, this, &PythonPlugin::executeEditorCode);
}


void PythonPlugin::onNewSelection(const ccHObject::Container &selectedEntities)
{
}


QList<QAction *> PythonPlugin::getActions()
{
	if ( !m_showEditor )
	{
		m_showEditor = new QAction("Show Editor", this);
		m_showEditor->setToolTip("Show the code editor window");
		m_showEditor->setIcon(QIcon(":/CC/plugin/PythonPlugin/images/python-editor-icon.png"));
		connect(m_showEditor, &QAction::triggered, this, &PythonPlugin::showEditor);
		m_showEditor->setEnabled(true);
	}

	if ( !m_showREPL )
	{
		m_showREPL = new QAction("Show REPL", this);
		m_showREPL->setToolTip("Show the Python REPL");
		m_showREPL->setIcon(QIcon(":/CC/plugin/PythonPlugin/images/repl-icon.png"));
		connect(m_showREPL, &QAction::triggered, this, &PythonPlugin::showRepl);
		m_showREPL->setEnabled(true);
	}

	return {m_showREPL, m_showEditor};
}

void PythonPlugin::showRepl()
{
	if ( m_repl )
	{
		m_repl->raise();
		m_repl->activateWindow();
	} else {
		Python::setMainAppInterfaceInstance(m_app);
		m_repl = new ui::QPythonREPL();
		m_repl->show();
	}
}

void PythonPlugin::showEditor()
{
	if ( m_editor )
	{
		Python::setMainAppInterfaceInstance(m_app);
		m_editor->show();
		m_editor->raise();
		m_editor->activateWindow();
	}
}

void PythonPlugin::executeEditorCode(const std::string &evalFileName, const std::string &code, QListWidget *output)
{
	try
	{
		py::object o = py::module::import("ccinternals")
				.attr("ConsoleREPL")(output);
		PyStdErrOutStreamRedirect redirect{o, o};
		py::exec(code);
	} catch (const std::exception &e)
	{
		auto message = new QListWidgetItem(e.what());
		message->setTextColor(Qt::red);
		output->addItem(message);
	}
}

PythonPlugin::~PythonPlugin()
{
	py::finalize_interpreter();
	Python::unsetMainAppInterfaceInstance();
}

struct PythonPluginCommand : public ccCommandLineInterface::Command
{
	PythonPluginCommand() : Command("PYTHON", "PYTHON_SCRIPT")
	{}

	bool process(ccCommandLineInterface &cmd) override
	{

		cmd.print("[PythonPlugin] Stating");
		if ( cmd.arguments().empty())
		{
			return cmd.error(QString("Missing parameter: parameters filename after \"-%1\"").arg("PYTHON_SCRIPT"));
		}

		//open specified file
		QString paramFilename(cmd.arguments().takeFirst());
		cmd.print(QString("[PythonPlugin] python file: '%1'").arg(paramFilename));
		try
		{
			PyStdErrOutStreamRedirect r{};
			py::eval_file(qPrintable(paramFilename));
		} catch (const std::exception &e)
		{
			ccLog::Warning(e.what());
			return false;
		}
		return true;
	}

};

void PythonPlugin::registerCommands(ccCommandLineInterface *cmd)
{
	cmd->registerCommand(ccCommandLineInterface::Command::Shared(new PythonPluginCommand()));
}
