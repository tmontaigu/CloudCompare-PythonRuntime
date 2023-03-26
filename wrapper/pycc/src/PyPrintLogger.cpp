#include "PyPrintLogger.h"

void PyPrintLogger::logMessage(const QString &message, int level)
{
    std::lock_guard<std::mutex> guard(m_lock);
    const std::string stdMsg = message.toStdString();
    py::print(stdMsg.c_str());
}
