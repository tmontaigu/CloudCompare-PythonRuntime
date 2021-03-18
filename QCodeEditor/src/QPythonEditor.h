#ifndef CHAISCRIPT_CODE_EDITOR_MAIN_WINDOW
#define CHAISCRIPT_CODE_EDITOR_MAIN_WINDOW

//##########################################################################
//#                                                                        #
//#                CLOUDCOMPARE PLUGIN: ChaiScripting                      #
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

#include <ui_QPythonEditor.h>

// Qt
#include "QEditorSettings.h"
#include <QMainWindow>
#include <ccLog.h>

class CodeEditor;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QFileSystemModel;
class QModelIndex;
QT_END_NAMESPACE

class ccMainAppInterface;
class PythonInterpreter;

class QPythonEditor : public QMainWindow, public Ui::QPythonEditor
{
    Q_OBJECT

  public:
    explicit QPythonEditor(PythonInterpreter *interpreter);

    void changeEvent(QEvent *e) override;
    /// Opens a file
    ///
    /// If the fileName points to an already opened file,
    /// then its window takes focus
    ///
    /// \param fileName the file to open
    /// \return true of success
    bool openFile(const QString &fileName);
    static QString settingsApplicationName();

  protected:
    void closeEvent(QCloseEvent *event) override;
  Q_SIGNALS:
    void
    executionCalled(const std::string &evalStatement, QListWidget *output);

  protected Q_SLOTS:
    void newFile();
    void promptForFileToOpen();
    // Display a folder selection dialog & fills the
    // content of the project view with the content of the selected folder
    void promptForFolderToOpen();
    void save();
    void saveAs();
    void updateRecentFileActions();
    void openRecentFile();
    bool eventFilter(QObject *obj, QEvent *e) override;
    // Loads the double clicked item from the project view
    // if its a file
    void projectTreeDoubleClicked(const QModelIndex& index);

#ifndef QT_NO_CLIPBOARD
    void cut();
    void copy();
    void paste();
#endif
    void comment();
    void uncomment();
    void indentMore();
    void indentLess();
    void about();
    void updateWindowMenu();
    void executionStarted();
    void executionFinished();
    CodeEditor *createChildCodeEditor();

  private:
    enum
    {
        MaxRecentFiles = 10
    };

    // Initialization functions
    void createActions();
    void createStatusBar();
    void updateMenus();
    void initProjectView();

    void runExecute();
    void readSettings();
    void writeSettings();
    bool loadFile(const QString &fileName);
    static bool hasRecentFiles();
    void prependToRecentFiles(const QString &fileName);
    void setRecentFilesVisible(bool visible);
    CodeEditor *activeChildCodeEditor() const;
    QMdiSubWindow *findChildCodeEditor(const QString &fileName) const;

    QFileSystemModel* fileSystemModel{nullptr};

    QEditorSettings *settings{nullptr};

    QMdiArea *mdiArea{nullptr};

    QMenu *windowMenu{nullptr};

    QAction *recentFileActs[MaxRecentFiles] = {nullptr};
    QAction *recentFileSeparator{nullptr};
    QAction *recentFileSubMenuAct{nullptr};

    QAction *closeAct{nullptr};
    QAction *closeAllAct{nullptr};
    QAction *tileAct{nullptr};
    QAction *cascadeAct{nullptr};
    QAction *nextAct{nullptr};
    QAction *previousAct{nullptr};
    QAction *windowMenuSeparatorAct{nullptr};
};

#endif // CHAISCRIPT_CODE_EDITOR_MAIN_WINDOW
