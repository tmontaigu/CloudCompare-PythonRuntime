#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QString>
#include <QtGlobal>

#include "casters.h"

#include <pybind11/numpy.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;

#if defined(Q_OS_WIN32)
const char *MODULE_EXTENSION = ".pyd";

QString getPluginsWrappersPath()
{
#ifdef PYCC_STAND_ALONE
    py::module_ sysconfig = py::module_::import("sysconfig");
    const QString site_packages = sysconfig.attr("get_path")("purelib").cast<QString>();
    return QString("%1\\pycc\\plugins-python").arg(site_packages);
#else
    QDir appDir = QCoreApplication::applicationDirPath();
    QString pluginPath = QString("%1\\plugins-python").arg(appDir.absolutePath());
    return pluginPath;
#endif
}
#elif defined(Q_OS_MACOS)
const char *MODULE_EXTENSION = ".so";

QString getPluginsWrappersPath()
{
#ifdef PYCC_STAND_ALONE
    py::module_ sys = py::module_::import("sys");
    const QString envPrefix = sys.attr("prefix").cast<QString>();
    QString pluginPath = QString("%1/lib/cloudcompare/plugins-python").arg(envPrefix);
#else
    // This point to $InstallFolder/CloudCompare.app/Contents/MacOS
    // We store python plugin wrappers in
    // $InstallFolder/CloudCompare.app/Contents/PlugIns/ccPythonPlugins
    QDir appDir = QCoreApplication::applicationDirPath();
    appDir.cdUp();
    QString pluginPath = QString("%1/PlugIns/ccPythonPlugins").arg(appDir.absolutePath());
#endif
    return pluginPath;
}
#else
const char *MODULE_EXTENSION = ".so";

QString guessPlatlibdir()
{
    // TODO it might be interesting to get the lib64 / lib from a config.h generated by cmake
    //  instead of calling python
    py::module_ sys = py::module_::import("sys");
    const QString platlibdir = sys.attr("platlibdir").cast<QString>();
    return platlibdir;
}

QString getPluginsWrappersPath()
{
#ifdef PYCC_STAND_ALONE
    // We put the python wrapper in $venvPrefix/lib(64)/cloudcompare/plugins-python
    py::module_ sys = py::module_::import("sys");
    const QString envPrefix = sys.attr("prefix").cast<QString>();
    const QString platlibdir = guessPlatlibdir();
    QString pluginPath = QString("%1/%2/cloudcompare/plugins-python").arg(envPrefix, platlibdir);
#else  // PYCC_STAND_ALONE

    const QString platlibdir = guessPlatlibdir();
    QDir theDir = QCoreApplication::applicationDirPath();
    if (theDir.dirName() == "bin")
    {
        theDir.cdUp();
    }

    QString pluginPath =
        QString("%1/%2/cloudcompare/plugins-python").arg(theDir.absolutePath(), platlibdir);
#endif // PYCC_STAND_ALONE
    return pluginPath;
}
#endif // Q_OS_WIN32

void load_pluginWrappers(py::module_ &m)
{
    py::module_ pluginWrappers = m.def_submodule("plugins");
    pluginWrappers.doc() = "Module we wrap around some CloudCompare plugins";

    const QString pluginPath = getPluginsWrappersPath();

    QDir pluginPathDir(pluginPath);
    if (!pluginPathDir.exists())
    {
        return;
    }

    py::module_::import("sys").attr("path").attr("append")(pluginPath);
    QDirIterator iter(pluginPathDir);
    while (iter.hasNext())
    {
        iter.next();
        QFileInfo entry = iter.fileInfo();
        QString fileName = entry.fileName();

        if (!fileName.endsWith(MODULE_EXTENSION))
        {
            continue;
        }

        // Our wrappers are wheels with name like "pluginName.cpython.etc.etc.so"
        const std::string stdFileName = fileName.split('.')[0].toStdString();
        try
        {
            const py::module mod = py::module_::import(stdFileName.c_str());

            const std::string pluginName = mod.attr("plugin_name").cast<std::string>();
            pluginWrappers.attr(pluginName.c_str()) = mod;
        }
        catch (const std::exception &e)
        {
            py::print("Failed to load plugin '", stdFileName, "': ", e.what());
            continue;
        }
    }
}
