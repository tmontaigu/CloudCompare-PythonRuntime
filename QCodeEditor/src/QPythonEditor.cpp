//##########################################################################
//#                                                                        #
//#                CLOUDCOMPARE PLUGIN: ChaiScriptingPlugin                #
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
//#                                                                        #
//##########################################################################

#include "QPythonEditor.h"
#include "codeEditor.h"

//qCC
#include "ccMainAppInterface.h"

//Qt
#include <QMainWindow>
#include <QtWidgets>

//global static pointer (as there should only be one instance of chaiScriptCodeEditorMainWindow!)
static QPythonEditor* s_instance = nullptr;

QPythonEditor::QPythonEditor() : Ui::QPythonEditor()
{
	setupUi(this);
	mdiArea = new QMdiArea(this);
	setCentralWidget(mdiArea);
	mdiArea->showMaximized();
	//PBtreeView->showMaximized();
	connect(mdiArea, &QMdiArea::subWindowActivated,
		this, &QPythonEditor::updateMenus);

	this->projectBrowser->hide();
	createActions();
	createStatusBar();
	updateMenus();

	readSettings();
	QCoreApplication::instance()->installEventFilter(this);
}

void QPythonEditor::closeEvent(QCloseEvent* event)
{
	mdiArea->closeAllSubWindows();
	if (mdiArea->currentSubWindow()) 
	{
		event->ignore();
	}
	else 
	{
		Q_EMIT destroy_chai();
		writeSettings();
		event->accept();
		QCoreApplication::instance()->removeEventFilter(this);
	}
}

void QPythonEditor::newFile()
{
	CodeEditor* child = createChildCodeEditor();
	child->newFile();
	child->show();
}

void QPythonEditor::open()
{
	const QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty())
	{
		openFile(fileName);
	}
}

void QPythonEditor::changeEvent(QEvent* e)
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

QPythonEditor* QPythonEditor::TheInstance()
{
	if (!s_instance)
	{
		s_instance = new QPythonEditor();
	}
	return s_instance;
}

bool QPythonEditor::openFile(const QString& fileName)
{
	if (QMdiSubWindow* existing = findChildCodeEditor(fileName))
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

bool QPythonEditor::loadFile(const QString& fileName)
{
	CodeEditor* child = createChildCodeEditor();
	const bool succeeded = child->loadFile(fileName);
	if (succeeded)
	{
		child->show();
	}
	else
	{
		child->close();
	}
	QPythonEditor::prependToRecentFiles(fileName);
	return succeeded;
}

static inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }
static inline QString fileKey() { return QStringLiteral("file"); }


static QStringList readRecentFiles(QSettings& settings)
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

static void writeRecentFiles(const QStringList& files, QSettings& settings)
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

void QPythonEditor::prependToRecentFiles(const QString& fileName)
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
	if (const QAction* action = qobject_cast<const QAction*>(sender()))
	{
		openFile(action->data().toString());
	}
}

bool QPythonEditor::eventFilter(QObject* obj, QEvent* e)
{
	switch (e->type()) 
	{
		case QEvent::Shortcut: 
		{
			QShortcutEvent* sev = static_cast<QShortcutEvent*>(e);
			if (sev->isAmbiguous()) 
			{
				for (const auto & action : actions())
				{
					if (action->shortcut() == sev->key())
					{
						action->trigger(); // Trigger the action that matches the ambigous shortcut event.
						return true;
					}
				}
			}
		}
		default: break;
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
	CodeEditor* child = activeChildCodeEditor();
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

void QPythonEditor::about()
{

}

void QPythonEditor::createActions()
{
	connect(actionNew, &QAction::triggered, this, &QPythonEditor::newFile);
	connect(actionSave, &QAction::triggered, this, &QPythonEditor::save);
	connect(actionOpen, &QAction::triggered, this, &QPythonEditor::open);
	connect(actionSave_As, &QAction::triggered, this, &QPythonEditor::saveAs);
	connect(actionRun, &QAction::triggered, this, &QPythonEditor::runExecute);
	connect(actionClose, &QAction::triggered, this, [=]() {close(); });
	

	
	connect(actionSave_Chai_state, &QAction::triggered, this, [=]() { Q_EMIT save_Chai_state(); });
	connect(actionReset_Chai_to_initial_state, &QAction::triggered, this, [=]() { Q_EMIT reset_Chai_to_initial_state(); });
	connect(actionReset_chai_to_last_save, &QAction::triggered, this, [=]() { Q_EMIT reset_chai_to_last_save(); });

	menuFile->addSeparator();

	QMenu* recentMenu = menuFile->addMenu(tr("Recent..."));
	connect(recentMenu, &QMenu::aboutToShow, this, &QPythonEditor::updateRecentFileActions);
	recentFileSubMenuAct = recentMenu->menuAction();

	for (int i = 0; i < MaxRecentFiles; ++i)
	{
		recentFileActs[i] = recentMenu->addAction(QString());
		connect(recentFileActs[i], &QAction::triggered, this, &QPythonEditor::openRecentFile);
		recentFileActs[i]->setVisible(false);
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
	connect(closeAct, &QAction::triggered,
		mdiArea, &QMdiArea::closeActiveSubWindow);

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

	QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));

	QAction* aboutAct = helpMenu->addAction(tr("&About"));
	connect(aboutAct, &QAction::triggered, this, &QPythonEditor::about);
	aboutAct->setStatusTip(tr("Show the application's About box"));

	addAction(actionNew); //Actions must be added to be able to find shortcuts in event filter
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

void QPythonEditor::updateMenus()
{
	bool hasChildCodeEditor = (activeChildCodeEditor() != 0);
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
	bool hasSelection = (activeChildCodeEditor() &&
		activeChildCodeEditor()->textCursor().hasSelection());
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

	QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
	windowMenuSeparatorAct->setVisible(!windows.isEmpty());

	for (int i = 0; i < windows.size(); ++i) {
		QMdiSubWindow* mdiSubWindow = windows.at(i);
		CodeEditor* child = qobject_cast<CodeEditor*>(mdiSubWindow->widget());

		QString text;
		if (i < 9) {
			text = tr("&%1 %2").arg(i + 1)
				.arg(child->userFriendlyCurrentFile());
		}
		else {
			text = tr("%1 %2").arg(i + 1)
				.arg(child->userFriendlyCurrentFile());
		}
		QAction* action = windowMenu->addAction(text);
		connect(action, &QAction::triggered,  mdiSubWindow, [this, mdiSubWindow]() {
			mdiArea->setActiveSubWindow(mdiSubWindow);
			});
		action->setCheckable(true);
		action->setChecked(child == activeChildCodeEditor());
	}
}


CodeEditor* QPythonEditor::createChildCodeEditor()
{
	CodeEditor* child = new CodeEditor;
	mdiArea->addSubWindow(child);

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
	QSettings settings(QCoreApplication::organizationName(), settingsApplicationName());
	settings.setValue("geometry", saveGeometry());
}

CodeEditor* QPythonEditor::activeChildCodeEditor() const
{
	if (QMdiSubWindow* activeSubWindow = mdiArea->activeSubWindow())
	{
		return qobject_cast<CodeEditor*>(activeSubWindow->widget());
	}
	return nullptr;
}

QMdiSubWindow* QPythonEditor::findChildCodeEditor(const QString& fileName) const
{
	QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

	for(QMdiSubWindow * window : mdiArea->subWindowList())
	{
		CodeEditor* mdiChild = qobject_cast<CodeEditor*>(window->widget());
		if (mdiChild->currentFile() == canonicalFilePath)
		{
			return window;
		}
	}
	return nullptr;
}

QString QPythonEditor::settingsApplicationName()
{

	return QString(QCoreApplication::applicationName()).append(":ChaiScriptPlugin");
}

void QPythonEditor::runExecute()
{
	if (activeChildCodeEditor())
	{
		this->scriptOutputConsole->clear();
		Q_EMIT executionCalled(
				qPrintable(activeChildCodeEditor()->userFriendlyCurrentFile()),
				qPrintable(activeChildCodeEditor()->toPlainText()),
				this->scriptOutputConsole
		);
	}
}

