//##########################################################################
//#                                                                        #
//#                CLOUDCOMPARE PLUGIN: PythonPlugin                       #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                   COPYRIGHT: Thomas Montaigu                           #
//#                                                                        #
//##########################################################################

#include "PythonInterpreter.h"
#include "PythonStdErrOutRedirect.h"
#include "Utilities.h"

#include <pybind11/embed.h>

#include <QApplication>
#include <QDir>
#include <QListWidgetItem>
#include <QProcess>
#include <QTextCodec>

#include <ccLog.h>

#ifdef Q_OS_UNIX
#include <cstdio>
#include <dlfcn.h>
#endif

// seems like gcc defines macro with these names
#undef major
#undef minor

namespace py = pybind11;

//================================================================================

struct Version
{
    constexpr Version() = default;

    constexpr Version(uint16_t major_, uint16_t minor_, uint16_t patch_);

    explicit Version(const QStringRef &versionStr);

    bool operator==(const Version &other) const;

    uint16_t major{0};
    uint16_t minor{0};
    uint16_t patch{0};
};

constexpr Version::Version(uint16_t major_, uint16_t minor_, uint16_t patch_)
    : major(major_), minor(minor_), patch(patch_)
{
}

Version::Version(const QStringRef &versionStr) : Version()
{

    QVector<QStringRef> parts = versionStr.split('.');
    if (parts.size() == 3)
    {
        major = parts[0].toUInt();
        minor = parts[1].toUInt();
        patch = parts[2].toUInt();
    }
}

bool Version::operator==(const Version &other) const
{
    return major == other.major && minor == other.minor && patch == other.patch;
}

constexpr Version PythonVersion(PY_MAJOR_VERSION, PY_MINOR_VERSION, PY_MICRO_VERSION);

//================================================================================

struct PyVenvCfg
{
    PyVenvCfg() = default;

    static PyVenvCfg FromFile(const QString &path);

    QString home{};
    bool includeSystemSitesPackages{};
    Version version;
};

PyVenvCfg PyVenvCfg::FromFile(const QString &path)
{
    PyVenvCfg cfg{};

    QFile cfgFile(path);
    if (cfgFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!cfgFile.atEnd())
        {
            QString line = cfgFile.readLine();
            QStringList v = line.split("=");

            if (v.size() == 2)
            {
                QString name = v[0].simplified();
                QString value = v[1].simplified();

                if (name == "home")
                {
                    cfg.home = value;
                }
                else if (name == "include-system-site-packages")
                {
                    cfg.includeSystemSitesPackages = (value == "true");
                }
                else if (name == "version")
                {
                    cfg.version = Version(QStringRef(&value));
                }
            }
        }
    }

    return cfg;
}

Version GetPythonExeVersion(const QString &pythonExePath)
{
    QProcess pythonProcess;
    pythonProcess.start(pythonExePath, {"--version"});
    pythonProcess.waitForFinished();

    const QString versionStr =
        QTextCodec::codecForName("utf-8")->toUnicode(pythonProcess.readAllStandardOutput());

    QVector<QStringRef> splits = versionStr.splitRef(" ");
    if (splits.size() == 2 && splits[0].contains("Python"))
    {
        return Version(splits[1]);
    }
    return Version{};
}

//================================================================================
#ifdef Q_OS_WIN32
PythonConfigPaths PythonConfigPaths::WindowsBundled()
{
    PythonConfigPaths config{};

    const QDir pythonEnvDirPath(QApplication::applicationDirPath() + "/plugins/Python");
    if (pythonEnvDirPath.exists())
    {
        const QString qPythonHome = pythonEnvDirPath.path();
        config.m_pythonHome.reset(QStringToWcharArray(qPythonHome));

        const QString qPythonPath =
            QString("%1/DLLs;%1/lib;%1/Lib/site-packages;").arg(qPythonHome);
        config.m_pythonPath.reset(QStringToWcharArray(qPythonPath));
    }
    else
    {
        throw std::runtime_error("Python environment not found, plugin wasn't correctly installed");
    }
    return config;
}

PythonConfigPaths PythonConfigPaths::WindowsCondaEnv(const char *condaPrefix)
{
    PythonConfigPaths config{};

    QDir pythonEnvDirPath(condaPrefix);
    if (pythonEnvDirPath.exists())
    {
        QString qPythonHome = pythonEnvDirPath.path();
        config.m_pythonHome.reset(QStringToWcharArray(qPythonHome));

        QString qPythonPath = QString("%1/DLLs;%1/lib;%1/Lib/site-packages;").arg(qPythonHome);
#ifndef USE_EMBEDDED_MODULES
        qPythonPath.append(QApplication::applicationDirPath() +
                           "/plugins/Python/Lib/site-packages");
#endif
        config.m_pythonPath.reset(QStringToWcharArray(qPythonPath));
    }
    else
    {
        throw std::runtime_error("Python environment not found, plugin wasn't correctly installed");
    }
    return config;
}

PythonConfigPaths PythonConfigPaths::WindowsVenv(const char *venvPrefix, const PyVenvCfg &cfg)
{
    PythonConfigPaths config{};

    QDir pythonEnvDirPath(venvPrefix);
    if (pythonEnvDirPath.exists())
    {
        QString qPythonHome = pythonEnvDirPath.path();
        config.m_pythonHome.reset(QStringToWcharArray(qPythonHome));

        QString qPythonPath =
            QString("%1/Lib/site-packages;%3/DLLS;%3/lib").arg(qPythonHome).arg(cfg.home);

        if (cfg.includeSystemSitesPackages)
        {
            qPythonPath.append(QString("%1/Lib/site-packages").arg(cfg.home));
        }

#ifndef USE_EMBEDDED_MODULES
        qPythonPath.append(QApplication::applicationDirPath() +
                           "/plugins/Python/Lib/site-packages");
#endif
        config.m_pythonPath.reset(QStringToWcharArray(qPythonPath));
    }
    else
    {
        throw std::runtime_error("Python environment not found, plugin wasn't correctly installed");
    }
    return config;
}
#endif

bool PythonConfigPaths::isSet() const
{
    return m_pythonHome != nullptr && m_pythonPath != nullptr;
}

const wchar_t *PythonConfigPaths::pythonHome() const
{
    return m_pythonHome.get();
}

const wchar_t *PythonConfigPaths::pythonPath() const
{
    return m_pythonPath.get();
}

//================================================================================

PythonInterpreter::PythonInterpreter(QObject *parent) : QObject(parent) {}

bool PythonInterpreter::executeFile(const std::string &filepath)
{
    if (m_isExecuting)
    {
        return false;
    }
    Q_EMIT executionStarted();

    bool success{true};
    try
    {
        py::object pyStdout =
            py::module::import("ccinternals").attr("ccConsoleOutput")("[PythonStdout] ");
        py::object pyStderr =
            py::module::import("ccinternals").attr("ccConsoleOutput")("[PythonStderr] ");
        PyStdErrOutStreamRedirect r{pyStdout, pyStderr};
        py::eval_file(filepath);
    }
    catch (const std::exception &e)
    {
        ccLog::Warning(e.what());
        success = false;
    }

    Q_EMIT executionFinished();
    return success;
}

void PythonInterpreter::executeCodeWithState(const std::string &code,
                                             QListWidget *output,
                                             State &state)
{
    Q_ASSERT(output != nullptr);
    if (m_isExecuting)
    {
        return;
    }

    Q_EMIT executionStarted();
    m_isExecuting = true;
    const QColor orange(255, 100, 0);

    try
    {
        py::object newStdout = py::module::import("ccinternals").attr("ConsoleREPL")(output);
        py::object newStderr =
            py::module::import("ccinternals").attr("ConsoleREPL")(output, orange);
        PyStdErrOutStreamRedirect redirect{newStdout, newStderr};
        py::exec(code);
    }
    catch (const std::exception &e)
    {
        auto message = new QListWidgetItem(e.what());
        message->setForeground(Qt::red);
        output->addItem(message);
    }

    m_isExecuting = false;
    Q_EMIT executionFinished();
}

void PythonInterpreter::executeCode(const std::string &code, QListWidget *output)
{
    State tmpState;
    executeCodeWithState(code, output, tmpState);
}

void PythonInterpreter::initialize()
{
#ifdef Q_OS_WIN32
    configureEnvironment();
#if PY_MINOR_VERSION == 6
    Py_SetPythonHome(const_cast<wchar_t *>(m_config.pythonHome()));
#else
    Py_SetPythonHome(m_config.pythonHome());
#endif
    Py_SetPath(m_config.pythonPath());
#endif
#ifdef Q_OS_UNIX
    // Work-around issue: undefined symbol: PyExc_RecursionError
    // when trying to import numpy in the intepreter
    // e.g: https://github.com/numpy/numpy/issues/14946
    // https://stackoverflow.com/questions/49784583/numpy-import-fails-on-multiarray-extension-library-when-called-from-embedded-pyt
    // This workaround is weak

    const auto displaydlopenError = []() {
        char *error = dlerror();
        if (error)
        {
            ccLog::Warning("[PythonPlugin] dlopen error:", error);
        }
    };

    char soName[25];
    snprintf(soName, 24, "libpython%d.%d.so", PythonVersion.major, PythonVersion.minor);
    m_libPythonHandle = dlopen(soName, RTLD_LAZY | RTLD_GLOBAL);
    if (!m_libPythonHandle)
    {
        displaydlopenError();
        snprintf(soName, 24, "libpython%d.%dm.so", PythonVersion.major, PythonVersion.minor);
        m_libPythonHandle = dlopen(soName, RTLD_LAZY | RTLD_GLOBAL);
        if (!m_libPythonHandle)
        {
            displaydlopenError();
        }
    }
#endif

    py::initialize_interpreter();
}

bool PythonInterpreter::IsInitialized()
{
    return Py_IsInitialized();
}

void PythonInterpreter::finalize()
{
    if (Py_IsInitialized())
    {
        py::finalize_interpreter();
#ifdef Q_OS_UNIX
        if (m_libPythonHandle)
        {
            dlclose(m_libPythonHandle);
            m_libPythonHandle = nullptr;
        }
#else
        Q_UNUSED(this);
#endif
    }
}

#ifdef Q_OS_WIN32
void PythonInterpreter::configureEnvironment()
{
    const char *condaPrefix = std::getenv("CONDA_PREFIX");
    const char *venvPrefix = std::getenv("VIRTUAL_ENV");
    if (condaPrefix)
    {
        ccLog::Print("[PythonPlugin] Conda environment detected (%s)",
                     std::getenv("CONDA_DEFAULT_ENV"));
        QString pythonExePath = QString(condaPrefix) + "/python.exe";
        if (!QFile::exists(pythonExePath))
        {
            pythonExePath = "python";
        }

        Version condaPythonVersion = GetPythonExeVersion(pythonExePath);
        if (condaPythonVersion == PythonVersion)
        {
            m_config = PythonConfigPaths::WindowsCondaEnv(condaPrefix);
        }
        else
        {
            ccLog::Warning("[PythonPlugin] Conda environment's Python version (%u.%u.%u)"
                           " does not match the plugin expected version (%u.%u.%u)",
                           condaPythonVersion.major,
                           condaPythonVersion.minor,
                           condaPythonVersion.patch,
                           PythonVersion.major,
                           PythonVersion.minor,
                           PythonVersion.patch);
        }
    }
    else if (venvPrefix)
    {
        ccLog::Print("[PythonPlugin] Virtual environment detected");
        PyVenvCfg cfg = PyVenvCfg::FromFile(QString("%1/pyvenv.cfg").arg(venvPrefix));
        if (cfg.version == PythonVersion)
        {
            m_config = PythonConfigPaths::WindowsVenv(venvPrefix, cfg);
        }
        else
        {
            ccLog::Warning("[PythonPlugin] venv's Python version (%u.%u.%u)"
                           " does not match the plugin expected version (%u.%u.%u)",
                           cfg.version.major,
                           cfg.version.minor,
                           cfg.version.patch,
                           PythonVersion.major,
                           PythonVersion.minor,
                           PythonVersion.patch);
        }
    }

    if (!m_config.isSet())
    {
        if (condaPrefix || venvPrefix)
        {
            ccLog::Warning("Something went wrong using custom environment configuration"
                           " falling back to bundled Python environment");
        }
        m_config = PythonConfigPaths::WindowsBundled();
    }
}
#endif

bool PythonInterpreter::isExecuting() const
{
    return m_isExecuting;
}
