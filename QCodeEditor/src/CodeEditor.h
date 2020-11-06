

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

static const char *const indentString = "    ";

static const char *const PYTHON_COMMENT_STR = "# ";

#include "QEditorSettings.h"
#include <QObject>
#include <QPlainTextEdit>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class PythonHighlighter;

class LineNumberArea;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

  public:
    explicit CodeEditor(QEditorSettings *settings, QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    ~CodeEditor() override = default;
    bool eventFilter(QObject *target, QEvent *event) override;
    int lineNumberAreaWidth();
    void newFile();
    bool loadFile(const QString &fileName);
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }
    void comment();
    void uncomment();
    void indentMore();
    void indentLess();

  protected:
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

  private Q_SLOTS:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void documentWasModified();
    void startAllHighlighting();

  private:
    void updateUsingSettings();

  protected:
    void keyPressEvent(QKeyEvent *e) override;

  private:
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void createPairedCharsSelection(int pos);
    int getSelectedLineCount();

    QString curFile;
    bool isUntitled{true};
    QWidget *lineNumberArea{nullptr};
    QEditorSettings *settings{nullptr};
};

class LineNumberArea : public QWidget
{
  public:
    explicit LineNumberArea(CodeEditor *editor) : QWidget(editor) { codeEditor = editor; }

    QSize sizeHint() const override { return QSize(codeEditor->lineNumberAreaWidth(), 0); }

  protected:
    void paintEvent(QPaintEvent *event) override { codeEditor->lineNumberAreaPaintEvent(event); }

  private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H