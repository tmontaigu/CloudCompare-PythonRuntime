//
// Created by t.montaigu on 29/06/2020.
//

#ifndef CLOUDCOMPAREPROJECTS_CONSOLES_H
#define CLOUDCOMPAREPROJECTS_CONSOLES_H

#include <QListWidget>
#include <QString>
#include <ccLog.h>
#include <functional>

/// Class implementing 'write' to be able to act like
/// a Python file object in order to be able to
/// output messages from Python's print to the CloudCompare console
/// instead of CloudCompare's stdout & stderr
class ConsoleWrapper
{
  public:
    /// printFn: callback function that will be called by this wrapper
    ///			 this callback function shall display the received QString somewhere
    explicit ConsoleWrapper(std::function<void(const QString &)> printFn) : m_printFn(std::move(printFn)) {}

    /// Method called by the Python interpreter when something is written.
    /// This function will in turn call the printFn callback given to the constructor
    ///
    /// This function splits the messagePart given by the Python interpreter
    /// using the newline ('\n') character and call the printFn on for each of these
    /// splits.
    ///
    /// The goal if this is to ensure that each '\n'-separated lines
    /// appears one one line on CloudCompare's console output
    ///
    /// \param messagePart the message that the Python interpreter sends us
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

    void flush()
    {
        m_printFn(m_currentMessage);
    }

  private:
    QString m_currentMessage{};
    std::function<void(const QString &)> m_printFn;
};

/// Redirects messages to CloudCompare's console output
class ccConsoleOutput
{
  public:
    ccConsoleOutput() = default;

    void write(const char *messagePart) { m_output.write(messagePart); }
    void flush() { m_output.flush(); }

  private:
    ConsoleWrapper m_output{[](const QString &message) { ccLog::Print(message); }};
};

/// Redirects messages to the console output of the Python plugin's REPL
class ConsoleREPL
{
  public:
    explicit ConsoleREPL(QListWidget *view) : view(view) {}

    void write(const char *messagePart) { m_output.write(messagePart); }
    void flush() { m_output.flush(); }

  private:
    QListWidget *view;
    ConsoleWrapper m_output{[this](const QString &message) { view->addItem(message); }};
};
#endif // CLOUDCOMPAREPROJECTS_CONSOLES_H
