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

#include "PythonHighlighter.h"
#include "ColorScheme.h"

// Python keywords
static const QStringList s_Keywords = {
    "and",    "assert", "break", "class",   "continue", "def",  "del",    "elif",
    "else",   "except", "exec",  "finally", "for",      "from", "global", "if",
    "import", "in",     "is",    "lambda",  "not",      "or",   "pass",   "print",
    "raise",  "return", "try",   "while",   "yield",    "None", "True",   "False"};

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

QString PythonHighlighter::CodeElementName(PythonHighlighter::CodeElement e)
{
    switch (e)
    {
    case CodeElement::Keyword:
        return {"Keyword"};
    case CodeElement::Operator:
        return {"Operator"};
    case CodeElement::Brace:
        return {"Brace"};
    case CodeElement::Definition:
        return {"Definition"};
    case CodeElement::String:
        return {"String"};
    case CodeElement::DocString:
        return {"Doc String"};
    case CodeElement::Comment:
        return {"Comment"};
    case CodeElement::Self:
        return {"Self"};
    case CodeElement::Numbers:
        return {"Numbers"};
    case CodeElement::End:
        Q_FALLTHROUGH();
    default:
        Q_ASSERT(false);
        return {};
    }
}

void PythonHighlighter::useColorScheme(const ColorScheme &colorScheme)
{
    for (HighlightingRule &rule : m_highlightingRules)
    {
        rule.format = colorScheme[rule.element];
    }
}

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
    m_triSingle = HighlightingRule(CodeElement::DocString, "'''", 1);

    m_triDouble = HighlightingRule(CodeElement::DocString, R"(""")", 2);

    // Keyword, operator, and brace rules
    for (const QString &keyword : s_Keywords)
    {
        QString pattern = QString("\\b%1\\b").arg(keyword);
        m_highlightingRules += HighlightingRule(CodeElement::Keyword, pattern, 0);
    }

    for (const QString &pattern : s_Operators)
        m_highlightingRules += HighlightingRule(CodeElement::Operator, pattern, 0);

    for (const QString &pattern : s_Braces)
        m_highlightingRules += HighlightingRule(CodeElement::Brace, pattern, 0);

    // All other rules

    // 'self'
    m_highlightingRules += HighlightingRule(CodeElement::Self, "\\bself\\b", 0);

    // Double-quoted string, possibly containing escape sequences
    m_highlightingRules += HighlightingRule(CodeElement::String, R"("[^"\\]*(\\.[^"\\]*)*")", 0);
    // Single-quoted string, possibly containing escape sequences
    m_highlightingRules += HighlightingRule(CodeElement::String, R"("[^'\\]*(\\.[^'\\]*)*")", 0);

    // 'def' followed by an identifier
    m_highlightingRules += HighlightingRule(CodeElement::Definition, R"(\bdef\b\s*(\w+))", 1);
    // 'class' followed by an identifier
    m_highlightingRules += HighlightingRule(CodeElement::Definition, R"(\bclass\b\s*(\w+))", 1);

    // From '#' until a newline
    m_highlightingRules += HighlightingRule(CodeElement::Comment, "#[^\\n]*", 0);

    // Numeric literals
    m_highlightingRules += HighlightingRule(CodeElement::Numbers, "\\b[+-]?[0-9]+[lL]?\\b", 0);
    m_highlightingRules +=
        HighlightingRule(CodeElement::Numbers, "\\b[+-]?0[xX][0-9A-Fa-f]+[lL]?\\b", 0);
    m_highlightingRules += HighlightingRule(
        CodeElement::Numbers, R"(\b[+-]?[0-9]+(?:\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\b)", 0);
}

void PythonHighlighter::highlightPythonBlock(const QString &text)
{
    if (text.isEmpty())
        return;

    int index = -1;

    // Do other syntax formatting
    for (const auto &rule : m_highlightingRules)
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
    bool in_multiline = matchMultiLine(text, m_triSingle);
    if (!in_multiline)
        in_multiline = matchMultiLine(text, m_triDouble);
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
