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

#ifndef PYTHON_PLUGIN_CONSOLES_H
#define PYTHON_PLUGIN_CONSOLES_H

#include <QListWidget>
#include <QString>

#include <ccLog.h>

#include <functional>
#include <utility>

/// Class implementing 'write' and `flush` to be able to act like
/// a Python file object in order to be able to
/// output messages from Python's print to the CloudCompare console
/// instead of CloudCompare's stdout & stderr
class ConsoleWrapper
{
  public:
    /// printFn: callback function that will be called by this wrapper
    ///			 this callback function shall display the received QString somewhere
    explicit ConsoleWrapper(std::function<void(const QString &)> printFn)
        : m_printFn(std::move(printFn))
    {
    }

    /// Method called by the Python interpreter when something is written.
    /// This function will in turn call the printFn callback given to the constructor
    ///
    /// This function splits the messagePart given by the Python interpreter
    /// using the newline ('\n') character and call the printFn on each of these
    /// splits.
    ///
    /// The goal of this is to ensure that each '\n'-separated lines
    /// appears one one line on CloudCompare's console output
    ///
    /// \param messagePart the message that the Python interpreter sends us
    void write(const char *messagePart)
    {

        const size_t len = strlen(messagePart);
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

    void flush() const
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
    explicit ccConsoleOutput(const char *prefix) : m_prefix(prefix) {}

    void write(const char *messagePart)
    {
        m_output.write(messagePart);
    }

    bool isatty() const
    {
        return false;
    }

    void flush() const
    {
        m_output.flush();
    }

  private:
    const QString m_prefix;
    ConsoleWrapper m_output{[this](const QString &message)
                            {
                                if (m_prefix.isEmpty())
                                {
                                    ccLog::Print(message);
                                }
                                else
                                {
                                    ccLog::Print(m_prefix + message);
                                }
                            }};
};

/// Writes messages to the QListWidget given
class ListWidgetConsole
{
  public:
    ListWidgetConsole(QListWidget *view, const Qt::GlobalColor color) : m_view(view), m_brush(color)
    {
    }
    ListWidgetConsole(QListWidget *view, const QColor &color) : m_view(view), m_brush() {}
    explicit ListWidgetConsole(QListWidget *view) : m_view(view), m_brush() {}

    void write(const char *messagePart)
    {
        m_output.write(messagePart);
    }
    bool isatty() const
    {
        return false;
    }

    void flush() const
    {
        m_output.flush();
    }

  private:
    QListWidget *m_view;
    QBrush m_brush;
    QString m_prefix;
    ConsoleWrapper m_output{[this](const QString &message)
                            {
                                auto *messageItem = new QListWidgetItem(message);
                                messageItem->setForeground(m_brush);
                                m_view->addItem(messageItem);
                            }};
};
#endif // PYTHON_PLUGIN_CONSOLES_H
