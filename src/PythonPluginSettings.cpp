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

#include <memory>
#include <QSettings>
#include <ui_PythonPluginSettings.h>

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
    restoreSettings();
}

void PythonPluginSettings::restoreSettings()
{
    std::unique_ptr<QSettings> settings = LoadOurSettings();
    m_ui->lineEdit->setText(settings->value(QString::fromUtf8("PluginPaths")).value<QString>());
}

void PythonPluginSettings::saveSettings() const
{
    std::unique_ptr<QSettings> settings = LoadOurSettings();
    settings->setValue(QString::fromUtf8("PluginPaths"), pluginsPaths());
}

QString PythonPluginSettings::pluginsPaths() const
{
    return m_ui->lineEdit->text();
}