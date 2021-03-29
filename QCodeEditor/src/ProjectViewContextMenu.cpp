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
//#                     COPYRIGHT: Thomas Montaigu                         #
//#                                                                        #
//##########################################################################
#include "ProjectViewContextMenu.h"

#include <QDir>
#include <QFileSystemModel>
#include <QInputDialog>
#include <QMessageBox>
#include <QTreeView>

#include <ccLog.h>

ProjectViewContextMenu::ProjectViewContextMenu(QTreeView *view)
    : QMenu(), treeView(view), model(static_cast<QFileSystemModel *>(treeView->model())),
      renameAction("Rename"), deleteAction("Delete"), createFileAction("Create File"),
      createFolderAction("Create Folder")
{
    addAction(&renameAction);
    addAction(&deleteAction);
    addAction(&createFileAction);
    addAction(&createFolderAction);

    connect(&renameAction, &QAction::triggered, this, &ProjectViewContextMenu::renameFile);
    connect(&deleteAction, &QAction::triggered, this, &ProjectViewContextMenu::deleteElement);
    connect(&createFileAction, &QAction::triggered, this, &ProjectViewContextMenu::createFile);
    connect(&createFolderAction, &QAction::triggered, this, &ProjectViewContextMenu::createFolder);
}

void ProjectViewContextMenu::requested(const QPoint &pos)
{
    currentIndex = treeView->indexAt(pos);

    if (currentIndex.isValid())
    {

        renameAction.setVisible(true);
        deleteAction.setVisible(true);

        ccLog::Print(model->rootDirectory().filePath(model->filePath(currentIndex)));
        if (QFileInfo(model->rootDirectory().filePath(model->filePath(currentIndex))).isDir())
        {
            createFileAction.setVisible(true);
            createFolderAction.setVisible(true);
        }
        else
        {
            createFileAction.setVisible(false);
            createFolderAction.setVisible(false);
        }
    }
    else
    {
        renameAction.setVisible(false);
        deleteAction.setVisible(false);
        createFileAction.setVisible(true);
        createFolderAction.setVisible(true);
    }
    exec(QCursor::pos());
}

void ProjectViewContextMenu::renameFile() const
{
    Q_ASSERT(currentIndex.isValid());
    bool ok;
    QString newName = QInputDialog::getText(treeView,
                                            tr("Enter a new name"),
                                            tr("new name"),
                                            QLineEdit::Normal,
                                            *static_cast<QString *>(currentIndex.internalPointer()),
                                            &ok);

    if (ok && !newName.isEmpty())
    {
        const QString currentPath = model->filePath(currentIndex);
        const QString oldFilePath = QFileInfo(currentPath).absoluteFilePath();
        const QString newFilePath = QFileInfo(currentPath).absoluteDir().filePath(newName);

        if (!QFile::rename(oldFilePath, newFilePath))
        {
            QMessageBox::critical(treeView, "Error", "Failed to rename the file");
        }
        treeView->setEnabled(true);
    }
}

void ProjectViewContextMenu::deleteElement() const
{
    Q_ASSERT(currentIndex.isValid());
    auto result =
        QMessageBox::question(treeView, "Confirm deletion ?", "Are you sure you want to delete this ?");

    if (result == QMessageBox::StandardButton::Yes)
    {
        if (!model->remove(treeView->currentIndex()))
        {
            QMessageBox::critical(treeView, "Error", "Failed to delete");
        }
    }
}

void ProjectViewContextMenu::createFile() const
{
    bool ok;
    QString fileName =
        QInputDialog::getText(treeView, tr("Enter a new name"), tr("new name"), QLineEdit::Normal, "", &ok);

    if (ok && !fileName.isEmpty())
    {
        QDir basePath;
        if (currentIndex.isValid())
        {
            basePath = model->filePath(currentIndex);
        }
        else
        {
            basePath = model->rootDirectory();
        }

        QFile f(basePath.filePath(fileName));
        if (!f.open(QFile::OpenModeFlag::NewOnly))
        {
            QMessageBox::critical(treeView, "Error", "Failed to create the file");
            return;
        }
        f.close();
    }
}

void ProjectViewContextMenu::createFolder() const
{
    bool ok;
    QString folderName = QInputDialog::getText(
        treeView, tr("Enter a new name"), tr("Folder name"), QLineEdit::Normal, "New Folder", &ok);

    if (ok && !folderName.isEmpty())
    {
        if (currentIndex.isValid())
        {
            model->mkdir(currentIndex, folderName);
        }
        else
        {
            model->rootDirectory().mkdir(folderName);
        }
    }
}
