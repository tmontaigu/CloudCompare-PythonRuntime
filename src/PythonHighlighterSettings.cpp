//##########################################################################
//#                                                                        #
//#              CLOUDCOMPARE PLUGIN: PythonPlugin                         #
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
//#          COPYRIGHT: Thomas Montaigu                                    #
//#                                                                        #
//##########################################################################

#include "PythonHighlighterSettings.h"

#include "PythonHighlighter.h"


#include <QColorDialog>
#include <QFormLayout>
#include <QPushButton>
#include <QSignalMapper>

PythonHighlighterSettings::PythonHighlighterSettings(QWidget *parent)
    : QWidget(parent)
    , m_colorDialog(new QColorDialog(this))
    , m_signalMapper(new QSignalMapper(this)) {
    auto formLayout = new QFormLayout;
    setLayout(formLayout);

    const int numCodeElement = static_cast<size_t>(PythonHighlighter::CodeElement::End);
    m_colors.resize(numCodeElement);
    for (int i{0}; i < numCodeElement; ++i)
    {
        const auto e = static_cast<PythonHighlighter::CodeElement>(i);
        auto *btn = new QPushButton;
        const auto qss = QString("background-color: red");
        btn->setStyleSheet(qss);
        formLayout->addRow(PythonHighlighter::CodeElementName(e), btn);
       
        connect(btn, &QPushButton::clicked, [this, btn, i]
        {
            m_colorDialog->exec();
            m_colors[i].second = m_colorDialog->selectedColor();
            btn->setStyleSheet(QString("background-color: %1")
                .arg(m_colors[i].second.name(QColor::HexRgb)));
        });
    }
}

