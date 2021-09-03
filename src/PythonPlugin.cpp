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

#include "PythonPlugin.h"
#include "AboutDialog.h"
#include "CodeEditor/PythonEditor.h"
#include "FileRunner.h"
#include "PackageManager.h"
#include "PythonActionLauncher.h"
#include "PythonPluginSettings.h"
#include "PythonRepl.h"
#include "PythonStdErrOutRedirect.h"
#include "Runtime/Runtime.h"
#include "Utilities.h"

#include <QDesktopServices>
#include <QUrl>

#define slots Q_SLOTS
#define signals Q_SIGNALS
#include <ccCommandLineInterface.h>

static void LoadCustomPythonPlugins(const QString &paths)
{
    if (paths.isEmpty())
    {
        return;
    }
#ifdef Q_OS_WIN
    const QChar sep = ';';
#else
    const QChar sep = ':';
#endif

    ccLog::Print("[PythonPlugin] Searching for custom plugin");
    QStringList pluginsPaths = paths.split(sep);
    py::module::import("sys").attr("path").attr("append")(paths);
    for (const QString &path : pluginsPaths)
    {
        ccLog::Print(QString("[PythonPlugin]     searching in %1").arg(path));
        QDirIterator iter(path);
        while (iter.hasNext())
        {
            iter.next();
            QFileInfo entry = iter.fileInfo();
            QString fileName = entry.fileName();

            if (fileName == "." || fileName == ".." || fileName == "__pycache__")
            {
                continue;
            }

            QString nameToImport = fileName;
            if (!entry.isDir() && fileName.endsWith(".py"))
            {
                nameToImport = fileName.left(fileName.size() - 3);
            }

            const std::string nameToImportStd = nameToImport.toStdString();
            try
            {
                py::module::import(nameToImportStd.c_str());
                ccLog::Print("[PythonPlugin]     Loaded plugin '%s'", nameToImportStd.c_str());
            }
            catch (const std::exception &e)
            {
                ccLog::Warning("[PythonPlugin]     Failed to load plugin '%s': %s",
                               nameToImportStd.c_str(),
                               e.what());
            }
        }

        py::list subClassTypes = py::module::import("pycc_runtime")
                                     .attr("PythonPluginInterface")
                                     .attr("__subclasses__")();
        for (auto &subClassType : subClassTypes)
        {
            try
            {
                // Here, we create an instance of the plugin,
                // it will then register the methods it wants to appear as actions
                // (because we call the "registerActions")
                // As we keep references to registered methods
                // we do not need to keep a reference to the actual instance.
                auto instance = subClassType.cast<py::object>();
                instance().attr("registerActions")();
            }
            catch (const std::exception &e)
            {
                ccLog::Warning("[PythonPlugin]     Failed to instantiate plugin: %s", e.what());
            }
        }
    }
}

// Useful link:
// https://docs.python.org/3/c-api/init.html#initialization-finalization-and-threads
PythonPlugin::PythonPlugin(QObject *parent)
    : QObject(parent),
      ccStdPluginInterface(":/CC/plugin/PythonPlugin/info.json"),
      m_interp(nullptr),
      m_editor(new PythonEditor(&m_interp)),
      m_fileRunner(new FileRunner(&m_interp)),
      m_actionLauncher(new PythonActionLauncher),
      m_settings(new PythonPluginSettings)
{
    m_interp.initialize(m_config);

    LogPythonHome();
    LogPythonPath();

    connect(QCoreApplication::instance(),
            &QCoreApplication::aboutToQuit,
            this,
            &PythonPlugin::finalizeInterpreter);
}

QList<QAction *> PythonPlugin::getActions()
{
    const bool enableActions = PythonInterpreter::IsInitialized();

    if (!m_showEditor)
    {
        m_showEditor = new QAction("Show Editor", this);
        m_showEditor->setToolTip("Show the code editor window");
        m_showEditor->setIcon(QIcon(":/CC/plugin/PythonPlugin/images/python-editor-icon.png"));
        connect(m_showEditor, &QAction::triggered, this, &PythonPlugin::showEditor);
        m_showEditor->setEnabled(enableActions);
    }

    if (!m_showRepl)
    {
        m_showRepl = new QAction("Show REPL", this);
        m_showRepl->setToolTip("Show the Python REPL");
        m_showRepl->setIcon(QIcon(":/CC/plugin/PythonPlugin/images/repl-icon.png"));
        connect(m_showRepl, &QAction::triggered, this, &PythonPlugin::showRepl);
        m_showRepl->setEnabled(enableActions);
    }

    if (!m_showDoc)
    {
        m_showDoc = new QAction("Show Documentation", this);
        m_showDoc->setToolTip("Show local documentation in your web browser");
        m_showDoc->setIcon(
            m_app->getMainWindow()->style()->standardIcon(QStyle::SP_FileDialogInfoView));
        connect(m_showDoc, &QAction::triggered, &PythonPlugin::showDocumentation);
        m_showDoc->setEnabled(enableActions);
    }

    if (!m_showAboutDialog)
    {
        m_showAboutDialog = new QAction("About", this);
        m_showAboutDialog->setToolTip("About this plugin");
        m_showAboutDialog->setIcon(
            m_app->getMainWindow()->style()->standardIcon(QStyle::SP_MessageBoxQuestion));
        connect(m_showAboutDialog, &QAction::triggered, this, &PythonPlugin::showAboutDialog);
        m_showAboutDialog->setEnabled(enableActions);
    }

    if (!m_showFileRunner)
    {
        m_showFileRunner = new QAction("File Runner", this);
        m_showFileRunner->setToolTip("Small widget to select and run a script");
        m_showFileRunner->setIcon(QIcon(":/CC/plugin/PythonPlugin/images/runner-icon.png"));
        connect(m_showFileRunner, &QAction::triggered, this, &PythonPlugin::showFileRunner);
        m_showFileRunner->setEnabled(enableActions);
    }

    if (!m_showPackageManager)
    {
        m_showPackageManager = new QAction("Package Manager", this);
        m_showPackageManager->setToolTip("Manage packages with pip");
        m_showPackageManager->setIcon(QIcon());
        connect(m_showPackageManager, &QAction::triggered, this, &PythonPlugin::showPackageManager);
        m_showPackageManager->setEnabled(enableActions);
    }

    if (!m_showActionLauncher)
    {
        m_showActionLauncher = new QAction("Show Action Launcher", this);
        m_showActionLauncher->setToolTip("Launch actions of custom Python plugins");
        m_showActionLauncher->setIcon(QIcon());
        connect(m_showActionLauncher,
                &QAction::triggered,
                this,
                &PythonPlugin::showPythonActionLauncher);
        m_showActionLauncher->setEnabled(enableActions);
    }

    if (!m_showSettings)
    {
        m_showSettings = new QAction("Show Settings", this);
        m_showSettings->setToolTip("Show some settings");
        m_showActionLauncher->setIcon(QIcon());
        connect(m_showSettings, &QAction::triggered, this, &PythonPlugin::showSettings);
        m_showSettings->setEnabled(enableActions);
    }

    return {m_showEditor,
            m_showFileRunner,
            m_showAboutDialog,
            m_showDoc,
            m_showRepl,
            m_showPackageManager,
            m_showActionLauncher,
            m_showSettings};
}

void PythonPlugin::showRepl()
{
    if (m_repl)
    {
        m_repl->show();
        m_repl->raise();
        m_repl->activateWindow();
    }
    else
    {
        m_repl = new PythonRepl(&m_interp);
        m_repl->show();
    }
}

void PythonPlugin::showEditor() const
{
    if (m_editor)
    {
        m_editor->show();
        m_editor->raise();
        m_editor->activateWindow();
    }
}

void PythonPlugin::showFileRunner() const
{
    m_fileRunner->show();
}

void PythonPlugin::showDocumentation()
{
    const QUrl url(QString("https://tmontaigu.github.io/CloudCompare-PythonPlugin/index.html"));
    QDesktopServices::openUrl(url);
}

void PythonPlugin::showAboutDialog() const
{
    AboutDialog dlg(m_app->getMainWindow());
    dlg.exec();
}

void PythonPlugin::showPackageManager()
{
    if (m_packageManager == nullptr)
    {
        m_packageManager = new PackageManager(m_config);
    }
    m_packageManager->show();
    m_editor->raise();
    m_editor->activateWindow();
}

void PythonPlugin::showPythonActionLauncher() const
{
    m_actionLauncher->show();
}

void PythonPlugin::showSettings() const
{
    m_settings->show();
}

PythonPlugin::~PythonPlugin() noexcept
{
    Runtime::unsetMainAppInterfaceInstance();
    Runtime::unsetCmdLineInterfaceInstance();
}

struct PythonPluginCommand final : public ccCommandLineInterface::Command
{
    explicit PythonPluginCommand(PythonInterpreter *interpreter_)
        : Command("PYTHON", "PYTHON_SCRIPT"), interpreter(interpreter_)
    {
    }

    bool process(ccCommandLineInterface &cmd) override
    {
        Q_ASSERT(interpreter);
        cmd.print("[PythonPlugin] Starting");
        Args args;
        if (!args.parseFrom(cmd))
        {
            return false;
        }

        PySys_SetArgvEx(static_cast<int>(args.pythonArgv.size()), args.pythonArgv.data(), 1);
        const bool success = interpreter->executeFile(qPrintable(args.filepath));

        cmd.print(QString("[PythonPlugin] Script %1 executed")
                      .arg(success ? "successfully" : "unsuccessfully"));
        return success;
    }

    struct Args
    {
        QString filepath;
        std::vector<wchar_t *> pythonArgv{};

        Args() = default;

        bool parseFrom(ccCommandLineInterface &cmd)
        {
            if (cmd.arguments().empty())
            {
                return cmd.error(QString("Missing parameter: parameters filename after \"-%1\"")
                                     .arg("PYTHON_SCRIPT"));
            }
            filepath = cmd.arguments().takeFirst();

            pythonArgv.reserve(cmd.arguments().size() + 1);
            pythonArgv.push_back(QStringToWcharArray(filepath));
            while (!cmd.arguments().isEmpty())
            {
                pythonArgv.push_back(QStringToWcharArray(cmd.arguments().takeFirst()));
            }
            return true;
        }

        virtual ~Args()
        {
            for (wchar_t *arg : pythonArgv)
            {
                delete[] arg;
            }
        }
    };

    PythonInterpreter *interpreter{nullptr};
};

void PythonPlugin::registerCommands(ccCommandLineInterface *cmd)
{
    cmd->registerCommand(
        ccCommandLineInterface::Command::Shared(new PythonPluginCommand(&m_interp)));
    Runtime::setCmdLineInterfaceInstance(cmd);
}

void PythonPlugin::setMainAppInterface(ccMainAppInterface *app)
{
    ccStdPluginInterface::setMainAppInterface(app);
    Runtime::setMainAppInterfaceInstance(m_app);

    // Now that the mainAppInterface is set, we can load custom
    // python plugins, if we did this earlier, `pycc.GetInstance()`
    // in python would return `None` and that's bad.
    try
    {
        LoadCustomPythonPlugins(m_settings->pluginsPaths());
    }
    catch (const std::exception &e)
    {
        ccLog::Warning("[PythonPlugin] Failed to load custom python plugins: %e", e.what());
    }

    m_fileRunner->setParent(m_app->getMainWindow(), Qt::Window);
    m_actionLauncher->setParent(m_app->getMainWindow(), Qt::Window);
    m_settings->setParent(m_app->getMainWindow(), Qt::Window);
}

void PythonPlugin::finalizeInterpreter()
{
    // We have to clear registered functions before
    // we finalize the interpreter otherwise our references to
    // python object would outlive the interpreter
    Runtime::clearRegisteredFunction();
    m_interp.finalize();
}
