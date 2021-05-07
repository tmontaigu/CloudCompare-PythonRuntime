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

#include "ccGuiPythonInstance.h"


#include <QMainWindow>

#include <ccGLWindow.h>

#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccGUIPythonInstance(py::module &m)
{
    py::class_<ccGuiPythonInstance>(m,
                                    "ccPythonInstance",
                                    R"(
    Class to interact with the running GUI of CloudCompare.

    The methods allow you to get/add objects to GUI DB Tree.
    Refresh/freeze/update the UI and display.
    Get selected objects, deselect/set selected state for entities...

)")
        .def("getMainWindow", &ccGuiPythonInstance::getMainWindow, py::return_value_policy::reference)
        .def("getActiveGLWindow", &ccGuiPythonInstance::getActiveGLWindow, py::return_value_policy::reference)
        .def("haveSelection",
             &ccGuiPythonInstance::haveSelection,
             R"(Returns true if at least one entity is selected in the GUI DB Tree)")
        .def("haveOneSelection",
             &ccGuiPythonInstance::haveOneSelection,
             R"(Returns True if exactly one entity is selected in the GUI DB Tree)")
        .def("getSelectedEntities",
             &ccGuiPythonInstance::getSelectedEntities,
             py::return_value_policy::reference,
             R"(Returns the list of selected entities in the GUI DB Tree)")
        .def("setSelectedInDB",
             &ccGuiPythonInstance::setSelectedInDB,
             "obj"_a,
             "selected"_a,
             R"(Set the selection state of the object in the GUI DB Tree)")
        .def("dbRootObject",
             &ccGuiPythonInstance::dbRootObject,
             py::return_value_policy::reference,
             R"(Returns the root of the GUI DB Tree)")
        .def("addToDB",
             &ccGuiPythonInstance::addToDB,
             "obj"_a,
             "updateZoom"_a = false,
             "autoExpandDBTree"_a = true,
             "checkDimensions"_a = false,
             "autoRedraw"_a = true,
             R"(Adds the object to the GUI DB Tree)")
        .def("redrawAll", &ccGuiPythonInstance::redrawAll, "only2D"_a = false)
        .def("refreshAll", &ccGuiPythonInstance::refreshAll, "only2D"_a = false)
        .def("enableAll", &ccGuiPythonInstance::enableAll)
        .def("disableAll", &ccGuiPythonInstance::disableAll)
        .def("updateUI", &ccGuiPythonInstance::updateUI)
        .def("freezeUI", &ccGuiPythonInstance::freezeUI, "state"_a)
        .def("loadFile",
             &ccGuiPythonInstance::loadFile,
             py::return_value_policy::reference,
             "filepath"_a,
             "loadParameters"_a,
             R"(
    Loads the file located at the filepath.

    Adds the file content of the file into the DBTree
    and then returns loaded object handle.

    raises RuntimeError in case of error
)");
}
