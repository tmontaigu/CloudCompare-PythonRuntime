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

#ifndef PYTHON_PLUGIN_H
#define PYTHON_PLUGIN_H

#include "PythonConfig.h"
#include "PythonInterpreter.h"
#include "PythonPluginManager.h"
#include "ccStdPluginInterface.h"

#include <iostream>
#include <map>
#include <vector>

class QListWidget;
class PythonEditor;
class PythonRepl;
class FileRunner;
class PackageManager;
class PythonActionLauncher;
class PythonRuntimeSettings;

/// "Entry point" of the plugin
class PythonPlugin final : public QObject, public ccStdPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(ccPluginInterface ccStdPluginInterface)

    Q_PLUGIN_METADATA(IID "cccorp.cloudcompare.plugin.PythonRuntime" FILE "../info.json")

  public:
    explicit PythonPlugin(QObject *parent = nullptr);

    QList<QAction *> getActions() override;

    /// Registers the -PYTHON_SCRIPT command to run Python scripts in
    /// CloudCompare's commandline mode
    void registerCommands(ccCommandLineInterface *cmd) override;

    void setMainAppInterface(ccMainAppInterface *app) override;

    void stop() override;

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
    void showPythonActionLauncher() const;
    void showSettings() const;
    static void showDocumentation();

    // Script list function
    void addScriptAction();
    void addScript(QString path);
    void executeScript(QString path);
    void removeScript(QString name, QAction *self);

    void populatePluginSubMenu();

    void handlePluginActionClicked(bool checked);
    void handlePythonExecutionStarted();
    void handlePythonExecutionFinished();

    PythonConfig m_config{};
    PythonInterpreter m_interp;
    PythonPluginManager m_pluginManager;

    PythonRuntimeSettings *m_settings{nullptr};
    PythonRepl *m_repl{nullptr};
    PythonEditor *m_editor{nullptr};
    FileRunner *m_fileRunner{nullptr};
    PackageManager *m_packageManager{nullptr};
    PythonActionLauncher *m_actionLauncher{nullptr};

    /// Actions
    QAction *m_showEditor{nullptr};
    QAction *m_showRepl{nullptr};
    QAction *m_showDoc{nullptr};
    QAction *m_showFileRunner{nullptr};
    QAction *m_showAboutDialog{nullptr};
    QAction *m_showPackageManager{nullptr};
    QAction *m_showActionLauncher{nullptr};
    QAction *m_showSettings{nullptr};

    /// Script list submenu
    QMenu *m_drawScriptRegister{nullptr};
    QMenu *m_pluginsMenu{nullptr};

    /// keep track of plugins QAction and their corresponding registered plugin actions
    std::map<const QAction *, const Runtime::RegisteredPlugin::Action *> m_pluginActions;

    /// Variable for script list submenu
    QAction *m_addScript{nullptr};
    QMenu *m_removeScript{nullptr};
    std::map<QString, QAction *> m_scriptList;
    QStringList m_savedPath;
    QString m_saveFilePath;
};

#endif // PYTHON_PLUGIN_H
