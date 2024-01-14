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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <GenericProgressCallback.h>
#include <ccProgressDialog.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccProgressDialog(py::module &m)
{
    py::class_<ccProgressDialog, QProgressDialog, CCCoreLib::GenericProgressCallback>(
        m, "ccProgressDialog")
        .def(py::init<bool>(), "cancelButton"_a = false)
        .def("setMethodTitle",
             (void(ccProgressDialog::*)(QString)) & ccProgressDialog::setMethodTitle,
             "methodTitle"_a)
        .def("setInfo",
             (void(ccProgressDialog::*)(QString)) & ccProgressDialog::setInfo,
             "infoStr"_a);
}
