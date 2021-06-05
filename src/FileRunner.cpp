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

#include "FileRunner.h"
#include "PythonInterpreter.h"

#include <QFileDialog>
#include <QProgressBar>
#include <QResizeEvent>
#include <QStyle>

/// Simple widget that grays out the view of its parent
/// and shows a waiting bar, to give visual feedback that a script is running
static QWidget *CreateBusyWidget(QWidget *parent)
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

FileRunner::FileRunner(PythonInterpreter *interp, QWidget *parent)
    : QDialog(parent), m_interpreter(interp), m_busyWidget(nullptr), m_ui(new Ui::FileRunner)

{
    m_ui->setupUi(this);
    m_ui->runFileBtn->setEnabled(false);
    m_ui->runFileBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowRight));
    setWindowIcon(QIcon(":/CC/plugin/PythonPlugin/images/runner-icon.png"));
    m_busyWidget = CreateBusyWidget(this);

    connect(m_ui->selectFileBtn, &QPushButton::clicked, this, &FileRunner::selectFile);
    connect(m_ui->runFileBtn, &QPushButton::clicked, this, &FileRunner::runFile);
    connect(
        interp, &PythonInterpreter::executionStarted, this, &FileRunner::pythonExecutionStarted);
    connect(interp, &PythonInterpreter::executionFinished, this, &FileRunner::pythonExecutionEnded);
}

void FileRunner::selectFile()
{
    m_filePath = QFileDialog::getOpenFileName(
        this, "Select Python Script", QString(), "Python Script (*.py)");
    m_ui->filePathLabel->setText(m_filePath);
    m_ui->runFileBtn->setEnabled(!m_filePath.isEmpty());
}

void FileRunner::runFile() const
{
    if (!m_filePath.isEmpty())
    {
        const std::string path = m_filePath.toStdString();
        m_interpreter->executeFile(path);
    }
}

void FileRunner::pythonExecutionStarted()
{
    setEnabled(false);
    m_busyWidget->show();
}

void FileRunner::pythonExecutionEnded()
{
    setEnabled(true);
    m_busyWidget->hide();
}

FileRunner::~FileRunner() noexcept
{
    delete m_ui;
}

void FileRunner::resizeEvent(QResizeEvent *event)
{
    m_busyWidget->resize(event->size());
    QDialog::resizeEvent(event);
}
