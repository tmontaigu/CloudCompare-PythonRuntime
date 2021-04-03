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
