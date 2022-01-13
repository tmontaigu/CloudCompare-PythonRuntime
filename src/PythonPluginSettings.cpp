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
#include "Resources.h"
#include <ui_PathVariableEditor.h>
#include <ui_PythonPluginSettings.h>

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QIcon>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QStringListModel>

#include <memory>

/// Simple Dialog that displays a Line Edit with a button next to it
/// to let the user select a path to a directory
class PathVariableInputDialog final : public QDialog
{
  public:
    explicit PathVariableInputDialog(QWidget *parent = nullptr)
        : QDialog(parent),
          m_pathEdit(new QLineEdit),
          m_selectPathBtn(new QPushButton),
          m_dialogBtnBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel))
    {
        m_selectPathBtn->setText(QStringLiteral("..."));
        connect(m_selectPathBtn,
                &QPushButton::clicked,
                this,
                &PathVariableInputDialog::handleSelectPath);

        connect(m_dialogBtnBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(m_dialogBtnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

        auto *layout = new QHBoxLayout;
        layout->addWidget(m_pathEdit);
        layout->addWidget(m_selectPathBtn);

        auto layout1 = new QVBoxLayout;
        layout1->addLayout(layout);
        layout1->addWidget(m_dialogBtnBox);

        setLayout(layout1);
    }

    QString selectedPath() const
    {
        return m_pathEdit->text();
    }

  private:
    void handleSelectPath()
    {
        QString selectedDir = QFileDialog::getExistingDirectory(
            this,
            tr("Select a path to add"),
            QString(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        m_pathEdit->setText(selectedDir);
    }

  private:
    QLineEdit *m_pathEdit;
    QPushButton *m_selectPathBtn;
    QDialogButtonBox *m_dialogBtnBox;
};

/// Dialog that shows the list of Path in which we are looking for plugins.
/// It lets the user edits the paths, add new ones or delete existing ones.
class PathVariableEditor final : public QDialog
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

        PathVariableInputDialog dlg(this);
        dlg.exec();
        QString path = dlg.selectedPath();

        if (!path.isEmpty())
        {
            int rowCount = m_model->rowCount();
            m_model->insertRows(rowCount, 1);
            m_model->setData(m_model->index(rowCount), path);
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
    setWindowIcon(QIcon(SETTINGS_ICON_PATH));

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
