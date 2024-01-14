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
// #                     COPYRIGHT: Chris S Brown                           #
// #                                Thomas Montaigu                         #
// ##########################################################################

#include <QtWidgets>

#include "CodeEditor.h"
#include "PythonHighlighter.h"

static const char *const INDENT_STRING = "    ";

static const char *const PYTHON_COMMENT_STR = "# ";

CodeEditor::CodeEditor(EditorSettings *settings, QWidget *parent)
    : QPlainTextEdit(parent), m_settings(settings), m_highlighter(new PythonHighlighter(document()))
{
    connect(settings, &EditorSettings::settingsChanged, this, &CodeEditor::updateUsingSettings);

    m_lineNumberArea = new LineNumberArea(this);
    connect(this, &QPlainTextEdit::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &QPlainTextEdit::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeEditor::startAllHighlighting);

    updateLineNumberAreaWidth(0);
    setAttribute(Qt::WA_DeleteOnClose);
    updateUsingSettings();
    installEventFilter(this);
}

bool CodeEditor::eventFilter(QObject *target, QEvent *event)
{
    if (target == this && event->type() == QEvent::Wheel)
    {
        auto *wheel = static_cast<QWheelEvent *>(event);
        if (wheel->modifiers() == Qt::ControlModifier)
        {
            if (wheel->angleDelta().y() > 0)
            {
                zoomIn(1);
            }
            else
            {
                zoomOut(1);
            }
            return true;
        }
    }
    return QPlainTextEdit::eventFilter(target, event);
}

void CodeEditor::updateUsingSettings()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(m_settings->fontSize());

    setFont(font);
    QPalette p = palette();
    p.setColor(QPalette::Base, m_settings->colorScheme().backgroundColor());
    p.setColor(QPalette::Text, m_settings->colorScheme().foregroundColor());
    setPalette(p);

    setLineWrapMode(QPlainTextEdit::NoWrap);

    m_highlighter->useColorScheme(m_settings->colorScheme());

    m_highlighter->rehighlight();
    highlightCurrentLine();
    this->repaint();
}

void CodeEditor::startAllHighlighting()
{
    const QList<QTextEdit::ExtraSelection> selections;
    setExtraSelections(selections);
    highlightCurrentLine();
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().maxWidth() * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        m_lineNumberArea->scroll(0, dy);
    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    if (!m_settings->shouldHighlightCurrentLine())
    {
        return;
    }

    QList<QTextEdit::ExtraSelection> selections = extraSelections();

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        selection.format.setBackground(m_settings->colorScheme().currentLineHighlightColor());
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        selections.append(selection);
    }

    setExtraSelections(selections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(
                0, top, m_lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::newFile()
{
    static int sequenceNumber = 1;

    m_isUntitled = true;
    m_curFile = tr("script%1.py").arg(sequenceNumber++);
    setWindowTitle(m_curFile + "[*]");

    connect(document(), &QTextDocument::contentsChanged, this, &CodeEditor::documentWasModified);
}

bool CodeEditor::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(
            this, tr("MDI"), tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);

    connect(document(), &QTextDocument::contentsChanged, this, &CodeEditor::documentWasModified);

    return true;
}

bool CodeEditor::save()
{
    if (m_isUntitled)
    {
        return saveAs();
    }
    else
    {
        return saveFile(m_curFile);
    }
}

bool CodeEditor::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), m_curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool CodeEditor::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,
                             tr("MDI"),
                             tr("Cannot write file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    return true;
}

QString CodeEditor::userFriendlyCurrentFile()
{
    return strippedName(m_curFile);
}

void CodeEditor::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void CodeEditor::documentWasModified()
{
    setWindowModified(document()->isModified());
}

bool CodeEditor::maybeSave()
{
    if (!document()->isModified())
        return true;
    const QMessageBox::StandardButton ret =
        QMessageBox::warning(this,
                             tr("MDI"),
                             tr("'%1' has been modified.\n"
                                "Do you want to save your changes?")
                                 .arg(userFriendlyCurrentFile()),
                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret)
    {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void CodeEditor::setCurrentFile(const QString &fileName)
{
    m_curFile = QFileInfo(fileName).canonicalFilePath();
    m_isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString CodeEditor::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void CodeEditor::createPairedCharsSelection(int pos)
{
    QList<QTextEdit::ExtraSelection> selections = extraSelections();

    QTextEdit::ExtraSelection selection;
    QTextCharFormat format = selection.format;
    format.setBackground(Qt::green);
    selection.format = format;

    QTextCursor cursor = textCursor();
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    selection.cursor = cursor;

    selections.append(selection);

    setExtraSelections(selections);
}

void CodeEditor::comment()
{
    int lineCount = getSelectedLineCount();
    QTextCursor cursor = textCursor();
    cursor.setPosition(cursor.selectionEnd());

    for (int i = 0; i < lineCount; i++)
    {
        cursor.movePosition(QTextCursor::MoveOperation::StartOfLine);
        cursor.insertText(PYTHON_COMMENT_STR);
        cursor.movePosition(QTextCursor::MoveOperation::Up);
    }
}

void CodeEditor::uncomment()
{
    int lineCount = getSelectedLineCount();
    QTextCursor cursor = textCursor();
    cursor.setPosition(cursor.selectionEnd());

    for (int i = 0; i < lineCount; i++)
    {
        cursor.movePosition(QTextCursor::MoveOperation::StartOfLine);
        QString line = cursor.block().text();
        if (line.startsWith(PYTHON_COMMENT_STR))
        {
            cursor.deleteChar();
            cursor.deleteChar();
        }
        cursor.movePosition(QTextCursor::MoveOperation::Up);
    }
}

void CodeEditor::indentMore()
{
    int lineCount = getSelectedLineCount();
    QTextCursor cursor = textCursor();
    cursor.setPosition(cursor.selectionEnd());

    for (int i = 0; i < lineCount; i++)
    {
        cursor.movePosition(QTextCursor::MoveOperation::StartOfLine);
        cursor.insertText(INDENT_STRING);
        cursor.movePosition(QTextCursor::MoveOperation::Up);
    }
}

void CodeEditor::indentLess()
{
    int lineCount = getSelectedLineCount();
    QTextCursor cursor = textCursor();
    cursor.setPosition(cursor.selectionEnd());

    for (int i = 0; i < lineCount; i++)
    {
        cursor.movePosition(QTextCursor::MoveOperation::StartOfLine);
        QString line = cursor.block().text();
        if (line.startsWith("\t"))
        {
            cursor.deleteChar();
        }
        else if (line.startsWith(INDENT_STRING))
        {
            for (int i = 0; i < 4; i++)
            {
                cursor.deleteChar();
            }
        }
        // line = cursor.block().text();

        cursor.movePosition(QTextCursor::MoveOperation::Up);
    }
}

int CodeEditor::getSelectedLineCount()
{
    QTextCursor cursor = textCursor();
    if (cursor.hasSelection())
    {
        cursor.setPosition(cursor.selectionStart());
        int temp = cursor.blockNumber();
        cursor = textCursor();
        cursor.setPosition(cursor.selectionEnd());
        int diff = cursor.blockNumber() - temp;
        return diff + 1;
    }
    else
    {
        return 1;
    }
}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_Tab:
        indentMore();
        break;
    case Qt::Key_Backtab:
        indentLess();
        break;
    default:
        QPlainTextEdit::keyPressEvent(e);
        break;
    }
}
