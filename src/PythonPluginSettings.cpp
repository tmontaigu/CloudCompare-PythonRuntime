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
//#                   COPYRIGHT: Thomas Montaigu                           #
//#                                                                        #
//##########################################################################
#include "PythonPluginSettings.h"
#include <ui_PathVariableEditor.h>
#include <ui_PythonPluginSettings.h>

#include <QInputDialog>
#include <QSettings>
#include <QStringListModel>

#include <memory>

class PathVariableEditor : public QDialog
{
  public:
    explicit PathVariableEditor(const QStringList &values, QWidget *parent = nullptr)
        : QDialog(parent), m_model(new QStringListModel), m_ui(new Ui_PathVariableEditor)
    {
        m_ui->setupUi(this);
        m_model->setStringList(values);
        m_ui->valuesView->setModel(m_model);
        connect(m_ui->addButton, &QPushButton::clicked, this, &PathVariableEditor::handleAdd);
        connect(m_ui->deleteButton, &QPushButton::clicked, this, &PathVariableEditor::handleDelete);
    }

    QStringList stringList() const
    {
        return m_model->stringList();
    }

  private:
    void handleAdd()
    {
        bool ok;
        QString text = QInputDialog::getText(
            this, tr("Add Value"), tr("Value: "), QLineEdit::Normal, QString(), &ok);

        if (ok && !text.isEmpty())
        {
            int rowCount = m_model->rowCount();
            m_model->insertRows(rowCount, 1);
            m_model->setData(m_model->index(rowCount), text);
        }
    }

    void handleDelete()
    {
        const QModelIndex index = m_ui->valuesView->currentIndex();
        m_model->removeRow(index.row());
    }

  private:
    QStringListModel *m_model;
    Ui_PathVariableEditor *m_ui;
};

static std::unique_ptr<QSettings> LoadOurSettings()
{
    return std::make_unique<QSettings>(
        QCoreApplication::organizationName(),
        QCoreApplication::applicationName().append(":PythonPlugin.Settings"));
}

PythonPluginSettings::PythonPluginSettings(QWidget *parent)
    : QDialog(parent), m_ui(new Ui_PythonPluginSettings)
{
    m_ui->setupUi(this);
    connect(this, &QDialog::accepted, this, &PythonPluginSettings::saveSettings);
    connect(m_ui->editButton,
            &QPushButton::clicked,
            this,
            &PythonPluginSettings::handleEditPluginsPaths);
    restoreSettings();
}

void PythonPluginSettings::restoreSettings()
{
    std::unique_ptr<QSettings> settings = LoadOurSettings();
    m_pluginsPaths = settings->value(QString::fromUtf8("PluginPaths")).value<QStringList>();
}

void PythonPluginSettings::saveSettings() const
{
    std::unique_ptr<QSettings> settings = LoadOurSettings();
    settings->setValue(QString::fromUtf8("PluginPaths"), pluginsPaths());
}

void PythonPluginSettings::handleEditPluginsPaths()
{
    PathVariableEditor editor(pluginsPaths(), this);
    editor.exec();
    m_pluginsPaths = editor.stringList();
}

QStringList PythonPluginSettings::pluginsPaths() const
{
    return m_pluginsPaths;
}