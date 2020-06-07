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
//#                             COPYRIGHT: Thomas Montaigu                 #
//#                                                                        #
//##########################################################################

#undef slots

#include <pybind11/embed.h>

#include "ccMainAppInterface.h"
#include "exposed.h"

namespace py = pybind11;

static ccMainAppInterface *s_appInterface{nullptr};

ccHObject::Container GetSelectedEntities() {
    if (s_appInterface == nullptr)
    {
        throw std::runtime_error("missing MainAppInterface, is the Python Plugin running ?");
    } else
    {
        return s_appInterface->getSelectedEntities();
    }
}

void PrintMessage(const char *message) {
    ccLog::Print(message);
}

void PrintWarning(const char *message) {
    ccLog::Warning(message);
}

void PrintError(const char *message) {
    ccLog::Error(message);
}


namespace Python {
    // This is an example of an action's method called when the corresponding action
    // is triggered (i.e. the corresponding icon or menu entry is clicked in CC's
    // main interface). You can access most of CC's components (database,
    // 3D views, console, etc.) via the 'appInterface' variable.
    void performActionA(ccMainAppInterface *appInterface) {
        if (appInterface == nullptr)
        {
            // The application interface should have already been initialized when the plugin is loaded
            Q_ASSERT(false);
            return;
        }


        if (s_appInterface == nullptr)
        {
            s_appInterface = appInterface;
        }

//        py::scoped_interpreter guard{};


        try
        {
            py::eval_file("script.py");
        } catch (const std::exception &e)
        {
            appInterface->dispToConsole(QString("[Python] %1").arg(e.what()), ccMainAppInterface::ERR_CONSOLE_MESSAGE);
        }

        //py::exec(R"(raise RuntimeError("koukou"))");
        //py::exec(R"(pycc.test)");

        /*** HERE ENDS THE ACTION ***/
    }
}
