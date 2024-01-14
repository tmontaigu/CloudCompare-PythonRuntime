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
#include "ColorScheme.h"

#include <utility>

static QTextCharFormat FormatHelper(const QColor &color,
                                    const QFont::Style style = QFont::Style::StyleNormal,
                                    const QFont::Weight weight = QFont::Weight::Normal)
{
    QTextCharFormat charFormat;

    charFormat.setForeground(color);
    charFormat.setFontWeight(weight);
    if (style == QFont::Style::StyleItalic)
    {
        charFormat.setFontItalic(true);
    }

    return charFormat;
}

static QTextCharFormat FormatHelper(const QColor &color, QFont::Weight weight)
{
    return FormatHelper(color, QFont::Style::StyleNormal, weight);
}

ColorScheme::ColorScheme(QString name,
                         QVector<QTextCharFormat> &&formats,
                         const QTextFormat &defaultFormat,
                         QColor backgroundColor,
                         QColor currentLineHighlightColor)
    : m_name(std::move(name)),
      m_formats(formats),
      m_defaultFormat(defaultFormat),
      m_backgroundColor(std::move(backgroundColor)),
      m_currentLineHighlightColor(std::move(currentLineHighlightColor))
{
    Q_ASSERT(formats.size() == static_cast<int>(PythonHighlighter::CodeElement::End));
}

QColor ColorScheme::foregroundColor() const
{
    return m_defaultFormat.foreground().color();
}

QColor ColorScheme::backgroundColor() const
{
    return m_backgroundColor;
}

QColor ColorScheme::currentLineHighlightColor() const
{
    return m_currentLineHighlightColor;
}

const QTextCharFormat &ColorScheme::operator[](PythonHighlighter::CodeElement e) const
{
    const auto index = static_cast<int>(e);
    Q_ASSERT(index < m_formats.size());
    return m_formats[index];
}

ColorScheme ColorScheme::Default()
{
    QVector<QTextCharFormat> formats{FormatHelper(Qt::blue),
                                     FormatHelper(Qt::red),
                                     FormatHelper(Qt::darkGray),
                                     FormatHelper(Qt::black, QFont::Bold),
                                     FormatHelper(Qt::magenta),
                                     FormatHelper(Qt::darkMagenta, QFont::StyleItalic),
                                     FormatHelper(Qt::darkGreen, QFont::StyleItalic),
                                     FormatHelper(Qt::black, QFont::StyleItalic),
                                     FormatHelper("brown")};

    return {"Default",
            std::move(formats),
            FormatHelper(Qt::black),
            Qt::white,
            QColor(Qt::yellow).lighter(160)};
};

ColorScheme ColorScheme::Dracula()
{
    QVector<QTextCharFormat> formats{FormatHelper(QColor(255, 121, 198)),
                                     FormatHelper(Qt::red),
                                     FormatHelper(Qt::darkGray),
                                     FormatHelper(QColor(80, 250, 123), QFont::Bold),
                                     FormatHelper(QColor(241, 250, 140)),
                                     FormatHelper(Qt::darkMagenta, QFont::StyleItalic),
                                     FormatHelper(QColor(98, 114, 164), QFont::StyleItalic),
                                     FormatHelper(QColor(255, 184, 108), QFont::StyleItalic),
                                     FormatHelper("#bd93f9")};

    return {"Dracula",
            std::move(formats),
            FormatHelper(QColor(248, 248, 242)),
            QColor(40, 42, 54),
            QColor(68, 71, 90)};
}

const QString &ColorScheme::name() const
{
    return m_name;
};
static ColorScheme::Vector s_AvailableColorSchemes;

const ColorScheme::Vector &ColorScheme::AvailableColorSchemes()
{
    if (s_AvailableColorSchemes.empty())
    {
        s_AvailableColorSchemes.push_back(ColorScheme::Default());
        s_AvailableColorSchemes.push_back(ColorScheme::Dracula());
    }
    return s_AvailableColorSchemes;
}

const ColorScheme *ColorScheme::ColorSchemeByName(const QString &name)
{
    const auto it =
        std::find_if(s_AvailableColorSchemes.begin(),
                     s_AvailableColorSchemes.end(),
                     [name](const ColorScheme &colorScheme) { return colorScheme.name() == name; });

    if (it != s_AvailableColorSchemes.end())
    {
        return &*it;
    }
    return nullptr;
}
