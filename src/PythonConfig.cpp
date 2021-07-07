#include "PythonConfig.h"
#include "Utilities.h"

#include <QApplication>
#include <QDir>
#include <QProcess>
#include <QTextCodec>
#include <QVector>
#include <QtGlobal>

#include <stdexcept>

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

bool Version::operator==(const Version &other) const
{
    return major == other.major && minor == other.minor && patch == other.patch;
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
PythonConfig::PythonConfig() {
#ifdef Q_OS_WIN32
    const char *condaPrefix = std::getenv("CONDA_PREFIX");
    const char *venvPrefix = std::getenv("VIRTUAL_ENV");
    if (condaPrefix) {
        initCondaEnv(condaPrefix);
    } else if (venvPrefix) {
        initVenv(venvPrefix);
    } else {
        initBundled();
        return;
    }
#ifndef USE_EMBEDDED_MODULES
    m_pythonPath.append(QApplication::applicationDirPath() +
                        "/plugins/Python/Lib/site-packages");
#endif
#else
    // On Non windows platform
    // We do nothing, and rely on system's python installation
    m_type = Type::System;
#endif
}

#ifdef Q_OS_WIN32
void PythonConfig::initCondaEnv(const QString &condaPrefix)
{
    m_type = Type::Conda;
    m_pythonHome = condaPrefix;
    m_pythonPath = QString("%1/DLLs;%1/lib;%1/Lib/site-packages;").arg(condaPrefix);
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
}

void PythonConfig::initBundled()
{
    const QDir pythonEnvDirPath(QApplication::applicationDirPath() + "/plugins/Python");
    if (pythonEnvDirPath.exists())
    {
        if (pythonEnvDirPath.exists("pyenv.cfg")) {
            initVenv(pythonEnvDirPath.path());
        } else if (pythonEnvDirPath.exists("conda-meta")) {
            initCondaEnv(pythonEnvDirPath.path());
        } else {
            m_pythonHome = pythonEnvDirPath.path();
            m_pythonPath = QString("%1/DLLs;%1/lib;%1/Lib/site-packages;").arg(m_pythonHome);
            m_type = Type::Unknown;
        }
    }
}
#endif

void PythonConfig::preparePythonProcess(QProcess &pythonProcess) const
{
    if (m_type == Type::System) {
        pythonProcess.setProgram(QStringLiteral("python"));
    } else {
#ifdef Q_OS_WIN
        pythonProcess.setProgram(QString("%1/python.exe").arg(m_pythonHome));
#else
        pythonProcess.setProgram(QString("%1/python").arg(m_pythonHome));
#endif
    }

    // Conda env have SSL related libraries stored in a part that is not
    // in the path of the python exe, we have to add it ourselves.
    if (m_type == Type::Conda)
    {
        const QString additionalPath = QString("%1/Library/bin").arg(m_pythonHome);
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        QString path = env.value("PATH").append(';').append(additionalPath);
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
