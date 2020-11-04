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

#include "QEditorSettings.h"
#include <ccLog.h>

QEditorSettings::QEditorSettings() : Ui::QEditorSettings()
{
    this->setupUi(this);
    connectSignals();
}

int QEditorSettings::fontSize() const { return this->fontSizeSpinBox->value(); }

void QEditorSettings::connectSignals()
{
    connect(this->fontSizeSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int _newValue) {
        ccLog::Print("The value changed to %d", _newValue);
        Q_EMIT settingsChanged();
    });
}
