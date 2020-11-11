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

#include <QtGui>
#include <iostream>
#include <memory>

#include "AboutDialog.h"
#include "PrivateRuntime.h"
#include "PythonPlugin.h"
#include "QPythonEditor.h"
#include "QPythonREPL.h"

#include <Python.h>
#include <pybind11/embed.h>

#include <QWebEngineHistory>
#include <QWebEngineSettings>
#include <QWebEngineView>

#define slots Q_SLOTS
#define signals Q_SIGNALS
#include <ccCommandLineInterface.h>

class QDocViewer : public QWidget
{
    Q_OBJECT

  public:
    explicit QDocViewer(QWidget *parent = nullptr) : QWidget(parent, Qt::Window)
    {
        setMinimumSize(1280, 800);
        setWindowTitle("Python Plugin Documentation");

        m_viewEngine = new QWebEngineView;
        setupWebEngine();

        auto toolbar = new QToolBar;
        setupToolBarActions(toolbar);

        auto layout = new QVBoxLayout;
        layout->setSpacing(0);
        layout->setMargin(0);
        layout->addWidget(toolbar);
        layout->addWidget(m_viewEngine);
        setLayout(layout);

        loadHomePage();
    }

    void loadHomePage()
    {
        QUrl url(QString("file:///%1/%2")
                     .arg(QApplication::applicationDirPath())
                     .arg("plugins/Python/docs/index.html"));
        m_viewEngine->load(QUrl(url));
    }

    void loadPreviousPage() { m_viewEngine->history()->back(); }
    void loadNextPage() { m_viewEngine->history()->forward(); }

  protected:
    void setupWebEngine()
    {
        QWebEngineSettings *settings = m_viewEngine->settings();
        settings->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
        settings->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, false);
    }

    void setupToolBarActions(QToolBar *toolbar)
    {

        auto homePageAction = new QAction("Home Page");
        homePageAction->setIcon(this->style()->standardIcon(QStyle::SP_DirHomeIcon));
        connect(homePageAction, &QAction::triggered, this, &QDocViewer::loadHomePage);

        auto previousPageAction = new QAction("Previous Page");
        previousPageAction->setIcon(this->style()->standardIcon(QStyle::SP_ArrowLeft));
        connect(previousPageAction, &QAction::triggered, this, &QDocViewer::loadPreviousPage);

        auto nextPageAction = new QAction("Next Page");
        nextPageAction->setIcon(this->style()->standardIcon(QStyle::SP_ArrowRight));
        connect(nextPageAction, &QAction::triggered, this, &QDocViewer::loadNextPage);

        toolbar->addAction(previousPageAction);
        toolbar->addAction(nextPageAction);
        toolbar->addAction(homePageAction);
    }

  private:
    QWebEngineView *m_viewEngine{nullptr};
};

/// Returns a newly allocated array (null terminated) from a QString
wchar_t *QStringToWcharArray(const QString &string)
{
    auto *wcharArray = new wchar_t[string.size() + 1];
    int len = string.toWCharArray(wcharArray);
    if (len > string.size())
    {
        throw std::logic_error("len mismatch");
    }
    wcharArray[len] = '\0';
    return wcharArray;
}

void logPythonPath()
{
    const wchar_t *pythonPath = Py_GetPath();
    if (pythonPath != nullptr)
    {
        size_t errPos{0};
        char *cPythonPath = Py_EncodeLocale(pythonPath, &errPos);
        if (cPythonPath)
        {
            ccLog::Print("[PythonPlugin] PythonPath is set to: %s", cPythonPath);
        }
        else
        {
            ccLog::Print("[PythonPlugin] Failed to convert the PythonPath\n");
        }
    }
    else
    {
        ccLog::Print("[PythonPlugin] PythonPath is not set\n");
    }
}

void logPythonHome()
{
    const wchar_t *pythonHome = Py_GetPythonHome();
    if (pythonHome != nullptr)
    {
        size_t errPos{0};
        char *cPythonHome = Py_EncodeLocale(pythonHome, &errPos);
        if (cPythonHome)
        {
            ccLog::Print("[PythonPlugin] PythonHome is set to: %s", cPythonHome);
            PyMem_Free(cPythonHome);
        }
        else
        {
            ccLog::Print("[PythonPlugin]Failed to convert the PythonHome path\n");
        }
    }
    else
    {
        ccLog::Print("[PythonPlugin] PythonHome is not set\n");
    }
}


constexpr Version::Version(uint16_t major_, uint16_t minor_, uint16_t patch_)
    :  major(major_), minor(minor_), patch(patch_)
{
}

Version::Version(const QStringRef& versionStr) : Version() {

    QVector<QStringRef> parts = versionStr.split('.');
    if (parts.size() == 3)
    {
        major = parts[0].toUInt();
        minor = parts[1].toUInt();
        patch = parts[2].toUInt();
    }
}

bool Version::operator==(const Version& other) const {
    return major == other.major && minor == other.minor && patch == other.patch;
}

constexpr Version PythonVersion(PY_MAJOR_VERSION, PY_MINOR_VERSION, PY_MICRO_VERSION);

PyVenvCfg PyVenvCfg::FromFile(const QString &path)
{
    PyVenvCfg cfg{};

    QFile cfgFile(path);
    if (cfgFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!cfgFile.atEnd())
        {
            QString line = cfgFile.readLine();
            QStringList v = line.split("=");

            if (v.size() == 2)
            {
                QString name = v[0].simplified();
                QString value = v[1].simplified();

                if (name == "home")
                {
                    cfg.home = value;
                }
                else if (name == "include-system-site-packages")
                {
                    cfg.includeSystemSitesPackages = (value == "true");
                } else if (name == "version") {
                     cfg.version = Version(QStringRef(&value));
                }
            }
        }
    }

    return cfg;
}

Version GetPythonExeVersion(const QString& pythonExePath)
{
    QProcess pythonProcess;
    pythonProcess.start(pythonExePath, {"--version"});
    pythonProcess.waitForFinished();

    QString versionStr = QTextCodec::codecForName("utf-8")->toUnicode(pythonProcess.readAllStandardOutput());

    QVector<QStringRef> splits = versionStr.splitRef(" ");
    if (splits.size() == 2 && splits[0].contains("Python"))
    {
        return Version(splits[1]);
    }
    return Version{};
}

PythonConfigPaths PythonConfigPaths::WindowsBundled()
{
    PythonConfigPaths config{};

    QDir pythonEnvDirPath(QApplication::applicationDirPath() + "/plugins/Python");
    if (pythonEnvDirPath.exists())
    {
        QString qPythonHome = pythonEnvDirPath.path();
        config.m_pythonHome.reset(QStringToWcharArray(qPythonHome));

        QString qPythonPath = QString("%1/DLLs;%1/lib;%1/Lib/site-packages;").arg(qPythonHome);
        config.m_pythonPath.reset(QStringToWcharArray(qPythonPath));
    }
    else
    {
        throw std::runtime_error("Python environment not found, plugin wasn't correctly installed");
    }
    return config;
}

PythonConfigPaths PythonConfigPaths::WindowsCondaEnv(const char * condaPrefix)  {
    PythonConfigPaths config{};

    QDir pythonEnvDirPath(condaPrefix);
    if (pythonEnvDirPath.exists())
    {
        QString qPythonHome = pythonEnvDirPath.path();
        config.m_pythonHome.reset(QStringToWcharArray(qPythonHome));

        QString qPythonPath =
            QString("%1/DLLs;%1/lib;%1/Lib/site-packages;%2/plugins/Python/Lib/site-packages")
                .arg(qPythonHome)
                .arg(QApplication::applicationDirPath());
        config.m_pythonPath.reset(QStringToWcharArray(qPythonPath));
    }
    else
    {
        throw std::runtime_error("Python environment not found, plugin wasn't correctly installed");
    }
    return config;
}

PythonConfigPaths PythonConfigPaths::WindowsVenv(const char *venvPrefix, const PyVenvCfg& cfg)
{
    PythonConfigPaths config{};

    QDir pythonEnvDirPath(venvPrefix);
    if (pythonEnvDirPath.exists())
    {
        QString qPythonHome = pythonEnvDirPath.path();
        config.m_pythonHome.reset(QStringToWcharArray(qPythonHome));

        QString qPythonPath =
            QString("%1/Lib/site-packages;%2/plugins/Python/Lib/site-packages;%3/DLLS;%3/lib")
                .arg(qPythonHome)
                .arg(QApplication::applicationDirPath())
                .arg(cfg.home);

        if (cfg.includeSystemSitesPackages) {
            qPythonPath.append(QString("%1/Lib/site-packages").arg(cfg.home));
        }
        config.m_pythonPath.reset(QStringToWcharArray(qPythonPath));
    }
    else
    {
        throw std::runtime_error("Python environment not found, plugin wasn't correctly installed");
    }
    return config;
}

// Useful link:
// https://docs.python.org/3/c-api/init.html#initialization-finalization-and-threads
PythonPlugin::PythonPlugin(QObject *parent)
    : QObject(parent), ccStdPluginInterface(":/CC/plugin/PythonPlugin/info.json"), m_editor(new QPythonEditor())
{

    try
    {
        configurePython();
    }
    catch (const std::exception &e)
    {
        ccLog::Warning("[PythonPlugin] Failed to configure Python: %s", e.what());
        return;
    }

    Py_SetPythonHome(m_pythonConfig.pythonHome());
    Py_SetPath(m_pythonConfig.pythonPath());

    logPythonHome();
    logPythonPath();

    py::initialize_interpreter();

    connect(m_editor, &QPythonEditor::executionCalled, this, &PythonPlugin::executeEditorCode);
}


void PythonPlugin::configurePython() {
    const char *condaPrefix = std::getenv("CONDA_PREFIX");
    const char *venvPrefix = std::getenv("VIRTUAL_ENV");
    if (condaPrefix)
    {
        ccLog::Print("[PythonPlugin] Conda environment detected (%s)", std::getenv("CONDA_DEFAULT_ENV"));
        QString pythonExePath = QString(condaPrefix) + "/python.exe";
        if (!QFile::exists(pythonExePath))
        {
            pythonExePath = "python";
        }

        Version condaPythonVersion = GetPythonExeVersion(pythonExePath);
        if (condaPythonVersion == PythonVersion) {
            m_pythonConfig = PythonConfigPaths::WindowsCondaEnv(condaPrefix);
        } else {
            ccLog::Warning("[PythonPlugin] Conda environment's Python version (%u.%u.%u)"
                           " does not match the plugin expected version (%u.%u.%u)",
                           condaPythonVersion.major,
                           condaPythonVersion.minor,
                           condaPythonVersion.patch,
                           PythonVersion.major,
                           PythonVersion.minor,
                           PythonVersion.patch);
        }
    }
    else if (venvPrefix)
    {
        ccLog::Print("[PythonPlugin] Virtual environment detected");
        PyVenvCfg cfg = PyVenvCfg::FromFile(QString("%1/pyvenv.cfg").arg(venvPrefix));
        if (cfg.version == PythonVersion)
        {
            m_pythonConfig = PythonConfigPaths::WindowsVenv(venvPrefix, cfg);
        }
        else
        {
            ccLog::Warning("[PythonPlugin] venv's Python version (%u.%u.%u)"
                           " does not match the plugin expected version (%u.%u.%u)",
                           cfg.version.major,
                           cfg.version.minor,
                           cfg.version.patch,
                           PythonVersion.major,
                           PythonVersion.minor,
                           PythonVersion.patch);
        }
    }

    if (!m_pythonConfig.isSet())
    {
        if (condaPrefix || venvPrefix)
        {
            ccLog::Warning("Something went wrong using custom environment configuration"
                           " falling back to bundled Python environment");
        }
        m_pythonConfig = PythonConfigPaths::WindowsBundled();
    }
}

QList<QAction *> PythonPlugin::getActions()
{
    bool enableActions = Py_IsInitialized();

    if (!m_showEditor)
    {
        m_showEditor = new QAction("Show Editor", this);
        m_showEditor->setToolTip("Show the code editor window");
        m_showEditor->setIcon(QIcon(":/CC/plugin/PythonPlugin/images/python-editor-icon.png"));
        connect(m_showEditor, &QAction::triggered, this, &PythonPlugin::showEditor);
        m_showEditor->setEnabled(enableActions);
    }

    if (!m_showREPL)
    {
        m_showREPL = new QAction("Show REPL", this);
        m_showREPL->setToolTip("Show the Python REPL");
        m_showREPL->setIcon(QIcon(":/CC/plugin/PythonPlugin/images/repl-icon.png"));
        connect(m_showREPL, &QAction::triggered, this, &PythonPlugin::showRepl);
        m_showREPL->setEnabled(enableActions);
    }

    if (!m_showDoc)
    {
        m_showDoc = new QAction("Show Documentation", this);
        m_showDoc->setToolTip("Show local documentation");
        m_showDoc->setIcon(m_app->getMainWindow()->style()->standardIcon(QStyle::SP_FileDialogInfoView));
        connect(m_showDoc, &QAction::triggered, this, &PythonPlugin::showDocumentation);
        m_showDoc->setEnabled(enableActions);
    }

    if (!m_showAboutDialog)
    {

        m_showAboutDialog = new QAction("About", this);
        m_showAboutDialog->setToolTip("Show the Python REPL");
        m_showAboutDialog->setIcon(
            m_app->getMainWindow()->style()->standardIcon(QStyle::SP_MessageBoxQuestion));
        connect(m_showAboutDialog, &QAction::triggered, this, &PythonPlugin::showAboutDialog);
        m_showAboutDialog->setEnabled(enableActions);
    }

    return {m_showREPL, m_showEditor, m_showAboutDialog, m_showDoc};
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
        Python::setMainAppInterfaceInstance(m_app);
        m_repl = new ui::QPythonREPL();
        m_repl->show();
    }
}

void PythonPlugin::showEditor()
{
    if (m_editor)
    {
        Python::setMainAppInterfaceInstance(m_app);
        m_editor->show();
        m_editor->raise();
        m_editor->activateWindow();
    }
}

void PythonPlugin::showDocumentation()
{
    if (!m_docView)
    {
        m_docView = new QDocViewer(m_app->getMainWindow());
    }
    m_docView->show();
}

void PythonPlugin::showAboutDialog()
{
    AboutDialog dlg(m_app->getMainWindow());
    dlg.exec();
}

void PythonPlugin::executeEditorCode(const std::string &evalFileName,
                                     const std::string &code,
                                     QListWidget *output)
{
    try
    {
        py::object o = py::module::import("ccinternals").attr("ConsoleREPL")(output);
        PyStdErrOutStreamRedirect redirect{o, o};
        py::exec(code);
    }
    catch (const std::exception &e)
    {
        auto message = new QListWidgetItem(e.what());
        message->setTextColor(Qt::red);
        output->addItem(message);
    }

    size_t n = Python::clearDB();
    ccLog::Print("Python items garbaged: %d", n);
}

PythonPlugin::~PythonPlugin()
{
    Python::unsetMainAppInterfaceInstance();
    Python::unsetCmdLineInterfaceInstance();
    if (Py_IsInitialized())
    {
        py::finalize_interpreter();
    }
}

struct PythonPluginCommand : public ccCommandLineInterface::Command
{
    PythonPluginCommand() : Command("PYTHON", "PYTHON_SCRIPT") {}

    bool process(ccCommandLineInterface &cmd) override
    {
        cmd.print("[PythonPlugin] Starting");
        if (cmd.arguments().empty())
        {
            return cmd.error(
                QString("Missing parameter: parameters filename after \"-%1\"").arg("PYTHON_SCRIPT"));
        }

        QString paramFilename(cmd.arguments().takeFirst());
        cmd.print(QString("[PythonPlugin] python file: '%1'").arg(paramFilename));

        std::vector<wchar_t *> argv;
        argv.reserve(cmd.arguments().size() + 1);
        argv.push_back(QStringToWcharArray(paramFilename));
        while (!cmd.arguments().isEmpty())
        {
            argv.push_back(QStringToWcharArray(cmd.arguments().takeFirst()));
        }

        PySys_SetArgvEx(static_cast<int>(argv.size()), argv.data(), 1);

        bool success{true};
        try
        {
            PyStdErrOutStreamRedirect r{};
            py::eval_file(qPrintable(paramFilename));
        }
        catch (const std::exception &e)
        {
            ccLog::Warning(e.what());
            success = false;
        }

        for (wchar_t *arg : argv)
        {
            delete[] arg;
        }

        py::finalize_interpreter();

        cmd.print(
            QString("[PythonPlugin] Script %1 executed").arg(success ? "successfully" : "unsuccessfully"));
        return success;
    }
};

void PythonPlugin::registerCommands(ccCommandLineInterface *cmd)
{
    cmd->registerCommand(ccCommandLineInterface::Command::Shared(new PythonPluginCommand()));
    Python::setCmdLineInterfaceInstance(cmd);
}

#include "PythonPlugin.moc"