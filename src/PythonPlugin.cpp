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

#include "AboutDialog.h"
#include "PrivateRuntime.h"
#include "PythonPlugin.h"
#include "PythonStdErrOutRedirect.h"
#include "QPythonEditor.h"
#include "QPythonREPL.h"
#include "Utilities.h"

#include <QWebEngineHistory>
#include <QWebEngineSettings>
#include <QWebEngineView>

#include <memory>

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

    void loadPreviousPage()
    {
        m_viewEngine->history()->back();
    }

    void loadNextPage()
    {
        m_viewEngine->history()->forward();
    }

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

// Useful link:
// https://docs.python.org/3/c-api/init.html#initialization-finalization-and-threads
PythonPlugin::PythonPlugin(QObject *parent)
    : QObject(parent), ccStdPluginInterface(":/CC/plugin/PythonPlugin/info.json"),
      m_editor(new QPythonEditor(&m_interp)), m_interp(nullptr)
{
    m_interp.initialize();

    logPythonHome();
    logPythonPath();

    connect(&m_interp, &PythonInterpreter::executionFinished, Python::clearDB);
}

QList<QAction *> PythonPlugin::getActions()
{
    bool enableActions = PythonInterpreter::isInitialized();

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
        m_repl = new QPythonREPL(&m_interp);
        m_repl->show();
    }
}

void PythonPlugin::showEditor()
{
    if (m_editor)
    {
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

PythonPlugin::~PythonPlugin()
{
    Python::unsetMainAppInterfaceInstance();
    Python::unsetCmdLineInterfaceInstance();
    PythonInterpreter::finalize();
}

struct PythonPluginCommand : public ccCommandLineInterface::Command
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
        bool success = interpreter->executeFile(qPrintable(args.filepath));


        // We have to finalize the interpreter here, as it relies
        // on the argv we set earlier, and these argv will be deleted at the
        // end of this function
        PythonInterpreter::finalize();

        cmd.print(
            QString("[PythonPlugin] Script %1 executed").arg(success ? "successfully" : "unsuccessfully"));
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
                return cmd.error(
                    QString("Missing parameter: parameters filename after \"-%1\"").arg("PYTHON_SCRIPT"));
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
    cmd->registerCommand(ccCommandLineInterface::Command::Shared(new PythonPluginCommand(&m_interp)));
    Python::setCmdLineInterfaceInstance(cmd);
}

void PythonPlugin::setMainAppInterface(ccMainAppInterface *app)
{
    ccStdPluginInterface::setMainAppInterface(app);
    Python::setMainAppInterfaceInstance(m_app);
}

#include "PythonPlugin.moc"