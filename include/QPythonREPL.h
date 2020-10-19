#ifndef CLOUDCOMPAREPROJECTS_QPYTHONREPL_H
#define CLOUDCOMPAREPROJECTS_QPYTHONREPL_H

#include <QtWidgets>

#include <string>


#include "PythonStdErrOutRedirect.h"
#include <pybind11/pybind11.h>


namespace py = pybind11;

class Ui_PythonREPLQt;


namespace ui
{
	class QPythonREPL;

	/// Simple history system for the REPL.
	class History
	{
	public:
		explicit History() = default;

		void add(const QString &&cmd);

		size_t size() const;

		bool empty() const;

		const QString &older();

		const QString &newer();

	private:
		QVector<QString> m_commands;
		QVector<QString>::const_reverse_iterator m_current;
	};


	/// Class used by the REPL to handle key presses
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


	/// Homemade REPL (Read Print Eval Loop)
	class QPythonREPL : public QWidget
	{
		friend KeyPressEater;
	Q_OBJECT

	public:
		explicit QPythonREPL(QWidget *parent = nullptr);

		void executeCode(const QString &pythonCode);
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
