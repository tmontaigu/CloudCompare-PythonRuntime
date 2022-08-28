#ifndef CLOUDCOMPAREPROJECTS_PYPRINTLOGGER_H
#define CLOUDCOMPAREPROJECTS_PYPRINTLOGGER_H
#include <ccLog.h>

#include <pybind11/pybind11.h>

#include <mutex>

namespace py = pybind11;

/// Logger that prints messages to the python stdout
///
/// It uses python's print internally.
///
/// The goal is to still be able to display messages
/// logged via `ccLog::..` in stand alone mode by registering this
/// logger as the global instance.
class PyPrintLogger : public ccLog
{
  public:
    PyPrintLogger() = default;

    ~PyPrintLogger() override = default;

    void logMessage(const QString &message, int level) override;

  private:
    std::mutex m_lock;
};
#endif // CLOUDCOMPAREPROJECTS_PYPRINTLOGGER_H
