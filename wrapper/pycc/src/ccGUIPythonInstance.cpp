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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <QMainWindow>

#include "ccGUIPythonInstance.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccGUIPythonInstance(py::module &m)
{
    py::class_<ccGUIPythonInstance>(m, "ccPythonInstance")
        .def("getMainWindow", &ccGUIPythonInstance::getMainWindow, py::return_value_policy::reference)
        .def("haveSelection", &ccGUIPythonInstance::haveSelection)
        .def("haveOneSelection", &ccGUIPythonInstance::haveOneSelection)
        .def("getSelectedEntities",
             &ccGUIPythonInstance::getSelectedEntities,
             py::return_value_policy::reference)
        .def("setSelectedInDB", &ccGUIPythonInstance::setSelectedInDB)
        .def("dbRootObject", &ccGUIPythonInstance::dbRootObject, py::return_value_policy::reference)
        .def("addToDB",
             &ccGUIPythonInstance::addToDB,
             "obj"_a,
             "updateZoom"_a = false,
             "autoExpandDBTree"_a = true,
             "checkDimensions"_a = false,
             "autoRedraw"_a = true)
        .def("redrawAll", &ccGUIPythonInstance::redrawAll)
        .def("refreshAll", &ccGUIPythonInstance::refreshAll)
        .def("enableAll", &ccGUIPythonInstance::enableAll)
        .def("disableAll", &ccGUIPythonInstance::disableAll)
        .def("updateUI", &ccGUIPythonInstance::updateUI)
        .def("freezeUI", &ccGUIPythonInstance::freezeUI)
        .def("loadFile",
             &ccGUIPythonInstance::loadFile,
             py::return_value_policy::reference,
             "filepath"_a,
             "loadParameters"_a,
             R"(
    Loads the file located at the filepath.

    Adds the file content of the file into the DBTree
    and then returns loaded object handle.

    raises RuntimeError in case of error
)")
        .def("createObject",
             &ccGUIPythonInstance::createObject,
             py::return_value_policy::reference,
             "type_name"_a);
}