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

#ifndef PYTHON_HIGHLIGHTER_SETTINGS_H
#define PYTHON_HIGHLIGHTER_SETTINGS_H

#include <QDialog>
#include <QVector>

#include "PythonHighlighter.h"

class QColorDialog;
class QSignalMapper;

class PythonHighlighterSettings final : public QWidget
{
    Q_OBJECT
  public:
    explicit PythonHighlighterSettings(QWidget *parent = nullptr);

  private:
    QColorDialog *colorDialog;
    QSignalMapper *signalMapper;
    QVector<QPair<PythonHighlighter::CodeElement, QColor>> colors;
};

#endif // PYTHON_HIGHLIGHTER_SETTINGS_H
