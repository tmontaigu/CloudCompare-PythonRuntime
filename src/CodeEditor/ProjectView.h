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
#ifndef PYTHON_PLUGIN_PROJECT_TREE_VIEW_H
#define PYTHON_PLUGIN_PROJECT_TREE_VIEW_H

#include <QFileSystemModel>
#include <QTreeView>

#include "ProjectViewContextMenu.h"

class ProjectViewContextMenu;

class ProjectView final : public QTreeView
{
    Q_OBJECT

    friend class ProjectViewContextMenu;

  public:
    explicit ProjectView(QWidget *parent = nullptr) : QTreeView(parent)
    {
        m_fileSystemModel = new QFileSystemModel;
        QTreeView::setModel(m_fileSystemModel);

        m_contextMenu = new ProjectViewContextMenu(this);
        setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

        for (int i{1}; i < size().width(); ++i)
        {
            hideColumn(i);
        }
        connect(this,
                &QTreeView::customContextMenuRequested,
                m_contextMenu,
                &ProjectViewContextMenu::requested);
    }

    void setRootPath(const QString &path)
    {
        m_fileSystemModel->setRootPath(path);
        setRootIndex(m_fileSystemModel->index(path));
    }

    QString relativePathAt(const QModelIndex &index) const
    {
        return m_fileSystemModel->filePath(index);
    }

    QString absolutePathAt(const QModelIndex &index) const
    {
        return m_fileSystemModel->rootDirectory().filePath(relativePathAt(index));
    }

  private:
    QFileSystemModel *m_fileSystemModel;
    ProjectViewContextMenu *m_contextMenu;
};

#endif // PYTHON_PLUGIN_PROJECT_TREE_VIEW_H
