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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <ccHObject.h>
#include <ccHObjectCaster.h>
#include <ccPointCloud.h>

#include "exposed.h"


namespace py = pybind11;


PYBIND11_MODULE(pycc, m) {
    m.doc() = R"pbdoc(
        Python module exposing some CloudCompare functions
    )pbdoc";


    m.def("GetSelectedEntities", &GetSelectedEntities, py::return_value_policy::automatic_reference,
          R"pbdoc(Returns the list of currently selected entities in the CloudCompare DB Tree)pbdoc");

    m.def("PrintMessage", &PrintMessage);
    m.def("PrintWarning", &PrintWarning);
    m.def("PrintError", &PrintError);

    py::class_<ccPointCloud>(m, "ccPointCloud")
            .def("getName", [](const ccPointCloud *self) { return self->getName().toStdString(); }
            )
            .def("setName", [](ccPointCloud *self, const char *newName) { self->setName(newName); })
            .def("size", &ccPointCloud::size);

    py::class_<ccHObject>(m, "ccHObject")
            .def("toPointCloud", [](ccHObject *obj) {
                bool aya{false};
                return ccHObjectCaster::ToPointCloud(obj, &aya);
            });
}