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
#ifndef PYTHON_PLUGIN_PYTHON_PLUGIN_MANAGER_H
#define PYTHON_PLUGIN_PYTHON_PLUGIN_MANAGER_H

#include <vector>

#include "Runtime/Runtime.h"

/// The Manager class (#OOP) for the plugins,
/// responsible for loading and storing them
class PythonPluginManager final
{
  public:
    PythonPluginManager() = default;

    /// Returns the currently loaded plugins
    const std::vector<Runtime::RegisteredPlugin> &plugins() const;

    /// Autodiscover plugins based on the package metadata.
    /// it uses the entry point mechanism described
    /// - In PyPa docs:
    ///   https://packaging.python.org/en/latest/guides/creating-and-discovering-plugins/
    /// - In SetupTools docs:
    ///   https://setuptools.pypa.io/en/latest/userguide/entry_point.html
    /// The entry point group "cloudcompare.plugins" is searched among all packages included
    /// in the current site-package, then we try to load the plugin from the "entry.value" that must
    /// be formed in this fashion: "package_and_module:SubClassOfPythonPluginInterface". The name of
    /// the plugin is infered from the entry.name
    void loadPluginsFromEntryPoints();

    /// Loads the plugins from the given path.
    ///
    /// A plugin is either a `.py` file or a directory.
    ///
    /// \param paths where we will look for plugins to load
    void loadPluginsFrom(const QStringList &paths);

    /// This MUST be called before finalizing the interpreter
    void unloadPlugins();

  private:
    std::vector<Runtime::RegisteredPlugin> m_plugins;
};

#endif // PYTHON_PLUGIN_PYTHON_PLUGIN_MANAGER_H
