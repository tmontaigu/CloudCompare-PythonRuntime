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

#ifndef CLOUDCOMPAREPROJECTS_QEDITORSETTINGS_H
#define CLOUDCOMPAREPROJECTS_QEDITORSETTINGS_H

#include <ui_QEditorSettings.h>

class QEditorSettings : public QWidget, public Ui::QEditorSettings
{
    Q_OBJECT
  public:
    QEditorSettings();

    int fontSize() const;

  public:
  Q_SIGNALS:
    void settingsChanged();

  protected:
    void connectSignals();
};

#endif // CLOUDCOMPAREPROJECTS_QEDITORSETTINGS_H
