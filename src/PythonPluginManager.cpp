// ##########################################################################
// #                                                                        #
// #                CLOUDCOMPARE PLUGIN: PythonPlugin                       #
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
#include "PythonPluginManager.h"
#include "Utilities.h"

#include <QDirIterator>
#include <QFileInfo>

const std::vector<Runtime::RegisteredPlugin> &PythonPluginManager::plugins() const
{
    return m_plugins;
}

// Loading python plugins works in 3 steps
// 1. Add the path from where we want to load the plugin(s) to
//    the PythonPath (so that wa can import them)
// 2. Import the file if it's a .py file or import directory name
// 3. Create instances of subclasses of our interface for Python Plugin
void PythonPluginManager::loadPluginsFrom(const QStringList &paths)
{
    if (paths.isEmpty())
    {
        return;
    }

    plgPrint() << "Searching for custom plugin";
    py::object appendToPythonSysPath = py::module::import("sys").attr("path").attr("append");
    for (const QString &path : paths)
    {
        plgPrint() << "Searching in " << path;
        appendToPythonSysPath(path);
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
                plgDebug() << "\tLoaded python module '" << nameToImportStd.c_str() << "'";
            }
            catch (const std::exception &e)
            {
                plgWarning() << "\tFailed to python module '" << nameToImportStd.c_str()
                             << e.what();
            }
        }
    }

    py::list subClassTypes =
        py::module::import("pycc_runtime").attr("PythonPluginInterface").attr("__subclasses__")();
    for (auto &subClassType : subClassTypes)
    {
        QString pluginName;
        if (hasattr(subClassType, "__name__"))
        {
            pluginName = subClassType.attr("__name__").cast<QString>();
        }
        try
        {
            Runtime::RegisteredPlugin plugin =
                Runtime::RegisteredPlugin::InstanciatePlugin(subClassType.cast<py::object>());
            m_plugins.push_back(plugin);
            plgPrint() << "\tLoaded plugin: '" << pluginName << "'";
        }
        catch (const std::exception &e)
        {
            plgWarning() << "\tFailed to instantiate plugin named '" << pluginName
                         << "'\nThe error was:\n"
                         << e.what();
        }
    }
}

void PythonPluginManager::unloadPlugins()
{
    m_plugins.clear();
}
