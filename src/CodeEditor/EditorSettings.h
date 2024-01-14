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

#ifndef PYTHON_PLUGIN_EDITOR_SETTINGS_H
#define PYTHON_PLUGIN_EDITOR_SETTINGS_H

#include <ui_EditorSettings.h>

#include "ColorScheme.h"

class EditorSettings final : public QDialog, public Ui::EditorSettings
{
    Q_OBJECT
  public:
    EditorSettings();

    int fontSize() const;
    bool shouldHighlightCurrentLine() const;
    const ColorScheme &colorScheme() const;

  public:
  Q_SIGNALS:
    void settingsChanged();

  protected:
    void connectSignals() const;
    void saveChanges();
    void saveChangesAndClose();
    void cancel();
    void setFormValuesToCurrentValues() const;
    void loadFromSettings();
    void setupUi();

  private:
    ColorScheme m_colorScheme = ColorScheme::Default();
    bool m_shouldHighlightCurrentLine = true;
};

#endif // PYTHON_PLUGIN_EDITOR_SETTINGS_H
