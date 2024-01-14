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
#include "PythonPluginManager.h"
#include "Utilities.h"

#include <QDirIterator>
#include <QFileInfo>

using namespace pybind11::literals;

const std::vector<Runtime::RegisteredPlugin> &PythonPluginManager::plugins() const
{
    return m_plugins;
}

void PythonPluginManager::loadPluginsFromEntryPoints()
{
    plgPrint() << "Searching for custom plugins (checking metadata in site-package)";
    const py::object versionInfo = py::module::import("sys").attr("version_info");

    const py::object metadata = py::module::import("importlib.metadata");
    py::iterable entries;

    // Get entry points filtered by group="cloudcompare.plugins"
    if (versionInfo < py::make_tuple(3, 10))
    {
        const py::dict entries_dict = metadata.attr("entry_points")();
        if (entries_dict.contains("cloudcompare.plugins"))
        {
            entries = entries_dict["cloudcompare.plugins"];
        }
        else
        {
            plgDebug() << "No custom plugin registered in site-packages";

            return;
        }
    }
    else
    {
        entries = metadata.attr("entry_points")(
            "group"_a = "cloudcompare.plugins"); // return a list of entries
    }

    plgDebug() << py::str(entries).cast<QString>();

    for (auto &entry : entries)
    {
        QString pluginName = entry.attr("name").cast<QString>();
        QString entry_point = entry.attr("value").cast<QString>();

        QStringList entry_pieces = entry_point.split(':');
        if (entry_pieces.size() != 2)
        { // Entry point value should be in the form package_and_module:pluginclass
            plgWarning() << "Malformed entry point specification for '" << pluginName << "':'"
                         << entry_point << "'";
            return;
        }

        // No need to check that it's a subclass of PythonPluginInterface, the instanciation
        // and exception will work for us.
        try
        {
            py::object plugin_class = py::module::import(entry_pieces[0].toStdString().c_str())
                                          .attr(entry_pieces[1].toStdString().c_str());
            Runtime::RegisteredPlugin plugin =
                Runtime::RegisteredPlugin::InstanciatePlugin(plugin_class, pluginName);
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

// Loading python plugins from path works in 3 steps
// 1. Add the path from where we want to load the plugin(s) to
//    the PythonPath (so that we can import them)
// 2. Import the file if it's a .py file or import directory name
// 3. Create instances of subclasses of our interface for Python Plugin
void PythonPluginManager::loadPluginsFrom(const QStringList &paths)
{
    if (paths.isEmpty())
    {
        return;
    }

    plgPrint() << "Searching for custom plugins (in custom paths)";
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
