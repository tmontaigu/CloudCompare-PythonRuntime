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

#pragma once

#include "ccStdPluginInterface.h"
#include <memory>

#include "PythonInterpreter.h"

class QPythonEditor;
class QListWidget;
class QDocViewer;

namespace ui
{
class QPythonREPL;
}


/// "Entry point" of the plugin
class PythonPlugin : public QObject, public ccStdPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(ccPluginInterface ccStdPluginInterface)

    Q_PLUGIN_METADATA(IID "cccorp.cloudcompare.plugin.PythonPlugin" FILE "../info.json")

  public:
    explicit PythonPlugin(QObject *parent = nullptr);

    ~PythonPlugin() override;

    QList<QAction *> getActions() override;

    /// Registers the -PYTHON_SCRIPT command to run Python scripts in
    /// CloudCompare's commandline mode
    void registerCommands(ccCommandLineInterface *cmd) override;

    void setMainAppInterface(ccMainAppInterface *app) override;

  private:
    void showRepl();
    void showEditor();
    void showDocumentation();
    void showAboutDialog();

    PythonInterpreter m_interp;

    ui::QPythonREPL *m_repl{nullptr};
    QPythonEditor *m_editor{nullptr};
    QDocViewer *m_docView{nullptr};

    /// Actions
    QAction *m_showEditor{nullptr};
    QAction *m_showREPL{nullptr};
    QAction *m_showDoc{nullptr};
    QAction *m_showAboutDialog{nullptr};
};
