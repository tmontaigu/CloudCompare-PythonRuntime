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

// Original Author: "JeroenDierckx"
// from https://forum.qt.io/topic/96285/c-highlighter-for-python
// "The license is "don't make money off this without giving back to the community" ;-)"

#ifndef PYTHON_HIGHLIGHTER_H
#define PYTHON_HIGHLIGHTER_H

#include <QRegExp>
#include <QSyntaxHighlighter>
#include <utility>

// Started from Qt Syntax Highlighter example and then ported
// https://wiki.python.org/moin/PyQt/Python%20syntax%20highlighting
class PythonHighlighter : public QSyntaxHighlighter
{
  public:
    explicit PythonHighlighter(QTextDocument *parent = nullptr);

  protected:
    void highlightBlock(const QString &text) override;

  private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
        int matchIndex = 0;

        HighlightingRule() = default;

        HighlightingRule(const QRegExp &r, int i, QTextCharFormat f)
            : pattern(r), format(std::move(f)), matchIndex(i)
        {
        }

        HighlightingRule(const QString &p, int i, QTextCharFormat f)
            : pattern(QRegExp(p)), format(std::move(f)), matchIndex(i)
        {
        }
    };

    void initialize();

    void highlightPythonBlock(const QString &text);

    bool matchMultiLine(const QString &text, const HighlightingRule &rule);

    QVector<HighlightingRule> pythonHighlightingRules;
    HighlightingRule triSingle;
    HighlightingRule triDouble;
};

#endif // PYTHON_HIGHLIGHTER_H