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
#ifndef PROJECT_TREE_VIEW_H
#define PROJECT_TREE_VIEW_H

#include <QFileSystemModel>
#include <QTreeView>

#include "ProjectViewContextMenu.h"

class ProjectViewContextMenu;

class ProjectView : public QTreeView
{
    Q_OBJECT

    friend class ProjectViewContextMenu;

  public:
    explicit ProjectView(QWidget *parent = nullptr) : QTreeView(parent)
    {
        fileSystemModel = new QFileSystemModel;
        setModel(fileSystemModel);

        contextMenu = new ProjectViewContextMenu(this);
        setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

        for (int i{1}; i < size().width(); ++i)
        {
            hideColumn(i);
        }
        connect(
            this, &QTreeView::customContextMenuRequested, contextMenu, &ProjectViewContextMenu::requested);
    }


    void setRootPath(const QString& path) {
        fileSystemModel->setRootPath(path);
        setRootIndex(fileSystemModel->index(path));
    }

    inline QString relativePathAt(const QModelIndex& index) const {
        return fileSystemModel->filePath(index);
    }

    inline QString absolutePathAt(const QModelIndex& index) const {
        return fileSystemModel->rootDirectory().filePath(relativePathAt(index));
    }

  private:
    QFileSystemModel *fileSystemModel;
    ProjectViewContextMenu *contextMenu;
};

#endif // PROJECT_TREE_VIEW_H
