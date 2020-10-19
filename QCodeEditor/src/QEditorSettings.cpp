//
// Created by t.montaigu on 08/07/2020.
//

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
