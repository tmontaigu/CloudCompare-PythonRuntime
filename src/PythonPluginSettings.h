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
#ifndef PYTHON_PLUGIN_SETTINGS_H
#define PYTHON_PLUGIN_SETTINGS_H

#include <QDialog>

class Ui_PythonPluginSettings;

class PythonPluginSettings final : public QDialog
{
  public:
    explicit PythonPluginSettings(QWidget *parent = nullptr);

    QStringList pluginsPaths() const;

  private: // Methods
    void restoreSettings();
    void saveSettings() const;
    void handleEditPluginsPaths();

  private:
    Ui_PythonPluginSettings *m_ui;
    QStringList m_pluginsPaths;
};

#endif // PYTHON_PLUGIN_SETTINGS_H
