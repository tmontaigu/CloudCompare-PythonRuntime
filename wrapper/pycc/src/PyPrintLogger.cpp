#include "PyPrintLogger.h"

void PyPrintLogger::logMessage(const Message &message)
{
    std::lock_guard<std::mutex> guard(m_lock);
    const std::string stdMsg = message.text.toStdString();
    py::print(stdMsg.c_str());
}
