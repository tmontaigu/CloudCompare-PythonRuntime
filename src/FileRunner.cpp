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

#include "ui_FileRunner.h"

#include "PythonInterpreter.h"
#include "FileRunner.h"

#include <QFileDialog>
#include <QResizeEvent>
#include <QStyle>
#include <QProgressBar>

static QWidget *createBusyWidget(QWidget *parent)
{
    auto *w = new QWidget(parent);
    w->hide();
    w->setBaseSize(parent->size());
    w->setStyleSheet("background-color: rgba(125, 125, 125, 85%)");

    auto *bar = new QProgressBar(w);
    bar->setStyleSheet("");
    bar->setRange(0, 0);

    auto *layout = new QHBoxLayout;
    layout->addStretch();
    layout->addWidget(bar);
    layout->addStretch();
    w->setLayout(layout);

    return w;
}

FileRunner::FileRunner(PythonInterpreter* interp, QWidget *parent) :
      QDialog(parent),
      interpreter(interp),
      busyWidget(nullptr),
      ui(new Ui::FileRunner)

{
    ui->setupUi(this);
    ui->runFileBtn->setEnabled(false);
    ui->runFileBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowRight));
    setWindowIcon(QIcon(":/CC/plugin/PythonPlugin/images/runner-icon.png"));
    busyWidget = createBusyWidget(this);

    connect(ui->selectFileBtn, &QPushButton::clicked, this, &FileRunner::selectFile);
    connect(ui->runFileBtn, &QPushButton::clicked, this, &FileRunner::runFile);
    connect(interp, &PythonInterpreter::executionStarted, this, &FileRunner::pythonExecutionStarted);
    connect(interp, &PythonInterpreter::executionFinished, this, &FileRunner::pythonExecutionEnded);
}

void FileRunner::selectFile() {
    filePath = QFileDialog::getOpenFileName(this, "Select Python Script", QString(), "Python Script (*.py)");
    ui->filePathLabel->setText(filePath);
    ui->runFileBtn->setEnabled(!filePath.isEmpty());
}

void FileRunner::runFile() const
{
    if (!filePath.isEmpty()) {
        const std::string path = filePath.toStdString();
        interpreter->executeFile(path);
    }
}

void FileRunner::pythonExecutionStarted()
{
    setEnabled(false);
    busyWidget->show();
}

void FileRunner::pythonExecutionEnded()
{
    setEnabled(true);
    busyWidget->hide();
}

FileRunner::~FileRunner()
{
    delete ui;
}

void FileRunner::resizeEvent(QResizeEvent *event)
{
    busyWidget->resize(event->size());
    QDialog::resizeEvent(event);
}

