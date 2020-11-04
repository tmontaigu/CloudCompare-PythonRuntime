#include "AboutDialog.h"

#include <ui_AboutDialog.h>

#include <pybind11/pybind11.h>


AboutDialog::AboutDialog(QWidget *parent): QDialog(parent), m_dlg(new Ui_AboutDialog) {
    m_dlg->setupUi(this);

    QObject::connect(m_dlg->okBtn, &QPushButton::clicked, this, &QDialog::close);

    const char* versionStr = Py_GetVersion();
    m_dlg->pythonVersionLabel->setText(QString("<p align=\"center\"> %1 </p>").arg(versionStr));
}

