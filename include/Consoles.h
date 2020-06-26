//
// Created by t.montaigu on 29/06/2020.
//

#ifndef CLOUDCOMPAREPROJECTS_CONSOLES_H
#define CLOUDCOMPAREPROJECTS_CONSOLES_H

#include <functional>
#include <QString>
#include <QListWidget>
#include <ccLog.h>


/// Class implementing 'write' to be able to act like
/// a Python file object in order to be able to
/// output messages from Python's print to the CloudCompare console
/// instead of stdout & stderr
class ConsoleWrapper
{
public:
	explicit ConsoleWrapper(std::function<void(const QString &)> printFn) : m_printFn(std::move(printFn))
	{}

	void write(const char *messagePart)
	{

		size_t len = strlen(messagePart);
		const char *messageEnd = messagePart + len;
		const char *start = messagePart;

		const char *newLinePos;
		while ((newLinePos = strchr(start, '\n')) != nullptr)
		{
			m_currentMessage += QString::fromUtf8(start, static_cast<int>(newLinePos - start));
			m_printFn(m_currentMessage);
			m_currentMessage.clear();
			start = newLinePos + 1;
		}

		if (start != messageEnd)
		{
			m_currentMessage += QString::fromUtf8(start, static_cast<int>(messageEnd - start));
		}
	}

private:
	QString m_currentMessage{};
	std::function<void(const QString &)> m_printFn;
};


class ccConsoleOutput
{
public:
	ccConsoleOutput() = default;

	void write(const char *messagePart)
	{
		m_output.write(messagePart);
	}

private:
	ConsoleWrapper m_output{[](const QString &message)
	                        { ccLog::Print(message); }};
};

class ConsoleREPL
{
public:
	explicit ConsoleREPL(QListWidget *view) : view(view)
	{}

	void write(const char *messagePart)
	{
		m_output.write(messagePart);
	}

private:
	QListWidget *view;
	ConsoleWrapper m_output{[this](const QString &message)
	                        { view->addItem(message); }};

};
#endif //CLOUDCOMPAREPROJECTS_CONSOLES_H
