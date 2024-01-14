#ifndef PYTHON_PLUGIN_CODE_EDITOR_MAIN_WINDOW
#define PYTHON_PLUGIN_CODE_EDITOR_MAIN_WINDOW

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
// #                     COPYRIGHT: Chris S Brown                           #
// #                                Thomas Montaigu                         #
// ##########################################################################

#include <ui_PythonEditor.h>

// Qt
#include "EditorSettings.h"
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
class ProjectViewContextMenu;

#define MAX_RECENT_FILES 10

class PythonEditor final : public QMainWindow, public Ui::PythonEditor
{
    Q_OBJECT

  public:
    static QString SettingsApplicationName();

    explicit PythonEditor(PythonInterpreter *interpreter);

    void changeEvent(QEvent *e) override;
    /// Opens a file
    ///
    /// If the fileName points to an already opened file,
    /// then its window takes focus
    ///
    /// \param fileName the file to open
    /// \return true of success
    bool openFile(const QString &fileName);

  protected:
    void closeEvent(QCloseEvent *event) override;
  Q_SIGNALS:
    void executionCalled(const std::string &evalStatement, QListWidget *output);

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
    void projectTreeDoubleClicked(const QModelIndex &index);

#ifndef QT_NO_CLIPBOARD
    void cut();
    void copy();
    void paste();
#endif
    void comment();
    void uncomment();
    void indentMore();
    void indentLess();
    void updateWindowMenu();
    void executionStarted();
    void executionFinished();
    CodeEditor *createChildCodeEditor();

  private:
    static bool HasRecentFiles();

    // Initialization functions
    void setupUi();
    void createActions();
    void updateMenus();

    void runExecute();
    void readSettings();
    void writeSettings();
    bool loadFile(const QString &fileName);
    void prependToRecentFiles(const QString &fileName);
    void setRecentFilesVisible(bool visible);
    CodeEditor *activeChildCodeEditor() const;
    QMdiSubWindow *findChildCodeEditor(const QString &fileName) const;

  private: // Members
    EditorSettings *m_settings{nullptr};

    QMdiArea *m_mdiArea{nullptr};

    QMenu *m_windowMenu{nullptr};

    QAction *m_recentFileActs[MAX_RECENT_FILES] = {nullptr};
    QAction *m_recentFileSeparator{nullptr};
    QAction *m_recentFileSubMenuAct{nullptr};
    QAction *m_closeAct{nullptr};
    QAction *m_closeAllAct{nullptr};
    QAction *m_tileAct{nullptr};
    QAction *m_cascadeAct{nullptr};
    QAction *m_nextAct{nullptr};
    QAction *m_previousAct{nullptr};
    QAction *m_windowMenuSeparatorAct{nullptr};
};

#endif // PYTHON_PLUGIN_CODE_EDITOR_MAIN_WINDOW
