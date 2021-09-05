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

#pragma once

class ccMainAppInterface;
class ccCommandLineInterface;

#include "../../wrapper/pycc/src/casters.h"

namespace py = pybind11;

namespace Runtime
{
/// Stores a python function that was "registered" from a python script/module
struct RegisteredFunction
{
    explicit RegisteredFunction(const py::dict &dict) noexcept(false)
    {
        name = dict["name"].cast<QString>();
        target = dict["target"];
    }

    /// Shortcut to call the target function
    void operator()()
    {
        target();
    }

    bool operator==(const RegisteredFunction &other) const
    {
        return name == other.name && target.is(other.target);
    }

    /// Name to be displayed in the UI
    QString name{};
    /// The target python function (or method)
    pybind11::object target{};
};

std::vector<RegisteredFunction> &registeredFunctions();
/// This MUST be called before finalizing the interpreter
void clearRegisteredFunction();

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
