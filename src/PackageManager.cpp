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
#include "PackageManager.h"
#include "PythonInterpreter.h"
#include "Resources.h"

#include <QDialog>
#include <QDir>
#include <QIcon>
#include <QInputDialog>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QProcess>
#include <QRegExp>
#include <QTableWidgetItem>
#include <QTextCodec>
#include <QtGlobal>

#include <ccLog.h>

#include <ui_InstallDialog.h>

#include "Utilities.h"

#if defined(Q_OS_WIN32)
#include <Windows.h>
#endif

#if defined(Q_OS_WIN32)
static BOOL GetFolderRights(LPCTSTR folderName, DWORD genericAccessRights, DWORD *grantedRights)
{
    DWORD length = 0;

    constexpr SECURITY_INFORMATION requestedInformation =
        OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION;

    /* Get the needed length */
    GetFileSecurity(folderName, requestedInformation, nullptr, NULL, &length);

    if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
    {
        return FALSE;
    }

    PSECURITY_DESCRIPTOR security = LocalAlloc(LPTR, length);
    if (GetFileSecurity(folderName, requestedInformation, security, length, &length) == FALSE)
    {
        LocalFree(security);
        return FALSE;
    }

    DWORD desiredAccess = TOKEN_IMPERSONATE | TOKEN_QUERY | TOKEN_DUPLICATE | STANDARD_RIGHTS_READ;
    HANDLE hToken = nullptr;
    if (OpenProcessToken(GetCurrentProcess(), desiredAccess, &hToken) == FALSE)
    {
        LocalFree(security);
        return FALSE;
    }

    HANDLE hImpersonatedToken = nullptr;
    if (DuplicateToken(hToken, SecurityImpersonation, &hImpersonatedToken) == FALSE)
    {
        CloseHandle(hToken);
        LocalFree(security);
        return FALSE;
    }

    GENERIC_MAPPING mapping = {0xFFFFFFFF};
    PRIVILEGE_SET privileges = {0};
    DWORD grantedAccess = 0, privilegesLength = sizeof(privileges);
    BOOL result = FALSE;

    mapping.GenericRead = FILE_GENERIC_READ;
    mapping.GenericWrite = FILE_GENERIC_WRITE;
    mapping.GenericExecute = FILE_GENERIC_EXECUTE;
    mapping.GenericAll = FILE_ALL_ACCESS;

    MapGenericMask(&genericAccessRights, &mapping);
    if (AccessCheck(security,
                    hImpersonatedToken,
                    genericAccessRights,
                    &mapping,
                    &privileges,
                    &privilegesLength,
                    &grantedAccess,
                    &result) == FALSE)
    {
        CloseHandle(hImpersonatedToken);
        CloseHandle(hToken);
        LocalFree(security);
        return FALSE;
    }

    *grantedRights = grantedAccess;

    CloseHandle(hImpersonatedToken);
    CloseHandle(hToken);
    LocalFree(security);

    return TRUE;
}

static bool HasReadWriteAccessToFolder(const QString &folderPath)
{
    constexpr DWORD access_mask = MAXIMUM_ALLOWED;
    DWORD grant = 0;
#if defined(UNICODE)
    const std::wstring str = folderPath.toStdWString();
    BOOL ret = GetFolderRights(str.c_str(), access_mask, &grant);
#else
    const std::string str = folderPath.toStdString();
    BOOL ret = GetFolderRights(str.c_str(), access_mask, &grant);
#endif

    if (ret == FALSE)
    {
        plgWarning() << "Failed to get access rights for path '" << folderPath << '\n';
        return false;
    }

    bool hasRead = false;
    if (((grant & GENERIC_READ) == GENERIC_READ) ||
        ((grant & FILE_GENERIC_READ) == FILE_GENERIC_READ))
    {
        hasRead = true;
    }

    bool hasWrite = false;
    if (((grant & GENERIC_WRITE) == GENERIC_WRITE) ||
        ((grant & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE))
    {
        hasWrite = true;
    }

    bool hasExecute = false;
    if (((grant & GENERIC_EXECUTE) == GENERIC_EXECUTE) ||
        ((grant & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE))
    {
        hasExecute = true;
    }
    return hasRead && hasWrite && hasExecute;
}
#endif // defined(Q_OS_WIN32)

class InstallDialog : public QDialog
{
    Q_OBJECT
  public:
    explicit InstallDialog(QWidget *parent = nullptr) : QDialog(parent), m_ui(new Ui_InstallDialog)
    {
        m_ui->setupUi(this);
    }
    bool force() const
    {
        return m_ui->forceCheckBox;
    }
    bool upgrade() const
    {
        return m_ui->updateCheckBox;
    }
    QString packageName() const
    {
        return m_ui->packageNameEdit->text();
    }

  private:
    Ui_InstallDialog *m_ui;
};

class CommandOutputDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit CommandOutputDialog(QWidget *parent = nullptr)
        : QDialog(parent), m_display(new QPlainTextEdit(this))
    {
        setWindowTitle("pip output");
        m_display->setReadOnly(true);
        auto *widgetLayout = new QVBoxLayout;
        widgetLayout->addWidget(m_display);
        setLayout(widgetLayout);
        resize(600, 300);
    }

    void appendPlainText(const QString &text) const
    {
        m_display->appendPlainText(text);
    }

    void clear() const
    {
        m_display->clear();
    }

  private:
    QPlainTextEdit *m_display;
};

PackageManager::PackageManager(const PythonConfig &config, QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui_PackageManager),
      m_pythonProcess(new QProcess),
      m_outputDialog(new CommandOutputDialog(this)),
      m_shouldUseUserOption(false)
{
    m_ui->setupUi(this);
    connect(m_pythonProcess, &QProcess::started, [this]() { setBusy(true); });
    connect(m_pythonProcess,
            static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [this](int, QProcess::ExitStatus) { setBusy(false); });

    setWindowIcon(QIcon(PACKAGE_MANAGER_ICON_PATH));

    m_ui->installedPackagesView->setColumnCount(2);
    m_ui->installedPackagesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_ui->installedPackagesView->setHorizontalHeaderLabels({"Package Name", "Version"});
    m_ui->installedPackagesView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(m_ui->installedPackagesView,
            &QTableWidget::itemSelectionChanged,
            this,
            &PackageManager::handleSelectionChanged);

    m_ui->uninstallBtn->setEnabled(false);
    connect(m_ui->installBtn, &QPushButton::clicked, this, &PackageManager::handleInstallPackage);
    connect(
        m_ui->uninstallBtn, &QPushButton::clicked, this, &PackageManager::handleUninstallPackage);

    connect(m_ui->searchBar, &QLineEdit::returnPressed, this, &PackageManager::handleSearch);
    config.preparePythonProcess(*m_pythonProcess);
    refreshInstalledPackagesList();

    m_ui->installBtn->setEnabled(true);
    m_ui->messageFrame->hide();

    switch (config.type())
    {
    // The main intent of checking access rights of venv is for the Windows bundled
    // env, but checking for all venv won't hurt.
    // on Windows the bundled env  is very likely to be installed in
    // "C:\Programs\Cloud Compare\plugins\Python" and that requires admin rights to add/modify.
    // It is better to notify user and prevent them from trying something that will faill.
    case PythonConfig::Type::Venv:
    case PythonConfig::Type::Conda:
    case PythonConfig::Type::Unknown:
    {
#if defined Q_OS_WIN32
        // On Windows we use a custom function because isWritable from Qt was not correct
        // for our use case (its mentionned in their doc)
        const bool hasEnoughRights = HasReadWriteAccessToFolder(config.pythonHome());
#else
        const QFileInfo dirInfo(config.pythonHome());
        const bool hasEnoughRights = dirInfo.isWritable();
#endif
        if (!hasEnoughRights)
        {
            m_ui->installBtn->setEnabled(false);
            m_ui->messageIconLabel->setPixmap(QApplication::style()
                                                  ->standardIcon(QStyle::SP_MessageBoxCritical)
                                                  .pixmap({64, 64}));
            m_ui->messageTextLabel->setText("Admin rights are required to be able to install "
                                            "packages in the current environment");
            m_ui->messageFrame->show();
        }
    }
    break;
    case PythonConfig::Type::System:
        m_shouldUseUserOption = true;
        break;
    }
}

void PackageManager::refreshInstalledPackagesList()
{
    m_pythonProcess->setProcessChannelMode(QProcess::SeparateChannels);

    const QStringList arguments = {"-m", "pip", "list"};
    m_pythonProcess->setArguments(arguments);

    QEventLoop loop;
    QObject::connect(
        m_pythonProcess,
        static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
        &loop,
        &QEventLoop::quit);
    QObject::connect(m_pythonProcess, &QProcess::errorOccurred, &loop, &QEventLoop::quit);
    m_pythonProcess->start(QIODevice::ReadOnly);
    if (m_pythonProcess->state() != QProcess::ProcessState::Starting &&
        m_pythonProcess->state() != QProcess::ProcessState::Running)
    {
        ccLog::Warning("Failed to start python process");
        return;
    }
    loop.exec();

    if (m_pythonProcess->exitStatus() != QProcess::ExitStatus::NormalExit)
    {
        const QString errorMsg =
            QString("Failed to list installed packages: '%1'").arg(m_pythonProcess->errorString());
        QMessageBox::critical(this, "Package Manager Error", errorMsg);
        return;
    }

    const QString output =
        QTextCodec::codecForName("utf-8")->toUnicode(m_pythonProcess->readAllStandardOutput());

    const QVector<QStringRef> lines = output.splitRef("\n");

    const QRegExp regex(R"((\S*)(?:\s*)(\S*)(?:\s*)(\S?))");

    // First line is a header, second is separator
    // and last one seems to always be empty
    if (lines.size() <= 3)
    {
        return;
    }

    m_ui->installedPackagesView->setRowCount(lines.size() - 3);

    for (int i{2}; i < lines.size() - 1; ++i)
    {
        const QStringRef &currentLine = lines[i];

        // Do it this way to avoid an extra allocation
        int pos = regex.indexIn(currentLine.toString());
        if (pos != -1)
        {
            for (int j = 1; j < 3; ++j)
            {
                const QString lol = regex.cap(j);
                auto *thing = new QTableWidgetItem(lol);
                if (j != 1)
                {
                    thing->setFlags(thing->flags() & ~Qt::ItemFlag::ItemIsSelectable);
                }
                m_ui->installedPackagesView->setItem(i - 2, j - 1, thing);
            }
        }
    }

    const QString errorOutput =
        QTextCodec::codecForName("utf-8")->toUnicode(m_pythonProcess->readAllStandardError());

    if (!errorOutput.isEmpty())
    {
        ccLog::Warning(errorOutput);
    }

    m_pythonProcess->setProcessChannelMode(QProcess::MergedChannels);
}

void PackageManager::handleInstallPackage()
{
    InstallDialog installDialog(this);
    if (installDialog.exec() != QDialog::Accepted)
    {
        return;
    };

    const QString packageName = installDialog.packageName();

    if (packageName.isEmpty())
    {
        return;
    }

    QStringList arguments = {"-m", "pip", "install", packageName};
    if (installDialog.force())
    {
        arguments.push_back("--force");
    }
    if (installDialog.upgrade())
    {
        arguments.push_back("--upgrade");
    }
    if (m_shouldUseUserOption)
    {
        arguments.push_back("--user");
    }
    executeCommand(arguments);

    if (m_pythonProcess->exitCode() != 0)
    {
        ccLog::Error("Failed to run install commands", m_pythonProcess->error());
        ccLog::Warning(m_pythonProcess->errorString());
    }
    refreshInstalledPackagesList();
}

void PackageManager::handleUninstallPackage()
{
    const QList<QTableWidgetItem *> selectedItems = m_ui->installedPackagesView->selectedItems();

    if (selectedItems.isEmpty())
    {
        return;
    }

    for (const QTableWidgetItem *item : selectedItems)
    {
        const QString packageName = item->text();
        QMessageBox::StandardButton choice = QMessageBox::question(
            this, "Confirm", QString("Do you really want to uninstall: '%1' ?").arg(packageName));

        if (choice != QMessageBox::StandardButton::Yes)
        {
            continue;
        }
        const QStringList arguments = {"-m", "pip", "uninstall", "--yes", packageName};
        executeCommand(arguments);

        if (m_pythonProcess->exitCode() != 0)
        {
            ccLog::Error("Failed to run uninstall commands", m_pythonProcess->error());
            ccLog::Warning(m_pythonProcess->errorString());
        }
    }
    refreshInstalledPackagesList();
}

void PackageManager::handleSearch()
{
    const QString searchString = m_ui->searchBar->text();
    QTableWidget *table = m_ui->installedPackagesView;

    if (searchString.isEmpty())
    {
        for (int i = 0; i < table->rowCount(); ++i)
        {
            table->setRowHidden(i, false);
        }
    }
    else
    {
        for (int i = 0; i < table->rowCount(); ++i)
        {
            bool match = false;
            for (int j = 0; j < table->columnCount(); ++j)
            {
                QTableWidgetItem *item = table->item(i, j);
                if (item->text().contains(searchString))
                {
                    match = true;
                    break;
                }
            }
            table->setRowHidden(i, !match);
        }
    }
}

void PackageManager::executeCommand(const QStringList &arguments)
{
    m_outputDialog->show();
    m_outputDialog->clear();
    m_pythonProcess->setArguments(arguments);
    m_pythonProcess->start(QIODevice::ReadOnly);
    QTextCodec *utf8Codec = QTextCodec::codecForName("utf-8");

    while (m_pythonProcess->state() != QProcess::ProcessState::NotRunning)
    {
        if (m_pythonProcess->waitForReadyRead())
        {
            const QString output = utf8Codec->toUnicode(m_pythonProcess->readAll());
            m_outputDialog->appendPlainText(output);
            QApplication::processEvents();
        }
    }
    m_outputDialog->exec();
}

void PackageManager::handleSelectionChanged()
{
    m_ui->uninstallBtn->setEnabled(!m_ui->installedPackagesView->selectedItems().isEmpty());
}

void PackageManager::setBusy(bool isBusy)
{
    m_ui->installBtn->setEnabled(!isBusy);
    m_ui->uninstallBtn->setEnabled(!isBusy);
}

PackageManager::~PackageManager() noexcept
{
    delete m_ui;
}

#include "PackageManager.moc"
