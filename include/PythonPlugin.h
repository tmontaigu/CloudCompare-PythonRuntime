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
class QListWidget;

namespace ui {
	class QPythonREPL;
}


class PythonConfigPaths {
public:
	PythonConfigPaths();


	const wchar_t *pythonHome() const {
		return m_pythonHome.get();
	}

	const wchar_t *pythonPath() const {
		return m_pythonPath.get();
	}

private:
	std::unique_ptr<wchar_t[]> m_pythonHome;
	std::unique_ptr<wchar_t[]> m_pythonPath;
};


class PythonPlugin : public QObject, public ccStdPluginInterface
{
Q_OBJECT
	Q_INTERFACES(ccPluginInterface ccStdPluginInterface)

	Q_PLUGIN_METADATA(IID "cccorp.cloudcompare.plugin.PythonPlugin" FILE "../info.json")

public:
	explicit PythonPlugin(QObject *parent = nullptr);

	~PythonPlugin() override;

	void onNewSelection(const ccHObject::Container &selectedEntities) override;

	QList<QAction *> getActions() override;


private:
	void showRepl();
	void showEditor();
	void executeEditorCode(const std::string &evalFileName, const std::string &code, QListWidget *output);

	ui::QPythonREPL *m_repl{nullptr};
	QPythonEditor *m_editor{nullptr};

	PythonConfigPaths m_pythonConfig{};

	/// Actions
	QAction *m_showEditor{nullptr};
	QAction *m_showREPL{nullptr};
};
