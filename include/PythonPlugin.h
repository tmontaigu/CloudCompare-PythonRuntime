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

class QPythonEditor;
class QListWidget;
class QDocViewer;

namespace ui
{
class QPythonREPL;
}

struct Version {
    constexpr Version() = default;

    constexpr Version(uint16_t major, uint16_t minor, uint16_t patch);

    explicit Version(const QStringRef& versionStr);

    bool operator==(const Version& other) const;

    uint16_t major{0};
    uint16_t minor{0};
    uint16_t patch{0};
};

struct PyVenvCfg
{
    PyVenvCfg() = default;

    static PyVenvCfg FromFile(const QString &path);

    QString home{};
    bool includeSystemSitesPackages{};
    Version version;
};

/// Holds strings of the PythonHome & PythonPath
/// Which are variables that needs to be properly set
/// in order to have a functioning Python environment
class PythonConfigPaths
{
  public:
    PythonConfigPaths() = default;

    static PythonConfigPaths WindowsBundled();

    static PythonConfigPaths WindowsCondaEnv(const char * condaPrefix);

    static PythonConfigPaths WindowsVenv(const char *venvPrefix, const PyVenvCfg& cfg);

    bool isSet() const { return m_pythonHome != nullptr && m_pythonPath != nullptr; }

    const wchar_t *pythonHome() const { return m_pythonHome.get(); }

    const wchar_t *pythonPath() const { return m_pythonPath.get(); }

  private:
    std::unique_ptr<wchar_t[]> m_pythonHome{};
    std::unique_ptr<wchar_t[]> m_pythonPath{};
};

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

  private:
    void showRepl();
    void showEditor();
    void showDocumentation();
    void showAboutDialog();
    void executeEditorCode(const std::string &evalFileName, const std::string &code, QListWidget *output);
    void configurePython();



    ui::QPythonREPL *m_repl{nullptr};
    QPythonEditor *m_editor{nullptr};
    QDocViewer *m_docView{nullptr};


    PythonConfigPaths m_pythonConfig{};

    /// Actions
    QAction *m_showEditor{nullptr};
    QAction *m_showREPL{nullptr};
    QAction *m_showDoc{nullptr};
    QAction *m_showAboutDialog{nullptr};
};
