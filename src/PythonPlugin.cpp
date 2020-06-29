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

// Default constructor:
//	- pass the Qt resource path to the info.json file (from <yourPluginName>.qrc file) 
//  - constructor should mainly be used to initialize actions and other members
PythonPlugin::PythonPlugin(QObject *parent)
		: QObject(parent)
		, ccStdPluginInterface(":/CC/plugin/PythonPlugin/info.json")
{
	m_repl = new ui::QPythonREPL();
}

// This method should enable or disable your plugin actions
// depending on the currently selected entities ('selectedEntities').
void PythonPlugin::onNewSelection(const ccHObject::Container &selectedEntities)
{
	if (m_action == nullptr)
	{
		return;
	}

	m_action->setEnabled(true);
}

// This method returns all the 'actions' your plugin can perform.
// getActions() will be called only once, when plugin is loaded.
QList<QAction *> PythonPlugin::getActions()
{
	if (!m_action)
	{
		m_action = new QAction("Run script", this);
		m_action->setToolTip("Run a Python Script");
		m_action->setIcon(getIcon());

		connect(m_action, &QAction::triggered, this, [this]()
		{
			Python::runScript(m_app);
		});
	}

	if (!m_repl_action) {
		m_repl_action = new QAction("Show REPL", this);
		m_repl_action->setToolTip("Start Python REPL");
		m_repl_action->setIcon(QIcon(":/CC/plugin/PythonPlugin/images/repl-icon.png"));
		connect(m_repl_action, &QAction::triggered, this, &PythonPlugin::showRepl);
	}

	return {m_action, m_repl_action};
}

void PythonPlugin::showRepl() const
{
	if (m_repl) {
		Python::setMainAppInterfaceInstance(m_app);
		m_repl->show();
	}
}

PythonPlugin::~PythonPlugin()
{
	Python::unsetMainAppInterfaceInstance();
}
