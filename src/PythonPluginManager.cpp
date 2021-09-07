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
#include "PythonPluginManager.h"

#include <QDirIterator>
#include <QFileInfo>

const std::vector<Runtime::RegisteredPlugin> &PythonPluginManager::plugins() const
{
    return m_plugins;
}

// Loading python plugins works in 3 steps
// 1. Add the path from where we want to load the plugin(s) to
//    the PythonPath (so that wa can import them)
// 2. Import the file if its a .py file or import directory name
// 3. instanciate subclasses of our interface for Python Plugin
void PythonPluginManager::loadPluginsFrom(const QString &paths)
{
    if (paths.isEmpty())
    {
        return;
    }
#ifdef Q_OS_WIN
    const QChar sep = ';';
#else
    const QChar sep = ':';
#endif

    ccLog::Print("[PythonPlugin] Searching for custom plugin");
    QStringList pluginsPaths = paths.split(sep);
    py::module::import("sys").attr("path").attr("append")(paths);
    for (const QString &path : pluginsPaths)
    {
        ccLog::Print(QString("[PythonPlugin]     searching in %1").arg(path));
        QDirIterator iter(path);
        while (iter.hasNext())
        {
            iter.next();
            QFileInfo entry = iter.fileInfo();
            QString fileName = entry.fileName();

            if (fileName == "." || fileName == ".." || fileName == "__pycache__")
            {
                continue;
            }

            QString nameToImport = fileName;
            if (!entry.isDir() && fileName.endsWith(".py"))
            {
                nameToImport = fileName.left(fileName.size() - 3);
            }

            const std::string nameToImportStd = nameToImport.toStdString();
            try
            {
                py::module::import(nameToImportStd.c_str());
                ccLog::Print("[PythonPlugin]     Loaded plugin '%s'", nameToImportStd.c_str());
            }
            catch (const std::exception &e)
            {
                ccLog::Warning("[PythonPlugin]     Failed to load plugin '%s': %s",
                               nameToImportStd.c_str(),
                               e.what());
            }
        }
    }

    py::list subClassTypes =
        py::module::import("pycc_runtime").attr("PythonPluginInterface").attr("__subclasses__")();
    for (auto &subClassType : subClassTypes)
    {
        try
        {
            Runtime::RegisteredPlugin plugin =
                Runtime::RegisteredPlugin::InstanciatePlugin(subClassType.cast<py::object>());
            m_plugins.push_back(plugin);
        }
        catch (const std::exception &e)
        {
            ccLog::Warning("[PythonPlugin]     Failed to instantiate plugin: %s", e.what());
        }
    }
}

void PythonPluginManager::unloadPlugins()
{
    m_plugins.clear();
}