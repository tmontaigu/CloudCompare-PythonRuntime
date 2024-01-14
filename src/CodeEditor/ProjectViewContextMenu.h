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
// #                     COPYRIGHT: Thomas Montaigu                         #
// #                                                                        #
// ##########################################################################

#ifndef PYTHON_PLUGIN_PROJECT_VIEW_CONTEXT_MENU_H
#define PYTHON_PLUGIN_PROJECT_VIEW_CONTEXT_MENU_H

#include <QMenu>
#include <QModelIndex>

class QTreeView;
class QFileSystemModel;

class ProjectView;

class ProjectViewContextMenu final : public QMenu
{
    Q_OBJECT

  public:
    explicit ProjectViewContextMenu(ProjectView *view);

  public Q_SLOTS:
    void requested(const QPoint &pos);

  private Q_SLOTS:
    void renameFile() const;
    void deleteElement() const;
    void createFile() const;
    void createFolder() const;

  private:
    ProjectView *m_treeView;
    QModelIndex m_currentIndex;

    QAction m_renameAction;
    QAction m_deleteAction;
    QAction m_createFileAction;
    QAction m_createFolderAction;
};

#endif // PYTHON_PLUGIN_PROJECT_VIEW_CONTEXT_MENU_H
