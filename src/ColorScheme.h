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
#ifndef PYTHON_PLUGIN_COLOR_SCHEME_H
#define PYTHON_PLUGIN_COLOR_SCHEME_H

#include "PythonHighlighter.h"

/// ColorScheme to be used by the PythonHighlighter & Editor
class ColorScheme final
{
  public:
    using Vector = std::vector<ColorScheme>;

    /// Returns the vector of available color schemes
    static const Vector &AvailableColorSchemes();
    /// Returns a ColorScheme that matches the given `name`.
    /// returns nullptr if the color scheme does not exists;
    static const ColorScheme *ColorSchemeByName(const QString &name);

    /// Default color scheme
    static ColorScheme Default();
    /// Dracula color scheme
    /// https://draculatheme.com/
    static ColorScheme Dracula();

    /// The name of the color scheme
    const QString &name() const;
    /// Color to be used as the background for the displayed text
    QColor backgroundColor() const;
    /// Color to be used for regular text
    QColor foregroundColor() const;
    /// Color to be used if a highlight of the current line is shown
    QColor currentLineHighlightColor() const;

    /// Returns the format to be used for the given code element
    const QTextCharFormat &operator[](PythonHighlighter::CodeElement e) const;

  private:
    ColorScheme(QString name,
                QVector<QTextCharFormat> &&formats,
                const QTextFormat &defaultFormat,
                QColor backgroundColor,
                QColor currentLineHighlightColor);

  private:
    QString m_name;
    QVector<QTextCharFormat> m_formats;
    QTextFormat m_defaultFormat;
    QColor m_backgroundColor;
    QColor m_currentLineHighlightColor;
};

#endif
