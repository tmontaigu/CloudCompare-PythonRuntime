#include "QPythonREPL.h"
#include "PythonHighlighter.h"

#include <ui_PythonREPLQt.h>
#include <ccLog.h>

#undef slots

#include <pybind11/embed.h>
#include <iostream>
#include "PythonStdErrOutRedirect.h"
#include <QFontMetricsF>
#include <exposed.h>

namespace py = pybind11;

const static QString replArrows = ">>> ";

bool ui::KeyPressEater::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		auto *keyEvent = static_cast<QKeyEvent *>(event);
		qDebug("Ate key press %d", keyEvent->key());
		if (keyEvent->modifiers() & Qt::ShiftModifier &&
		    (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return))
		{
			m_repl->codeEdit()->appendPlainText("\n... ");
			return true;
		}

		switch (keyEvent->key())
		{
			case Qt::Key_Enter:
				Q_FALLTHROUGH();
			case Qt::Key_Return:
			{
				if (m_repl->codeEdit()->document()->characterCount() <= 5)
				{
					return true;
				}

				const QString text = m_repl->codeEdit()->toPlainText();
				/// Our raw input contains the ">>> " and "... " and is a QString
				/// convert it to std::string while filtering out non needed chars
				m_repl->m_buf.clear();
				QString::const_iterator iter = text.begin() + replArrows.size();
				while (iter != text.end())
				{
					// FIXME this will return 0 on non latin chars
					//  we should do better
					char c = iter->toLatin1();
					if (c == 0)
					{
						// TODO handle better, like print an error or something
						return true;
					}
					m_repl->m_buf += c;
					if (*iter == '\n')
					{
						iter += replArrows.size();
					}
					iter++;
				}
				m_repl->outputDisplay()->addItem(text);
				m_repl->codeEdit()->clear();
				m_repl->codeEdit()->insertPlainText(replArrows);
				try
				{
					// FIXME this should not be done every time
					py::module::import("pycc");
					m_repl->m_locals["cc"] = GetInstance();

					PyStdErrOutStreamRedirect redir{m_repl->m_output, m_repl->m_output};
					py::exec(m_repl->m_buf.c_str(), py::globals(), m_repl->m_locals);
//				 This part is only useful if we use py::eval
//				 however py::eval does not work on assignent expr (eg: my_var = 1)
//				const auto result = py::eval...
//				if (!result.is_none()) {
//					const std::string repr = result.attr("__repr__")().cast<std::string>();
//					m_repl->outputDisplay()->addItem(QString::fromStdString(repr));
//				}
				} catch (const std::exception &e)
				{
					m_repl->outputDisplay()->addItem(e.what());
				}
				m_repl->m_history.add(std::move(text));
				return true;
			}
			case Qt::Key_Backspace:
				Q_FALLTHROUGH();
			case Qt::Key_Delete:
				if (m_repl->codeEdit()->textCursor().position() > 4)
				{
					return QObject::eventFilter(obj, event);
				} else
				{
					return true;
				}
			case Qt::Key_Left:
				if (m_repl->codeEdit()->textCursor().position() > 4)
				{
					return QObject::eventFilter(obj, event);
				}
				return true;
			case Qt::Key_Down:
				if (!m_repl->m_history.empty())
				{
					m_repl->codeEdit()->clear();
					m_repl->codeEdit()->insertPlainText(m_repl->m_history.newer());
				}
				return true;
			case Qt::Key_Up:
				if (!m_repl->m_history.empty())
				{
					m_repl->codeEdit()->clear();
					m_repl->codeEdit()->insertPlainText(m_repl->m_history.older());
				}
				return true;
			case Qt::Key_Home:
				m_repl->codeEdit()->moveCursor(QTextCursor::StartOfLine);
				m_repl->codeEdit()->moveCursor(QTextCursor::NextWord);
				return true;
			default:
				if (m_repl->codeEdit()->textCursor().position() < 5)
				{
					m_repl->codeEdit()->moveCursor(QTextCursor::NextWord);
				}
				return QObject::eventFilter(obj, event);
		}

	} else
	{
		return QObject::eventFilter(obj, event);
	}
}

ui::KeyPressEater::KeyPressEater(QPythonREPL *repl, QObject *parent) : QObject(parent), m_repl(repl)
{

}

ui::QPythonREPL::QPythonREPL(QWidget *parent) :
		QWidget(parent),
		m_ui(new Ui_PythonREPLQt)
{
	m_ui->setupUi(this);
	auto keyPressEater = new KeyPressEater(this);
	codeEdit()->installEventFilter(keyPressEater);
	codeEdit()->insertPlainText(replArrows);

	auto highlighter = new PythonHighlighter(codeEdit()->document());
	codeEdit()->setTabStopWidth(codeEdit()->fontMetrics().width(' ') * 8);


	QFont font("Monospace");
	font.setStyleHint(QFont::TypeWriter);

	codeEdit()->setFont(font);

	m_buf.reserve(255);
	m_output = py::module::import("ccinternals")
			.attr("ConsoleREPL")(outputDisplay());
}

ui::QPythonREPL::~QPythonREPL()
{
	delete m_ui;
}

QPlainTextEdit *ui::QPythonREPL::codeEdit()
{
	return m_ui->codeEdit;
}

QListWidget *ui::QPythonREPL::outputDisplay()
{
	return m_ui->outputDisplay;
}

ui::History::History()
{}

void ui::History::add(const QString &&cmd)
{
	m_commands.push_back(cmd);
	m_current = m_commands.rbegin();
}

const QString &ui::History::older()
{
	if (m_current == m_commands.rend())
	{
		m_current = m_commands.rbegin();
	}
	const QString &current = *m_current;
	m_current++;
	return current;
}

const QString &ui::History::newer()
{
	if (m_current == m_commands.rend())
	{
		return replArrows;
	}

	const QString &current = *m_current;
	if (m_current == m_commands.rbegin())
	{
		m_current = m_commands.rend();
	} else
	{

		m_current--;
	}
	return current;
}

bool ui::History::empty() const
{
	return m_commands.empty();
}

size_t ui::History::size() const
{
	return m_commands.size();
}
