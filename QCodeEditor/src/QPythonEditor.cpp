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
#include "ProjectViewContextMenu.h"
#include "PythonInterpreter.h"

// qCC
#include "ccMainAppInterface.h"

// Qt
#include <QtWidgets>

QPythonEditor::QPythonEditor(PythonInterpreter *interpreter) : Ui::QPythonEditor()
{
    setupUi(this);
    mdiArea = new QMdiArea(this);
    setCentralWidget(mdiArea);
    mdiArea->showMaximized();
    connect(mdiArea, &QMdiArea::subWindowActivated, this, &QPythonEditor::updateMenus);

    this->settings = new QEditorSettings;
    initProjectView();
    createActions();
    createStatusBar();
    updateMenus();

    readSettings();
    QCoreApplication::instance()->installEventFilter(this);

    connect(this, &QPythonEditor::executionCalled, interpreter, &PythonInterpreter::executeCode);
    connect(interpreter, &PythonInterpreter::executionStarted, this, &QPythonEditor::executionStarted);
    connect(interpreter, &PythonInterpreter::executionFinished, this, &QPythonEditor::executionFinished);

    viewsToolBar->setMovable(false);
    projectBrowser->toggleViewAction()->setEnabled(true);
    viewsToolBar->addAction(projectBrowser->toggleViewAction());
    scriptOutputConsoleDock->toggleViewAction()->setEnabled(true);
    viewsToolBar->addAction(scriptOutputConsoleDock->toggleViewAction());
}

void QPythonEditor::closeEvent(QCloseEvent *event)
{
    projectBrowser->hide();
    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow())
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
    mdiArea->addSubWindow(child);
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
        fileSystemModel->setRootPath(folderName);
        PBtreeView->setRootIndex(fileSystemModel->index(folderName));
        this->projectBrowser->show();
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
        mdiArea->setActiveSubWindow(existing);
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
    const QString path = fileSystemModel->filePath(index);
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
        mdiArea->addSubWindow(child);
        child->show();
    }
    else
    {
        mdiArea->removeSubWindow(child);
        child->close();
        delete child;
    }
    QPythonEditor::prependToRecentFiles(fileName);
    return succeeded;
}

static inline QString recentFilesKey()
{
    return QStringLiteral("recentFileList");
}
static inline QString fileKey()
{
    return QStringLiteral("file");
}

static QStringList readRecentFiles(QSettings &settings)
{
    QStringList result;
    const int count = settings.beginReadArray(recentFilesKey());
    for (int i = 0; i < count; ++i)
    {
        settings.setArrayIndex(i);
        result.append(settings.value(fileKey()).toString());
    }
    settings.endArray();
    return result;
}

static void writeRecentFiles(const QStringList &files, QSettings &settings)
{
    const int count = files.size();
    settings.beginWriteArray(recentFilesKey());
    for (int i = 0; i < count; ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue(fileKey(), files.at(i));
    }
    settings.endArray();
}

bool QPythonEditor::hasRecentFiles()
{
    QSettings settings(QCoreApplication::organizationName(), settingsApplicationName());
    const int count = settings.beginReadArray(recentFilesKey());
    settings.endArray();
    return count > 0;
}

void QPythonEditor::prependToRecentFiles(const QString &fileName)
{
    QSettings settings(QCoreApplication::organizationName(), settingsApplicationName());

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
    recentFileSubMenuAct->setVisible(visible);
    recentFileSeparator->setVisible(visible);
}

void QPythonEditor::updateRecentFileActions()
{
    QSettings settings(QCoreApplication::organizationName(), settingsApplicationName());

    const QStringList recentFiles = readRecentFiles(settings);
    const int count = qMin(int(MaxRecentFiles), recentFiles.size());
    int i = 0;
    for (; i < count; ++i)
    {
        const QString fileName = QFileInfo(recentFiles.at(i)).fileName();
        recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
        recentFileActs[i]->setData(recentFiles.at(i));
        recentFileActs[i]->setVisible(true);
    }
    for (; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i]->setVisible(false);
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
                    action->trigger(); // Trigger the action that matches the ambiguous shortcut event.
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

void QPythonEditor::createActions()
{
    connect(actionNew, &QAction::triggered, this, &QPythonEditor::newFile);
    connect(actionSave, &QAction::triggered, this, &QPythonEditor::save);
    connect(actionOpen, &QAction::triggered, this, &QPythonEditor::promptForFileToOpen);
    connect(actionOpenFolder, &QAction::triggered, this, &QPythonEditor::promptForFolderToOpen);
    connect(actionSave_As, &QAction::triggered, this, &QPythonEditor::saveAs);
    connect(actionRun, &QAction::triggered, this, &QPythonEditor::runExecute);
    connect(actionClose, &QAction::triggered, this, [=]() { close(); });
    connect(actionSettings, &QAction::triggered, this, [this]() { this->settings->show(); });

    menuFile->addSeparator();

    QMenu *recentMenu = menuFile->addMenu(tr("Recent..."));
    connect(recentMenu, &QMenu::aboutToShow, this, &QPythonEditor::updateRecentFileActions);
    recentFileSubMenuAct = recentMenu->menuAction();

    for (auto &recentFileAct : recentFileActs)
    {
        recentFileAct = recentMenu->addAction(QString());
        connect(recentFileAct, &QAction::triggered, this, &QPythonEditor::openRecentFile);
        recentFileAct->setVisible(false);
    }

    recentFileSeparator = menuFile->addSeparator();

    setRecentFilesVisible(QPythonEditor::hasRecentFiles());
    menuFile->addSeparator();

    actionCu_t->setShortcuts(QKeySequence::Cut);
    action_Copy->setShortcuts(QKeySequence::Copy);
    action_Paste->setShortcuts(QKeySequence::Paste);
    connect(actionCu_t, &QAction::triggered, this, &QPythonEditor::cut);
    connect(action_Copy, &QAction::triggered, this, &QPythonEditor::copy);
    connect(action_Paste, &QAction::triggered, this, &QPythonEditor::paste);

    connect(actionComment, &QAction::triggered, this, &QPythonEditor::comment);
    connect(actionUncomment, &QAction::triggered, this, &QPythonEditor::uncomment);
    connect(actionIndent_More, &QAction::triggered, this, &QPythonEditor::indentMore);
    connect(actionIndent_Less, &QAction::triggered, this, &QPythonEditor::indentLess);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    connect(windowMenu, &QMenu::aboutToShow, this, &QPythonEditor::updateWindowMenu);

    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, &QAction::triggered, mdiArea, &QMdiArea::closeActiveSubWindow);

    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, &QAction::triggered, mdiArea, &QMdiArea::closeAllSubWindows);

    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, &QAction::triggered, mdiArea, &QMdiArea::tileSubWindows);

    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, &QAction::triggered, mdiArea, &QMdiArea::cascadeSubWindows);

    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAct, &QAction::triggered, mdiArea, &QMdiArea::activateNextSubWindow);

    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous "
                                 "window"));
    connect(previousAct, &QAction::triggered, mdiArea, &QMdiArea::activatePreviousSubWindow);

    windowMenuSeparatorAct = new QAction(this);
    windowMenuSeparatorAct->setSeparator(true);

    updateWindowMenu();

    menuBar()->addSeparator();

    addAction(actionNew); // Actions must be added to be able to find shortcuts in event filter
    addAction(actionSave);
    addAction(actionOpen);
    addAction(actionSave_As);
    addAction(actionRun);
    addAction(actionClose);
    addAction(actionComment);
    addAction(actionUncomment);
    addAction(actionIndent_More);
    addAction(actionIndent_Less);
}

void QPythonEditor::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void QPythonEditor::initProjectView()
{
    projectBrowser->hide();
    fileSystemModel = new QFileSystemModel;
    PBtreeView->setModel(fileSystemModel);

    projectViewContextMenu = new ProjectViewContextMenu(PBtreeView);
    PBtreeView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

    for (int i{1}; i < PBtreeView->size().width(); ++i)
    {
        PBtreeView->hideColumn(i);
    }
    connect(PBtreeView, &QTreeView::doubleClicked, this, &QPythonEditor::projectTreeDoubleClicked);
    connect(PBtreeView,
            &QTreeView::customContextMenuRequested,
            projectViewContextMenu,
            &ProjectViewContextMenu::requested);
}

void QPythonEditor::updateMenus()
{
    bool hasChildCodeEditor = (activeChildCodeEditor() != nullptr);
    actionSave->setEnabled(hasChildCodeEditor);
    actionSave_As->setEnabled(hasChildCodeEditor);
    actionRun->setEnabled(hasChildCodeEditor);
    closeAct->setEnabled(hasChildCodeEditor);
    closeAllAct->setEnabled(hasChildCodeEditor);
    tileAct->setEnabled(hasChildCodeEditor);
    cascadeAct->setEnabled(hasChildCodeEditor);
    nextAct->setEnabled(hasChildCodeEditor);
    previousAct->setEnabled(hasChildCodeEditor);
    windowMenuSeparatorAct->setVisible(hasChildCodeEditor);
    actionComment->setEnabled(hasChildCodeEditor);
    actionUncomment->setEnabled(hasChildCodeEditor);
    actionIndent_More->setEnabled(hasChildCodeEditor);
    actionIndent_Less->setEnabled(hasChildCodeEditor);

#ifndef QT_NO_CLIPBOARD
    action_Paste->setEnabled(hasChildCodeEditor);
    bool hasSelection = (activeChildCodeEditor() && activeChildCodeEditor()->textCursor().hasSelection());
    actionCu_t->setEnabled(hasSelection);
    action_Copy->setEnabled(hasSelection);
#endif
}

void QPythonEditor::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(windowMenuSeparatorAct);

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    windowMenuSeparatorAct->setVisible(!windows.isEmpty());

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
        QAction *action = windowMenu->addAction(text);
        connect(action, &QAction::triggered, mdiSubWindow, [this, mdiSubWindow]() {
            mdiArea->setActiveSubWindow(mdiSubWindow);
        });
        action->setCheckable(true);
        action->setChecked(child == activeChildCodeEditor());
    }
}

CodeEditor *QPythonEditor::createChildCodeEditor()
{
    auto *child = new CodeEditor(this->settings);

#ifndef QT_NO_CLIPBOARD
    connect(child, &QPlainTextEdit::copyAvailable, actionCu_t, &QAction::setEnabled);
    connect(child, &QPlainTextEdit::copyAvailable, action_Copy, &QAction::setEnabled);
#endif

    return child;
}

void QPythonEditor::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), settingsApplicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }
    else
    {
        restoreGeometry(geometry);
    }
}

void QPythonEditor::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), settingsApplicationName());
    settings.setValue("geometry", saveGeometry());
}

CodeEditor *QPythonEditor::activeChildCodeEditor() const
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
    {
        return qobject_cast<CodeEditor *>(activeSubWindow->widget());
    }
    return nullptr;
}

QMdiSubWindow *QPythonEditor::findChildCodeEditor(const QString &fileName) const
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    for (QMdiSubWindow *window : mdiArea->subWindowList())
    {
        auto *mdiChild = qobject_cast<CodeEditor *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
        {
            return window;
        }
    }
    return nullptr;
}

QString QPythonEditor::settingsApplicationName()
{
    return QString(QCoreApplication::applicationName()).append(":PythonPlugin");
}

void QPythonEditor::runExecute()
{
    if (activeChildCodeEditor())
    {
        this->scriptOutputConsole->clear();
        Q_EMIT executionCalled(qPrintable(activeChildCodeEditor()->toPlainText()), this->scriptOutputConsole);
    }
}
