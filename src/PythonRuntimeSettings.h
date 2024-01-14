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
// #                   COPYRIGHT: Thomas Montaigu                           #
// #                                                                        #
// ##########################################################################
#ifndef PYTHON_PLUGIN_SETTINGS_H
#define PYTHON_PLUGIN_SETTINGS_H

#include <PythonConfig.h>

#include <QDialog>

class Ui_PythonRuntimeSettings;

class PythonRuntimeSettings final : public QDialog
{
  public:
    explicit PythonRuntimeSettings(QWidget *parent = nullptr);

    QStringList pluginsPaths() const;
    PythonConfig pythonEnvConfig() const;
    bool isDefaultPythonEnv() const;

  private: // Methods
    QString selectedEnvType() const;
    QString localEnvPath() const;

    void restoreSettings();
    void saveSettings() const;
    void handleEditPluginsPaths();
    void handleEnvComboBoxChange(const QString &envTypeName);
    void handleSelectLocalEnv();

  private:
    Ui_PythonRuntimeSettings *m_ui;
    QStringList m_pluginsPaths;
};

#endif // PYTHON_PLUGIN_SETTINGS_H
