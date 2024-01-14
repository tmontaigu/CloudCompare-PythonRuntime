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

#include "AboutDialog.h"
#include "Resources.h"

#include <ui_AboutDialog.h>

#undef slots
#include <pybind11/pybind11.h>

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), m_dlg(new Ui_AboutDialog)
{
    m_dlg->setupUi(this);
    setWindowIcon(QIcon(ABOUT_ICON_PATH));

    connect(m_dlg->okBtn, &QPushButton::clicked, this, &QDialog::close);

    const char *versionStr = Py_GetVersion();
    m_dlg->pythonVersionLabel->setText(QString(versionStr));
}

AboutDialog::~AboutDialog() noexcept
{
    delete m_dlg;
}
