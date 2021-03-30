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

#include "PythonHighlighter.h"

static QTextCharFormat FormatHelper(const QColor &color,
                                    QFont::Style style = QFont::Style::StyleNormal,
                                    QFont::Weight weight = QFont::Weight::Normal)
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

// For lack of a better name
enum class CodeElement
{
    Keyword = 0,
    Operator,
    Brace,
    Definition,
    String,
    DocString,
    Comment,
    Self,
    Numbers
};

static const QMap<CodeElement, QTextCharFormat> s_Styles = {
    {CodeElement::Keyword, FormatHelper(Qt::blue)},
    {CodeElement::Operator, FormatHelper(Qt::red)},
    {CodeElement::Brace, FormatHelper(Qt::darkGray)},
    {CodeElement::Definition, FormatHelper(Qt::black, QFont::Bold)},
    {CodeElement::String, FormatHelper(Qt::magenta)},
    {CodeElement::DocString, FormatHelper(Qt::darkMagenta, QFont::StyleItalic)},
    {CodeElement::Comment, FormatHelper(Qt::darkGreen, QFont::StyleItalic)},
    {CodeElement::Self, FormatHelper(Qt::black, QFont::StyleItalic)},
    {CodeElement::Numbers, FormatHelper("brown")}};

// Python keywords
static const QStringList s_Keywords = {
    "and",     "assert", "break", "class",  "continue", "def",    "del",   "elif", "else",   "except", "exec",
    "finally", "for",    "from",  "global", "if",       "import", "in",    "is",   "lambda", "not",    "or",
    "pass",    "print",  "raise", "return", "try",      "while",  "yield", "None", "True",   "False"};

// Python operators
static const QStringList s_Operators = {"=",
                                        // Comparison
                                        "==",
                                        "!=",
                                        "<",
                                        "<=",
                                        ">",
                                        ">=",
                                        // Arithmetic
                                        "\\+",
                                        "-",
                                        "\\*",
                                        "/",
                                        "//",
                                        "\\%",
                                        "\\*\\*",
                                        // In-place
                                        "\\+=",
                                        "-=",
                                        "\\*=",
                                        "/=",
                                        "\\%=",
                                        // Bitwise
                                        "\\^",
                                        "\\|",
                                        "\\&",
                                        "\\~",
                                        ">>",
                                        "<<"};

// Python braces
static const QStringList s_Braces = {"\\{", "\\}", "\\(", "\\)", R"([)", R"(])"};

PythonHighlighter::PythonHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    initialize();
}

void PythonHighlighter::highlightBlock(const QString &text)
{
    highlightPythonBlock(text);
}

void PythonHighlighter::initialize()
{
    // Multi-line strings (expression, flag, style)
    // FIXME: The triple-quotes in these two lines will mess up the
    // syntax highlighting from this point onward
    triSingle = HighlightingRule("'''", 1, s_Styles[CodeElement::DocString]);
    triDouble = HighlightingRule(R"(""")", 2, s_Styles[CodeElement::DocString]);

    // Keyword, operator, and brace rules
    for (const QString &keyword : s_Keywords)
    {
        QString pattern = QString("\\b%1\\b").arg(keyword);
        pythonHighlightingRules += HighlightingRule(pattern, 0, s_Styles[CodeElement::Keyword]);
    }

    for (const QString &pattern : s_Operators)
        pythonHighlightingRules += HighlightingRule(pattern, 0, s_Styles[CodeElement::Operator]);

    for (const QString &pattern : s_Braces)
        pythonHighlightingRules += HighlightingRule(pattern, 0, s_Styles[CodeElement::Brace]);

    // All other rules

    // 'self'
    pythonHighlightingRules += HighlightingRule("\\bself\\b", 0, s_Styles[CodeElement::Self]);

    // Double-quoted string, possibly containing escape sequences
    pythonHighlightingRules +=
        HighlightingRule(R"("[^"\\]*(\\.[^"\\]*)*")", 0, s_Styles[CodeElement::String]);
    // Single-quoted string, possibly containing escape sequences
    pythonHighlightingRules +=
        HighlightingRule(R"("[^'\\]*(\\.[^'\\]*)*")", 0, s_Styles[CodeElement::String]);

    // 'def' followed by an identifier
    pythonHighlightingRules += HighlightingRule(R"(\bdef\b\s*(\w+))", 1, s_Styles[CodeElement::Definition]);
    // 'class' followed by an identifier
    pythonHighlightingRules +=
        HighlightingRule(R"(\bclass\b\s*(\w+))", 1, s_Styles[CodeElement::Definition]);

    // From '#' until a newline
    pythonHighlightingRules += HighlightingRule("#[^\\n]*", 0, s_Styles[CodeElement::Comment]);

    // Numeric literals
    pythonHighlightingRules += HighlightingRule("\\b[+-]?[0-9]+[lL]?\\b", 0, s_Styles[CodeElement::Numbers]);
    pythonHighlightingRules +=
        HighlightingRule("\\b[+-]?0[xX][0-9A-Fa-f]+[lL]?\\b", 0, s_Styles[CodeElement::Numbers]);
    pythonHighlightingRules += HighlightingRule(
        R"(\b[+-]?[0-9]+(?:\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\b)", 0, s_Styles[CodeElement::Numbers]);
}

void PythonHighlighter::highlightPythonBlock(const QString &text)
{
    if (text.isEmpty())
        return;

    int index = -1;

    // Do other syntax formatting
    for (const auto &rule : pythonHighlightingRules)
    {
        index = rule.pattern.indexIn(text, 0);

        // We actually want the index of the nth match
        while (index >= 0)
        {
            index = rule.pattern.pos(rule.matchIndex);
            int length = rule.pattern.cap(rule.matchIndex).length();
            if (length > 0)
            {
                setFormat(index, length, rule.format);
                index = rule.pattern.indexIn(text, index + length);
            }
        }
    }

    setCurrentBlockState(0);

    // Do multi-line strings
    bool in_multiline = matchMultiLine(text, triSingle);
    if (!in_multiline)
        in_multiline = matchMultiLine(text, triDouble);
}

/*Do highlighting of multi-line strings. ``delimiter`` should be a
``QRegExp`` for triple-single-quotes or triple-double-quotes, and
``in_state`` should be a unique integer to represent the corresponding
state changes when inside those strings. Returns True if we're still
inside a multi-line string when this function is finished.
*/
bool PythonHighlighter::matchMultiLine(const QString &text, const HighlightingRule &rule)
{
    int start, add, end, length;

    // If inside triple-single quotes, start at 0
    if (previousBlockState() == rule.matchIndex)
    {
        start = 0;
        add = 0;
    }
    // Otherwise, look for the delimiter on this line
    else
    {
        start = rule.pattern.indexIn(text);
        // Move past this match
        add = rule.pattern.matchedLength();
    }

    // As long as there's a delimiter match on this line...
    while (start >= 0)
    {
        // Look for the ending delimiter
        end = rule.pattern.indexIn(text, start + add);
        // Ending delimiter on this line?
        if (end >= add)
        {
            length = end - start + add + rule.pattern.matchedLength();
            setCurrentBlockState(0);
        }
        // No; multi-line string
        else
        {
            setCurrentBlockState(rule.matchIndex);
            length = text.length() - start + add;
        }

        // Apply formatting
        setFormat(start, length, rule.format);

        // Look for the next match
        start = rule.pattern.indexIn(text, start + length);
    }

    // Return True if still inside a multi-line string, False otherwise
    return currentBlockState() == rule.matchIndex;
}