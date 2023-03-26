// ##########################################################################
// #                                                                        #
// #                CLOUDCOMPARE PLUGIN: ChaiScriptingPlugin                #
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
#include "ProjectViewContextMenu.h"
#include "ProjectView.h"

#include <QFileSystemModel>
#include <QInputDialog>
#include <QMessageBox>

ProjectViewContextMenu::ProjectViewContextMenu(ProjectView *view)
    : QMenu(),
      m_treeView(view),
      m_renameAction("Rename"),
      m_deleteAction("Delete"),
      m_createFileAction("Create File"),
      m_createFolderAction("Create Folder")
{
    addAction(&m_renameAction);
    addAction(&m_deleteAction);
    addAction(&m_createFileAction);
    addAction(&m_createFolderAction);

    connect(&m_renameAction, &QAction::triggered, this, &ProjectViewContextMenu::renameFile);
    connect(&m_deleteAction, &QAction::triggered, this, &ProjectViewContextMenu::deleteElement);
    connect(&m_createFileAction, &QAction::triggered, this, &ProjectViewContextMenu::createFile);
    connect(
        &m_createFolderAction, &QAction::triggered, this, &ProjectViewContextMenu::createFolder);
}

void ProjectViewContextMenu::requested(const QPoint &pos)
{
    m_currentIndex = m_treeView->indexAt(pos);

    if (m_currentIndex.isValid())
    {

        m_renameAction.setVisible(true);
        m_deleteAction.setVisible(true);

        if (QFileInfo(m_treeView->absolutePathAt(m_currentIndex)).isDir())
        {
            m_createFileAction.setVisible(true);
            m_createFolderAction.setVisible(true);
        }
        else
        {
            m_createFileAction.setVisible(false);
            m_createFolderAction.setVisible(false);
        }
    }
    else
    {
        m_renameAction.setVisible(false);
        m_deleteAction.setVisible(false);
        m_createFileAction.setVisible(true);
        m_createFolderAction.setVisible(true);
    }
    exec(QCursor::pos());
}

void ProjectViewContextMenu::renameFile() const
{
    Q_ASSERT(m_currentIndex.isValid());
    bool ok;
    QString newName =
        QInputDialog::getText(m_treeView,
                              tr("Enter a new name"),
                              tr("new name"),
                              QLineEdit::Normal,
                              *static_cast<QString *>(m_currentIndex.internalPointer()),
                              &ok);

    if (ok && !newName.isEmpty())
    {
        const QString currentPath = m_treeView->relativePathAt(m_currentIndex);
        const QString oldFilePath = QFileInfo(currentPath).absoluteFilePath();
        const QString newFilePath = QFileInfo(currentPath).absoluteDir().filePath(newName);

        if (!QFile::rename(oldFilePath, newFilePath))
        {
            QMessageBox::critical(m_treeView, "Error", "Failed to rename the file");
        }
        m_treeView->setEnabled(true);
    }
}

void ProjectViewContextMenu::deleteElement() const
{
    Q_ASSERT(m_currentIndex.isValid());
    auto result = QMessageBox::question(
        m_treeView, "Confirm deletion ?", "Are you sure you want to delete this ?");

    if (result == QMessageBox::StandardButton::Yes)
    {
        if (!m_treeView->m_fileSystemModel->remove(m_treeView->currentIndex()))
        {
            QMessageBox::critical(m_treeView, "Error", "Failed to delete");
        }
    }
}

void ProjectViewContextMenu::createFile() const
{
    bool ok;
    QString fileName = QInputDialog::getText(
        m_treeView, tr("Enter a new name"), tr("new name"), QLineEdit::Normal, "", &ok);

    if (ok && !fileName.isEmpty())
    {
        QDir basePath(m_treeView->absolutePathAt(m_currentIndex));
        QFile f(basePath.filePath(fileName));
        if (!f.open(QFile::OpenModeFlag::NewOnly))
        {
            QMessageBox::critical(m_treeView, "Error", "Failed to create the file");
            return;
        }
        f.close();
    }
}

void ProjectViewContextMenu::createFolder() const
{
    bool ok;
    QString folderName = QInputDialog::getText(m_treeView,
                                               tr("Enter a new name"),
                                               tr("Folder name"),
                                               QLineEdit::Normal,
                                               "New Folder",
                                               &ok);

    if (ok && !folderName.isEmpty())
    {
        if (m_currentIndex.isValid())
        {
            m_treeView->m_fileSystemModel->mkdir(m_currentIndex, folderName);
        }
        else
        {
            m_treeView->m_fileSystemModel->rootDirectory().mkdir(folderName);
        }
    }
}
