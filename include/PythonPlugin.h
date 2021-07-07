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
#include "PythonInterpreter.h"
#include "PythonConfig.h"

class QListWidget;
class QPythonEditor;
class QPythonRepl;
class FileRunner;
class PackageManager;

/// "Entry point" of the plugin
class PythonPlugin final : public QObject, public ccStdPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(ccPluginInterface ccStdPluginInterface)

    Q_PLUGIN_METADATA(IID "cccorp.cloudcompare.plugin.PythonPlugin" FILE "../info.json")

  public:
    explicit PythonPlugin(QObject *parent = nullptr);

    QList<QAction *> getActions() override;

    /// Registers the -PYTHON_SCRIPT command to run Python scripts in
    /// CloudCompare's commandline mode
    void registerCommands(ccCommandLineInterface *cmd) override;

    void setMainAppInterface(ccMainAppInterface *app) override;

    PythonPlugin(const PythonPlugin &) = delete;
    PythonPlugin(PythonPlugin &&) = delete;
    PythonPlugin &operator=(const PythonPlugin &) = delete;
    PythonPlugin &operator=(PythonPlugin &&) = delete;
    ~PythonPlugin() noexcept override;

  private:
    void finalizeInterpreter();
    void showRepl();
    void showEditor() const;
    void showAboutDialog() const;
    void showFileRunner() const;
    void showPackageManager();
    static void showDocumentation();

    PythonConfig m_config{};
    PythonInterpreter m_interp;

    QPythonRepl *m_repl{nullptr};
    QPythonEditor *m_editor{nullptr};
    FileRunner *m_fileRunner{nullptr};
    PackageManager *m_packageManager{nullptr};



    /// Actions
    QAction *m_showEditor{nullptr};
    QAction *m_showRepl{nullptr};
    QAction *m_showDoc{nullptr};
    QAction *m_showFileRunner{nullptr};
    QAction *m_showAboutDialog{nullptr};
    QAction *m_showPackageManager{nullptr};
};
