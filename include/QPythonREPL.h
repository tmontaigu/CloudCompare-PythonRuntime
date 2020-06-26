#ifndef CLOUDCOMPAREPROJECTS_QPYTHONREPL_H
#define CLOUDCOMPAREPROJECTS_QPYTHONREPL_H

#include <QtWidgets>

#include <string>

#undef slots

#include <pybind11/embed.h>
#include "PythonStdErrOutRedirect.h"

class Ui_PythonREPLQt;


namespace ui
{
	class QPythonREPL;

	class History
	{
	public:
		explicit History();

		void add(const QString &&cmd);

		size_t size() const;

		bool empty() const;

		const QString &older();

		const QString &newer();

	private:
		QVector<QString> m_commands;
		QVector<QString>::const_reverse_iterator m_current;
	};


	class KeyPressEater : public QObject
	{
	Q_OBJECT
	public:
		explicit KeyPressEater(QPythonREPL *repl, QObject *parent = nullptr);

	protected:
		bool eventFilter(QObject *obj, QEvent *event) override;

	private:
		QPythonREPL *m_repl{nullptr};
	};


	class QPythonREPL : public QWidget
	{
		friend KeyPressEater;
	Q_OBJECT

	public:
		explicit QPythonREPL(QWidget *parent = nullptr);

		~QPythonREPL() override;

	protected:
		QPlainTextEdit *codeEdit();

		QListWidget *outputDisplay();

	private:
		History m_history{};
		std::string m_buf;
		py::object m_output;
		py::dict m_locals;
		Ui_PythonREPLQt *m_ui;
	};


}

#endif //CLOUDCOMPAREPROJECTS_QPYTHONREPL_H
