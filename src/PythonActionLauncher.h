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
#ifndef PYTHON_PLUGIN_PYTHON_ACTION_LAUNCHER_H
#define PYTHON_PLUGIN_PYTHON_ACTION_LAUNCHER_H

#include <QListWidget>
#include <QWidget>

class Ui_ActionLauncher;
class PythonPluginManager;

class PythonActionLauncher : public QWidget
{
    Q_OBJECT
  public:
    explicit PythonActionLauncher(const PythonPluginManager *pluginManager,
                                  QWidget *parent = nullptr);

  protected:
    void showEvent(QShowEvent *event) override;
    void populateToolBox();
    void clearToolBox();

  private: // Members
    Ui_ActionLauncher *m_ui;
    const PythonPluginManager *m_pluginManager;
};

#endif // PYTHON_PLUGIN_PYTHON_ACTION_LAUNCHER_H
