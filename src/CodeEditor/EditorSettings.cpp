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

#include "EditorSettings.h"

#include <QPushButton>
#include <QSettings>

static const auto SHOULD_HIGHLIGHT_SETTINGS_KEY = QStringLiteral("ShouldHighlightCurrentLine");
static const auto COLOR_SCHEME_SETTINGS_KEY = QStringLiteral("ColorSchemeName");

static QString SettingsScopeName()
{
    return QCoreApplication::applicationName().append(":PythonRuntime.Editor");
}

EditorSettings::EditorSettings() : QDialog(), Ui::EditorSettings()
{
    setupUi();
    connectSignals();
    loadFromSettings();
}

void EditorSettings::setupUi()
{
    Ui_EditorSettings::setupUi(this);
    for (const ColorScheme &colorScheme : ColorScheme::AvailableColorSchemes())
    {
        colorSchemeComboBox->addItem(colorScheme.name());
    }
}

int EditorSettings::fontSize() const
{
    return this->fontSizeSpinBox->value();
}

bool EditorSettings::shouldHighlightCurrentLine() const
{
    return m_shouldHighlightCurrentLine;
}

const ColorScheme &EditorSettings::colorScheme() const
{
    return m_colorScheme;
}

void EditorSettings::saveChanges()
{
    const auto *colorScheme = ColorScheme::ColorSchemeByName(colorSchemeComboBox->currentText());
    Q_ASSERT(colorScheme != nullptr);
    m_colorScheme = *colorScheme;
    m_shouldHighlightCurrentLine = highlightCurrentLineCheckBox->isChecked();

    QSettings savedSettings(QCoreApplication::organizationName(), SettingsScopeName());
    savedSettings.setValue(COLOR_SCHEME_SETTINGS_KEY, m_colorScheme.name());
    savedSettings.setValue(SHOULD_HIGHLIGHT_SETTINGS_KEY, m_shouldHighlightCurrentLine);

    Q_EMIT settingsChanged();
}

void EditorSettings::saveChangesAndClose()
{
    saveChanges();
    close();
}

void EditorSettings::cancel()
{
    setFormValuesToCurrentValues();
    close();
}

void EditorSettings::setFormValuesToCurrentValues() const
{
    highlightCurrentLineCheckBox->setChecked(m_shouldHighlightCurrentLine);
    colorSchemeComboBox->setCurrentText(m_colorScheme.name());
}

void EditorSettings::loadFromSettings()
{
    const QSettings savedSettings(QCoreApplication::organizationName(), SettingsScopeName());

    QVariant value = savedSettings.value(COLOR_SCHEME_SETTINGS_KEY);
    const auto *colorScheme = ColorScheme::ColorSchemeByName(value.toString());
    if (colorScheme)
    {
        m_colorScheme = *colorScheme;
    }

    if (savedSettings.contains(SHOULD_HIGHLIGHT_SETTINGS_KEY))
    {
        value = savedSettings.value(SHOULD_HIGHLIGHT_SETTINGS_KEY);
        m_shouldHighlightCurrentLine = value.toBool();
    }
    setFormValuesToCurrentValues();
}

void EditorSettings::connectSignals() const
{
    connect(buttonBox, &QDialogButtonBox::accepted, this, &EditorSettings::saveChangesAndClose);
    connect(buttonBox->button(QDialogButtonBox::Apply),
            &QPushButton::clicked,
            this,
            &EditorSettings::saveChanges);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &EditorSettings::cancel);
}
