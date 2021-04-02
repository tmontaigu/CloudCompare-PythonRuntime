#include <QFileDialog>

#include "PythonInterpreter.h"
#include "FileRunner.h"
#include "ui_FileRunner.h"

#include <QStyle>

FileRunner::FileRunner(PythonInterpreter* interpreter, QWidget *parent) :
      interpreter(interpreter),
      QDialog(parent),
      ui(new Ui::FileRunner)
{
    ui->setupUi(this);
    ui->runFileBtn->setEnabled(false);
    ui->runFileBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowRight));
    setWindowIcon(QIcon(":/CC/plugin/PythonPlugin/images/runner-icon.png"));

    connect(ui->selectFileBtn, &QPushButton::clicked, this, &FileRunner::selectFile);
    connect(ui->runFileBtn, &QPushButton::clicked, this, &FileRunner::runFile);
}

void FileRunner::selectFile() {
    filePath = QFileDialog::getOpenFileName(this, "Select Python Script", QString(), "Python Script (*.py)");
    ui->filePathLabel->setText(filePath);
    ui->runFileBtn->setEnabled(!filePath.isEmpty());
}

void FileRunner::runFile()
{
    if (!filePath.isEmpty()) {
        const std::string path = filePath.toStdString();
        interpreter->executeFile(path);
    }
}

FileRunner::~FileRunner()
{
    delete ui;
}
