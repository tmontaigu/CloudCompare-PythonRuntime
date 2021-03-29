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
//#                     COPYRIGHT: Thomas Montaigu                         #
//#                                                                        #
//##########################################################################

#ifndef PROJECT_VIEW_CONTEXT_MENU_H
#define PROJECT_VIEW_CONTEXT_MENU_H

#include <QMenu>
#include <QModelIndex>

class QTreeView;
class QFileSystemModel;

class ProjectViewContextMenu : public QMenu
{
    Q_OBJECT

  public:
    explicit ProjectViewContextMenu(QTreeView *view);

  public Q_SLOTS:
    void contextMenuRequested(const QPoint &pos);
    void renameFile() const;
    void deleteElement() const;
    void createFile() const;
    void createFolder() const;

  private:
    QTreeView *treeView;
    QFileSystemModel *model;

    QModelIndex currentIndex;

    QAction renameAction;
    QAction deleteAction;
    QAction createFileAction;
    QAction createFolderAction;
};

#endif // PROJECT_VIEW_CONTEXT_MENU_H
