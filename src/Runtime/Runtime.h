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

#pragma once

#include <pybind11/pytypes.h>
class ccMainAppInterface;
class ccCommandLineInterface;

#include "../../wrapper/pycc/src/casters.h"

namespace py = pybind11;

namespace Runtime
{

struct RegisteredPlugin
{
    struct Action
    {
        Action() = delete;
        Action(QString name, pybind11::object target, pybind11::object icon = pybind11::none())
            : name(std::move(name)), target(std::move(target)), icon(std::move(icon))
        {
        }

        /// Name to be displayed in the UI
        QString name{};
        /// The target python function (or method)
        pybind11::object target{};
        /// Optional path or (bytes, str) where str is the format
        pybind11::object icon{};
    };

    /// Instanciate a plugin with a known name
    static RegisteredPlugin InstanciatePlugin(pybind11::object class_type,
                                              const QString &name) noexcept(false)
    {
        pybind11::object instance = class_type();
        py::list pyActions = instance.attr("getActions")();
        std::vector<Action> actions;
        actions.reserve(pyActions.size());

        for (const py::handle &handle : pyActions)
        {
            actions.push_back(handle.cast<Runtime::RegisteredPlugin::Action>());
        }

        const pybind11::object mainIcon = instance.attr("getIcon")();

        return {name, instance, actions, mainIcon};
    }

    /// Instanciate a plugin with an unknown name.
    /// In this case the name will be infered from the python class name.
    static RegisteredPlugin InstanciatePlugin(pybind11::object class_type) noexcept(false)
    {
        QString name = class_type.attr("__name__").cast<QString>();
        return InstanciatePlugin(class_type, name);
    }

    QString name;
    pybind11::object instance;
    std::vector<Action> actions;
    pybind11::object mainIcon;
};

/// Sets the internal pointer to the app interface that python
/// scripts will be able to access in GUI mode
void setMainAppInterfaceInstance(ccMainAppInterface *appInterface) noexcept(false);
/// Unsets the app interface pointer
void unsetMainAppInterfaceInstance() noexcept;

/// Sets the internal pointer to the command line app interface that
/// python script will be able to access in the CMD Line mode
void setCmdLineInterfaceInstance(ccCommandLineInterface *cmdLine) noexcept;
/// Unsets the pointer to the cmdline app interface
void unsetCmdLineInterfaceInstance() noexcept;

} // namespace Runtime
