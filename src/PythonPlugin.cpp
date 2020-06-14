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

#include "PythonPlugin.h"

#include "ActionA.h"

// Default constructor:
//	- pass the Qt resource path to the info.json file (from <yourPluginName>.qrc file) 
//  - constructor should mainly be used to initialize actions and other members
PythonPlugin::PythonPlugin(QObject *parent)
		: QObject(parent), ccStdPluginInterface(":/CC/plugin/PythonPlugin/info.json"), m_action(nullptr)
{
}

// This method should enable or disable your plugin actions
// depending on the currently selected entities ('selectedEntities').
void PythonPlugin::onNewSelection(const ccHObject::Container &selectedEntities)
{
	if (m_action == nullptr)
	{
		return;
	}

	// If you need to check for a specific type of object, you can use the methods
	// in ccHObjectCaster.h or loop and check the objects' classIDs like this:
	//
	//	for ( ccHObject *object : selectedEntities )
	//	{
	//		if ( object->getClassID() == CC_TYPES::VIEWPORT_2D_OBJECT )
	//		{
	//			// ... do something with the viewports
	//		}
	//	}

//	 For example - only enable our action if something is selected.
	m_action->setEnabled(true);
}

// This method returns all the 'actions' your plugin can perform.
// getActions() will be called only once, when plugin is loaded.
QList<QAction *> PythonPlugin::getActions()
{
	if (!m_action)
	{
		m_action = new QAction("Run script", this);
		m_action->setToolTip(getDescription());
		m_action->setIcon(getIcon());

		connect(m_action, &QAction::triggered, this, [this]()
		{
			Python::performActionA(m_app);
		});
	}

	return {m_action};
}
