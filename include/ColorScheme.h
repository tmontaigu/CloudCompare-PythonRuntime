#ifndef PYTHON_PLUGIN_COLOR_SCHEME_H
#define PYTHON_PLUGIN_COLOR_SCHEME_H


#include "PythonHighlighter.h"


class ColorScheme final
{
  public:
    using Vector = std::vector<ColorScheme>;

    static const Vector &AvailableColorSchemes();
    static const ColorScheme *ColorSchemeByName(const QString &name);

    static ColorScheme Default();
    static ColorScheme Dracula();

    const QString &name() const;
    QColor backgroundColor() const;
    QColor foregroundColor() const;
    QColor currentLineHighlightColor() const;

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
