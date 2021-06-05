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
//#                     COPYRIGHT: Chris S Brown                           #
//#                                Thomas Montaigu                         #
//##########################################################################

#include "QPythonEditor.h"
#include "CodeEditor.h"
#include "PythonInterpreter.h"

// qCC
#include "ccMainAppInterface.h"

// Qt
#include <QtWidgets>

static QString RecentFilesKey()
{
    return QStringLiteral("recentFileList");
}

static QString FileKey()
{
    return QStringLiteral("file");
}

QPythonEditor::QPythonEditor(PythonInterpreter *interpreter)
    : Ui::QPythonEditor(), m_settings(new QEditorSettings), m_mdiArea(new QMdiArea(this))
{
    setupUi();
    createActions();
    updateMenus();

    readSettings();
    QCoreApplication::instance()->installEventFilter(this);

    connect(this, &QPythonEditor::executionCalled, interpreter, &PythonInterpreter::executeCode);
    connect(
        interpreter, &PythonInterpreter::executionStarted, this, &QPythonEditor::executionStarted);
    connect(interpreter,
            &PythonInterpreter::executionFinished,
            this,
            &QPythonEditor::executionFinished);
}

void QPythonEditor::closeEvent(QCloseEvent *event)
{
    projectBrowser->hide();
    m_mdiArea->closeAllSubWindows();
    if (m_mdiArea->currentSubWindow())
    {
        event->ignore();
    }
    else
    {
        writeSettings();
        event->accept();
        QCoreApplication::instance()->removeEventFilter(this);
    }
}

void QPythonEditor::newFile()
{
    CodeEditor *child = createChildCodeEditor();
    m_mdiArea->addSubWindow(child);
    child->newFile();
    child->show();
}

void QPythonEditor::promptForFileToOpen()
{
    const QString fileName =
        QFileDialog::getOpenFileName(this, "Open Python Script", QString(), "Python Script (*.py)");
    if (!fileName.isEmpty())
    {
        openFile(fileName);
    }
}

void QPythonEditor::promptForFolderToOpen()
{
    const QString folderName = QFileDialog::getExistingDirectory(this, "Open folder");
    if (!folderName.isEmpty())
    {
        projectBrowser->show();
        projectTreeView->setRootPath(folderName);
    }
}

void QPythonEditor::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

bool QPythonEditor::openFile(const QString &fileName)
{
    if (QMdiSubWindow *existing = findChildCodeEditor(fileName))
    {
        m_mdiArea->setActiveSubWindow(existing);
        return true;
    }
    const bool succeeded = loadFile(fileName);
    if (succeeded)
    {
        statusBar()->showMessage(tr("File loaded"), 2000);
    }
    return succeeded;
}

void QPythonEditor::projectTreeDoubleClicked(const QModelIndex &index)
{
    const QString path = projectTreeView->relativePathAt(index);
    if (QFileInfo(path).isFile())
    {
        openFile(path);
    }
}

bool QPythonEditor::loadFile(const QString &fileName)
{
    CodeEditor *child = createChildCodeEditor();
    const bool succeeded = child->loadFile(fileName);
    if (succeeded)
    {
        m_mdiArea->addSubWindow(child);
        child->show();
    }
    else
    {
        m_mdiArea->removeSubWindow(child);
        child->close();
        delete child;
    }
    QPythonEditor::prependToRecentFiles(fileName);
    return succeeded;
}

static QStringList readRecentFiles(QSettings &settings)
{
    QStringList result;
    const int count = settings.beginReadArray(RecentFilesKey());
    for (int i = 0; i < count; ++i)
    {
        settings.setArrayIndex(i);
        result.append(settings.value(FileKey()).toString());
    }
    settings.endArray();
    return result;
}

static void writeRecentFiles(const QStringList &files, QSettings &settings)
{
    const int count = files.size();
    settings.beginWriteArray(RecentFilesKey());
    for (int i = 0; i < count; ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue(FileKey(), files.at(i));
    }
    settings.endArray();
}

bool QPythonEditor::HasRecentFiles()
{
    QSettings settings(QCoreApplication::organizationName(), SettingsApplicationName());
    const int count = settings.beginReadArray(RecentFilesKey());
    settings.endArray();
    return count > 0;
}

void QPythonEditor::prependToRecentFiles(const QString &fileName)
{
    QSettings settings(QCoreApplication::organizationName(), SettingsApplicationName());

    const QStringList oldRecentFiles = readRecentFiles(settings);
    QStringList recentFiles = oldRecentFiles;
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);
    if (oldRecentFiles != recentFiles)
        writeRecentFiles(recentFiles, settings);

    setRecentFilesVisible(!recentFiles.isEmpty());
}

void QPythonEditor::setRecentFilesVisible(bool visible)
{
    m_recentFileSubMenuAct->setVisible(visible);
    m_recentFileSeparator->setVisible(visible);
}

void QPythonEditor::updateRecentFileActions()
{
    QSettings settings(QCoreApplication::organizationName(), SettingsApplicationName());

    const QStringList recentFiles = readRecentFiles(settings);
    const int count = qMin(MaxRecentFiles, recentFiles.size());
    int i = 0;
    for (; i < count; ++i)
    {
        const QString fileName = QFileInfo(recentFiles.at(i)).fileName();
        m_recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
        m_recentFileActs[i]->setData(recentFiles.at(i));
        m_recentFileActs[i]->setVisible(true);
    }
    for (; i < MaxRecentFiles; ++i)
    {
        m_recentFileActs[i]->setVisible(false);
    }
}

void QPythonEditor::openRecentFile()
{
    if (const auto *action = qobject_cast<const QAction *>(sender()))
    {
        openFile(action->data().toString());
    }
}

bool QPythonEditor::eventFilter(QObject *obj, QEvent *e)
{
    switch (e->type())
    {
    case QEvent::Shortcut:
    {
        auto *sev = static_cast<QShortcutEvent *>(e);
        if (sev->isAmbiguous())
        {
            for (const auto &action : actions())
            {
                if (action->shortcut() == sev->key())
                {
                    action->trigger(); // Trigger the action that matches the ambiguous shortcut
                                       // event.
                    return true;
                }
            }
        }
    }
    default:
        break;
    }
    return false;
}

void QPythonEditor::save()
{
    if (activeChildCodeEditor() && activeChildCodeEditor()->save())
    {
        statusBar()->showMessage(tr("File saved"), 2000);
    }
}

void QPythonEditor::saveAs()
{
    CodeEditor *child = activeChildCodeEditor();
    if (child && child->saveAs())
    {
        statusBar()->showMessage(tr("File saved"), 2000);
        QPythonEditor::prependToRecentFiles(child->currentFile());
    }
}

#ifndef QT_NO_CLIPBOARD
void QPythonEditor::cut()
{
    if (activeChildCodeEditor())
    {
        activeChildCodeEditor()->cut();
    }
}

void QPythonEditor::copy()
{
    if (activeChildCodeEditor())
    {
        activeChildCodeEditor()->copy();
    }
}

void QPythonEditor::paste()
{
    if (activeChildCodeEditor())
    {
        activeChildCodeEditor()->paste();
    }
}
#endif

void QPythonEditor::comment()
{
    if (activeChildCodeEditor())
    {
        activeChildCodeEditor()->comment();
    }
}
void QPythonEditor::uncomment()
{
    if (activeChildCodeEditor())
    {
        activeChildCodeEditor()->uncomment();
    }
}
void QPythonEditor::indentMore()
{
    if (activeChildCodeEditor())
    {
        activeChildCodeEditor()->indentMore();
    }
}
void QPythonEditor::indentLess()
{
    if (activeChildCodeEditor())
    {
        activeChildCodeEditor()->indentLess();
    }
}

void QPythonEditor::executionStarted()
{
    actionRun->setEnabled(false);
    if (activeChildCodeEditor())
    {
        statusbar->showMessage(QString("Executing %1").arg(activeChildCodeEditor()->windowTitle()));
    }
}

void QPythonEditor::executionFinished()
{
    actionRun->setEnabled(true);
    statusbar->clearMessage();
}

void QPythonEditor::setupUi()
{
    Ui::QPythonEditor::setupUi(this);

    // Setup MDI Area

    setCentralWidget(m_mdiArea);
    m_mdiArea->showMaximized();
    connect(m_mdiArea, &QMdiArea::subWindowActivated, this, &QPythonEditor::updateMenus);

    // setup ViewToolBar
    viewsToolBar->setMovable(false);
    projectBrowser->toggleViewAction()->setEnabled(true);
    viewsToolBar->addAction(projectBrowser->toggleViewAction());
    scriptOutputConsoleDock->toggleViewAction()->setEnabled(true);
    viewsToolBar->addAction(scriptOutputConsoleDock->toggleViewAction());

    // Setup Project View
    projectBrowser->hide();
    connect(projectTreeView,
            &ProjectView::doubleClicked,
            this,
            &QPythonEditor::projectTreeDoubleClicked);

    scriptOutputConsoleDock->hide();
}

void QPythonEditor::createActions()
{
    connect(actionNew, &QAction::triggered, this, &QPythonEditor::newFile);
    connect(actionSave, &QAction::triggered, this, &QPythonEditor::save);
    connect(actionOpen, &QAction::triggered, this, &QPythonEditor::promptForFileToOpen);
    connect(actionOpenFolder, &QAction::triggered, this, &QPythonEditor::promptForFolderToOpen);
    connect(actionSaveAs, &QAction::triggered, this, &QPythonEditor::saveAs);
    connect(actionRun, &QAction::triggered, this, &QPythonEditor::runExecute);
    connect(actionClose, &QAction::triggered, this, [=]() { close(); });
    connect(actionSettings, &QAction::triggered, m_settings, &QEditorSettings::show);

    menuFile->addSeparator();

    QMenu *recentMenu = menuFile->addMenu(tr("Recent..."));
    connect(recentMenu, &QMenu::aboutToShow, this, &QPythonEditor::updateRecentFileActions);
    m_recentFileSubMenuAct = recentMenu->menuAction();

    for (auto &recentFileAct : m_recentFileActs)
    {
        recentFileAct = recentMenu->addAction(QString());
        connect(recentFileAct, &QAction::triggered, this, &QPythonEditor::openRecentFile);
        recentFileAct->setVisible(false);
    }

    m_recentFileSeparator = menuFile->addSeparator();

    setRecentFilesVisible(QPythonEditor::HasRecentFiles());
    menuFile->addSeparator();

    actionCut->setShortcuts(QKeySequence::Cut);
    actionCopy->setShortcuts(QKeySequence::Copy);
    actionPaste->setShortcuts(QKeySequence::Paste);
    connect(actionCut, &QAction::triggered, this, &QPythonEditor::cut);
    connect(actionCopy, &QAction::triggered, this, &QPythonEditor::copy);
    connect(actionPaste, &QAction::triggered, this, &QPythonEditor::paste);

    connect(actionComment, &QAction::triggered, this, &QPythonEditor::comment);
    connect(actionUncomment, &QAction::triggered, this, &QPythonEditor::uncomment);
    connect(actionIndentMore, &QAction::triggered, this, &QPythonEditor::indentMore);
    connect(actionIndentLess, &QAction::triggered, this, &QPythonEditor::indentLess);

    m_windowMenu = menuBar()->addMenu(tr("&Window"));
    connect(m_windowMenu, &QMenu::aboutToShow, this, &QPythonEditor::updateWindowMenu);

    m_closeAct = new QAction(tr("Cl&ose"), this);
    m_closeAct->setStatusTip(tr("Close the active window"));
    connect(m_closeAct, &QAction::triggered, m_mdiArea, &QMdiArea::closeActiveSubWindow);

    m_closeAllAct = new QAction(tr("Close &All"), this);
    m_closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(m_closeAllAct, &QAction::triggered, m_mdiArea, &QMdiArea::closeAllSubWindows);

    m_tileAct = new QAction(tr("&Tile"), this);
    m_tileAct->setStatusTip(tr("Tile the windows"));
    connect(m_tileAct, &QAction::triggered, m_mdiArea, &QMdiArea::tileSubWindows);

    m_cascadeAct = new QAction(tr("&Cascade"), this);
    m_cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(m_cascadeAct, &QAction::triggered, m_mdiArea, &QMdiArea::cascadeSubWindows);

    m_nextAct = new QAction(tr("Ne&xt"), this);
    m_nextAct->setShortcuts(QKeySequence::NextChild);
    m_nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(m_nextAct, &QAction::triggered, m_mdiArea, &QMdiArea::activateNextSubWindow);

    m_previousAct = new QAction(tr("Pre&vious"), this);
    m_previousAct->setShortcuts(QKeySequence::PreviousChild);
    m_previousAct->setStatusTip(tr("Move the focus to the previous "
                                   "window"));
    connect(m_previousAct, &QAction::triggered, m_mdiArea, &QMdiArea::activatePreviousSubWindow);

    m_windowMenuSeparatorAct = new QAction(this);
    m_windowMenuSeparatorAct->setSeparator(true);

    updateWindowMenu();

    menuBar()->addSeparator();

    addAction(actionNew); // Actions must be added to be able to find shortcuts in event filter
    addAction(actionSave);
    addAction(actionOpen);
    addAction(actionSaveAs);
    addAction(actionRun);
    addAction(actionClose);
    addAction(actionComment);
    addAction(actionUncomment);
    addAction(actionIndentMore);
    addAction(actionIndentLess);
}

void QPythonEditor::updateMenus()
{
    const bool hasChildCodeEditor = (activeChildCodeEditor() != nullptr);
    actionSave->setEnabled(hasChildCodeEditor);
    actionSaveAs->setEnabled(hasChildCodeEditor);
    actionRun->setEnabled(hasChildCodeEditor);
    m_closeAct->setEnabled(hasChildCodeEditor);
    m_closeAllAct->setEnabled(hasChildCodeEditor);
    m_tileAct->setEnabled(hasChildCodeEditor);
    m_cascadeAct->setEnabled(hasChildCodeEditor);
    m_nextAct->setEnabled(hasChildCodeEditor);
    m_previousAct->setEnabled(hasChildCodeEditor);
    m_windowMenuSeparatorAct->setVisible(hasChildCodeEditor);
    actionComment->setEnabled(hasChildCodeEditor);
    actionUncomment->setEnabled(hasChildCodeEditor);
    actionIndentMore->setEnabled(hasChildCodeEditor);
    actionIndentLess->setEnabled(hasChildCodeEditor);

#ifndef QT_NO_CLIPBOARD
    actionPaste->setEnabled(hasChildCodeEditor);
    const bool hasSelection =
        (activeChildCodeEditor() && activeChildCodeEditor()->textCursor().hasSelection());
    actionCut->setEnabled(hasSelection);
    actionCopy->setEnabled(hasSelection);
#endif
}

void QPythonEditor::updateWindowMenu()
{
    m_windowMenu->clear();
    m_windowMenu->addAction(m_closeAct);
    m_windowMenu->addAction(m_closeAllAct);
    m_windowMenu->addSeparator();
    m_windowMenu->addAction(m_tileAct);
    m_windowMenu->addAction(m_cascadeAct);
    m_windowMenu->addSeparator();
    m_windowMenu->addAction(m_nextAct);
    m_windowMenu->addAction(m_previousAct);
    m_windowMenu->addAction(m_windowMenuSeparatorAct);

    QList<QMdiSubWindow *> windows = m_mdiArea->subWindowList();
    m_windowMenuSeparatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i)
    {
        QMdiSubWindow *mdiSubWindow = windows.at(i);
        auto *child = qobject_cast<CodeEditor *>(mdiSubWindow->widget());

        QString text;
        if (i < 9)
        {
            text = tr("&%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
        }
        else
        {
            text = tr("%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
        }
        QAction *action = m_windowMenu->addAction(text);
        connect(action, &QAction::triggered, mdiSubWindow, [this, mdiSubWindow]() {
            m_mdiArea->setActiveSubWindow(mdiSubWindow);
        });
        action->setCheckable(true);
        action->setChecked(child == activeChildCodeEditor());
    }
}

CodeEditor *QPythonEditor::createChildCodeEditor()
{
    auto *child = new CodeEditor(this->m_settings);

#ifndef QT_NO_CLIPBOARD
    connect(child, &QPlainTextEdit::copyAvailable, actionCut, &QAction::setEnabled);
    connect(child, &QPlainTextEdit::copyAvailable, actionCopy, &QAction::setEnabled);
#endif

    return child;
}

void QPythonEditor::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), SettingsApplicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    }
    else
    {
        restoreGeometry(geometry);
    }
}

void QPythonEditor::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), SettingsApplicationName());
    settings.setValue("geometry", saveGeometry());
}

CodeEditor *QPythonEditor::activeChildCodeEditor() const
{
    if (QMdiSubWindow *activeSubWindow = m_mdiArea->activeSubWindow())
    {
        return qobject_cast<CodeEditor *>(activeSubWindow->widget());
    }
    return nullptr;
}

QMdiSubWindow *QPythonEditor::findChildCodeEditor(const QString &fileName) const
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    for (QMdiSubWindow *window : m_mdiArea->subWindowList())
    {
        auto *mdiChild = qobject_cast<CodeEditor *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
        {
            return window;
        }
    }
    return nullptr;
}

QString QPythonEditor::SettingsApplicationName()
{
    return QString(QCoreApplication::applicationName()).append(":PythonPlugin");
}

void QPythonEditor::runExecute()
{
    if (activeChildCodeEditor())
    {
        scriptOutputConsoleDock->show();
        this->scriptOutputConsole->clear();
        Q_EMIT executionCalled(qPrintable(activeChildCodeEditor()->toPlainText()),
                               this->scriptOutputConsole);
    }
}
