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
#include "PythonActionLauncher.h"

#include <QVBoxLayout>

#undef slots
#include "PythonStdErrOutRedirect.h"
#include "Runtime/Runtime.h"

PythonActionLauncher::PythonActionLauncher(QWidget *parent)
    : QWidget(parent), m_actions(new QListWidget(this))
{
    setWindowTitle("ActionLauncher");
    connect(m_actions, &QListWidget::itemDoubleClicked, this, &PythonActionLauncher::launchAction);
    setLayout(new QVBoxLayout);
    layout()->addWidget(m_actions);
}

void PythonActionLauncher::showEvent(QShowEvent *event)
{
    m_actions->clear();
    for (const Runtime::RegisteredFunction &f : Runtime::registeredFunctions())
    {
        m_actions->addItem(f.name);
    }
    QWidget::showEvent(event);
}

void PythonActionLauncher::launchAction(QListWidgetItem *item) const
{
    QString actionName = item->text();
    for (Runtime::RegisteredFunction &f : Runtime::registeredFunctions())
    {
        if (f.name == actionName)
        {
            try
            {
                PyStdErrOutStreamRedirect scoped_redirect;
                f();
            }
            catch (const std::exception &e)
            {
                ccLog::Error("Failed to start Python actions: %s", e.what());
            }
            break;
        }
    }
}