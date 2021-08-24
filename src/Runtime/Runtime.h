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
struct RegisteredFunction
{
    explicit RegisteredFunction(const py::dict &dict) noexcept(false)
    {
        name = dict["name"].cast<QString>();
        target = dict["target"];
    }

    void operator()()
    {
        target();
    }

    bool operator==(const RegisteredFunction &other) const
    {
        return name == other.name && target.is(other.target);
    }

    QString name{};
    pybind11::object target{};
};

std::vector<RegisteredFunction> &registeredFunctions();
/// This must be called before finalizing the interpreter
void clearRegisteredFunction();

void setMainAppInterfaceInstance(ccMainAppInterface *appInterface) noexcept(false);
void unsetMainAppInterfaceInstance() noexcept;

void setCmdLineInterfaceInstance(ccCommandLineInterface *cmdLine) noexcept;
void unsetCmdLineInterfaceInstance() noexcept;

} // namespace Runtime
