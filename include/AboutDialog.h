#ifndef CLOUDCOMPAREPROJECTS_ABOUTDIALOG_H
#define CLOUDCOMPAREPROJECTS_ABOUTDIALOG_H

#include <QDialog>

class Ui_AboutDialog;

class AboutDialog : public QDialog
{
  public:
    explicit AboutDialog(QWidget *parent = nullptr);

  private:
    Ui_AboutDialog *m_dlg;
};

#endif // CLOUDCOMPAREPROJECTS_ABOUTDIALOG_H
