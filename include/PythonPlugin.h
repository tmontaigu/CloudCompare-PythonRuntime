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

#pragma once

#include <memory>
#include "ccStdPluginInterface.h"


class QPythonEditor;

namespace ui {
	class QPythonREPL;
}


class PythonPlugin : public QObject, public ccStdPluginInterface
{
Q_OBJECT
	Q_INTERFACES(ccPluginInterface ccStdPluginInterface)

	Q_PLUGIN_METADATA(IID "cccorp.cloudcompare.plugin.PythonPlugin" FILE "../info.json")

public:
	explicit PythonPlugin(QObject *parent = nullptr);

	virtual ~PythonPlugin();

	void onNewSelection(const ccHObject::Container &selectedEntities) override;

	QList<QAction *> getActions() override;


private:
	void showRepl();
	void showEditor();
	void runScript();
	void executeEditorCode(const std::string &evalFileName, const std::string &code);

	ui::QPythonREPL *m_repl{nullptr};
	QPythonEditor *editor{nullptr};

	/// These are used to correctly setup the Python interpreter
	std::unique_ptr<wchar_t[]> m_pythonPath{nullptr};
	std::unique_ptr<wchar_t[]> m_pythonHome{nullptr};

	/// Actions
	QAction *m_action{nullptr};
	QAction *m_show_editor{nullptr};
	QAction *m_repl_action{nullptr};
};
