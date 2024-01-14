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
#include "PythonConfig.h"
#include "Utilities.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QTextCodec>
#include <QVector>
#include <QtGlobal>

#if !defined(USE_EMBEDDED_MODULES) && defined(Q_OS_WINDOWS)
static QString WindowsBundledSitePackagesPath()
{
    return QDir::listSeparator() + QApplication::applicationDirPath() +
           "/plugins/Python/Lib/site-packages";
}
#endif

//================================================================================

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

bool Version::isCompatibleWithCompiledVersion() const
{
    return major == PythonVersion.major && minor == PythonVersion.minor;
}

bool Version::operator==(const Version &other) const
{
    return major == other.major && minor == other.minor && patch == other.patch;
}

static Version GetPythonExeVersion(QProcess &pythonProcess)
{
    pythonProcess.setArguments({"--version"});
    pythonProcess.start(QIODevice::ReadOnly);
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

//================================================================================

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

static QString PathToPythonExecutableInEnv(PythonConfig::Type envType, const QString &envRoot)
{
#if defined(Q_OS_WINDOWS)
    switch (envType)
    {
    case PythonConfig::Type::Conda:
        return envRoot + "/python.exe";
    case PythonConfig::Type::Venv:
        return envRoot + "/Scripts/python.exe";
    case PythonConfig::Type::Unknown:
        return envRoot + "/python.exe";
    case PythonConfig::Type::System:
        return "python.exe";
    }
#else
    switch (envType)
    {
    case PythonConfig::Type::Conda:
    case PythonConfig::Type::Venv:
    case PythonConfig::Type::Unknown:
        return envRoot + "/bin/python";
    case PythonConfig::Type::System:
        return "python";
    }
#endif
    return {};
}

void PythonConfig::initDefault()
{
#ifdef Q_OS_WIN32
    initBundled();
#else
    // On Non windows platform
    // We do nothing, and rely on system's python installation
    m_type = Type::System;
#endif
}

#ifdef Q_OS_WIN32
void PythonConfig::initBundled()
{
    const QString pythonEnvDirPath(QApplication::applicationDirPath() + "/plugins/Python");
    initFromLocation(pythonEnvDirPath);
}
#endif

void PythonConfig::initFromLocation(const QString &prefix)
{
    QDir envRoot(prefix);

    if (!envRoot.exists())
    {
        m_pythonHome = QString();
        m_pythonPath = QString();
        m_type = Type::Unknown;
        return;
    }

    if (envRoot.exists("pyvenv.cfg"))
    {
        QString pythonExePath = PathToPythonExecutableInEnv(Type::Venv, prefix);
        initFromPythonExecutable(pythonExePath);
        if (m_pythonHome.isEmpty() && m_pythonPath.isEmpty())
        {
            qDebug() << "Failed to get paths info from python executable at (venv)"
                     << pythonExePath;
            initVenv(envRoot.path());
        }
        else
        {
            m_type = Type::Venv;
        }
    }
    else if (envRoot.exists("conda-meta"))
    {
        QString pythonExePath = PathToPythonExecutableInEnv(Type::Conda, prefix);
        initFromPythonExecutable(pythonExePath);
        if (m_pythonHome.isEmpty() && m_pythonPath.isEmpty())
        {
            qDebug() << "Failed to get paths info from python executable at (conda)"
                     << pythonExePath;
            initCondaEnv(envRoot.path());
        }
        else
        {
            m_type = Type::Conda;
        }
    }
    else
    {
        m_pythonHome = envRoot.path();
        m_pythonPath = QString("%1/DLLs;%1/lib;%1/Lib/site-packages;").arg(m_pythonHome);
        m_type = Type::Unknown;

#if !defined(USE_EMBEDDED_MODULES) && defined(Q_OS_WINDOWS)
        m_pythonPath.append(WindowsBundledSitePackagesPath());
#endif
    }
}

void PythonConfig::initCondaEnv(const QString &condaPrefix)
{
    m_type = Type::Conda;
    m_pythonHome = condaPrefix;
    m_pythonPath = QString("%1/DLLs;%1/lib;%1/Lib/site-packages;").arg(condaPrefix);

#if !defined(USE_EMBEDDED_MODULES) && defined(Q_OS_WINDOWS)
    m_pythonPath.append(WindowsBundledSitePackagesPath());
#endif
}

void PythonConfig::initVenv(const QString &venvPrefix)
{
    PyVenvCfg cfg = PyVenvCfg::FromFile(QString("%1/pyvenv.cfg").arg(venvPrefix));

    m_type = Type::Venv;
    m_pythonHome = venvPrefix;
    m_pythonPath = QString("%1/Lib/site-packages;%3/DLLS;%3/lib").arg(venvPrefix, cfg.home);
    if (cfg.includeSystemSitesPackages)
    {
        m_pythonPath.append(QString("%1/Lib/site-packages").arg(cfg.home));
    }

#if !defined(USE_EMBEDDED_MODULES) && defined(Q_OS_WINDOWS)
    m_pythonPath.append(WindowsBundledSitePackagesPath());
#endif
}

void PythonConfig::preparePythonProcess(QProcess &pythonProcess) const
{
    const QString pythonExePath = PathToPythonExecutableInEnv(type(), m_pythonHome);
    pythonProcess.setProgram(pythonExePath);

    // Conda env have SSL related libraries stored in a part that is not
    // in the path of the python exe, we have to add it ourselves.
    if (m_type == Type::Conda)
    {
        const QString additionalPath = QString("%1/Library/bin").arg(m_pythonHome);
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        QString path = env.value("PATH").append(QDir::listSeparator()).append(additionalPath);
        env.insert("PATH", path);
        pythonProcess.setProcessEnvironment(env);
    }
}

PythonConfigPaths PythonConfig::pythonCompatiblePaths() const
{
    PythonConfigPaths paths;
    paths.m_pythonHome.reset(QStringToWcharArray(m_pythonHome));
    paths.m_pythonPath.reset(QStringToWcharArray(m_pythonPath));
    return paths;
}

Version PythonConfig::getVersion() const
{
    QProcess pythonProcess;
    preparePythonProcess(pythonProcess);
    return GetPythonExeVersion(pythonProcess);
}

bool PythonConfig::validateAndDisplayErrors(QWidget *parent) const
{
    Version envVersion = getVersion();
    if (envVersion.isNull())
    {
        // This hints that the selected directory is likely not valid.
        QMessageBox::critical(
            parent,
            "Invalid Python Environment",
            "The selected directory does not seems to be a valid python environment");
        return false;
    }

    if (!envVersion.isCompatibleWithCompiledVersion())
    {
        QMessageBox::critical(
            parent,
            "Incompatible Python Environment",
            QString("The selected directory does not contain a Python Environment that is "
                    "compatible. Expected a python version like %1.%2.x, selected environment "
                    "has version %3.%4.%5")
                .arg(QString::number(PythonVersion.major),
                     QString::number(PythonVersion.minor),
                     QString::number(envVersion.major),
                     QString::number(envVersion.minor),
                     QString::number(envVersion.patch)));
        return false;
    }

    return true;
}

bool PythonConfig::IsInsideEnvironment()
{
    return qEnvironmentVariableIsSet("CONDA_PREFIX") || qEnvironmentVariableIsSet("VIRTUAL_ENV");
}

PythonConfig PythonConfig::fromContainingEnvironment()
{
    PythonConfig config;

    QString root = qEnvironmentVariable("CONDA_PREFIX");
    if (!root.isEmpty())
    {
        const QString pythonExePath = PathToPythonExecutableInEnv(Type::Conda, root);
        config.initFromPythonExecutable(pythonExePath);
        config.m_type = Type::Conda;
        return config;
    }

    root = qEnvironmentVariable("VIRTUAL_ENV");
    if (!root.isEmpty())
    {
        const QString pythonExePath = PathToPythonExecutableInEnv(Type::Venv, root);
        config.initFromPythonExecutable(pythonExePath);
        config.m_type = Type::Venv;
        return config;
    }

    return config;
}

void PythonConfig::initFromPythonExecutable(const QString &pythonExecutable)
{
    m_type = Type::Unknown;

    const QString pythonPathScript = QStringLiteral(
        "import os;import sys;print(os.pathsep.join(sys.path[1:]));print(sys.prefix, end='')");

    QProcess pythonProcess;
    pythonProcess.setProgram(pythonExecutable);
    pythonProcess.setArguments({"-c", pythonPathScript});
    pythonProcess.start(QIODevice::ReadOnly);
    pythonProcess.waitForFinished();

    const QString result =
        QTextCodec::codecForName("utf-8")->toUnicode(pythonProcess.readAllStandardOutput());

    QStringList pathsAndHome = result.split('\n');

    if (pathsAndHome.size() != 2)
    {
        plgWarning() << "'" << result << "' could not be parsed as a list if paths and a home path."
                     << "Expected 2 strings found " << pathsAndHome.size();
        return;
    }

    m_pythonPath = pathsAndHome.takeFirst();
    m_pythonHome = pathsAndHome.takeFirst();

#if !defined(USE_EMBEDDED_MODULES) && defined(Q_OS_WINDOWS)
    m_pythonPath.append(WindowsBundledSitePackagesPath());
#endif
}
