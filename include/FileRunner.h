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

#ifndef FILERUNNER_H
#define FILERUNNER_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class FileRunner;
}

class PythonInterpreter;

class FileRunner : public QDialog
{
    Q_OBJECT

  public:
    explicit FileRunner(PythonInterpreter* interp, QWidget *parent = nullptr);
    ~FileRunner() override;


  private Q_SLOTS:
    void selectFile();
    void runFile();

  private:
    PythonInterpreter *interpreter;
    QString filePath;
    Ui::FileRunner *ui;
};

#endif // FILERUNNER_H
