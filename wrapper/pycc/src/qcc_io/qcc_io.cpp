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

#include "../casters.h"

#include <FileIOFilter.h>

#include <QWidget>

namespace py = pybind11;
using namespace pybind11::literals;

void define_qcc_io(py::module &m)
{
    py::class_<ccGlobalShiftManager> PyccGlobalShiftManager(m, "ccGlobalShiftManager");

    py::enum_<ccGlobalShiftManager::Mode>(PyccGlobalShiftManager, "Mode")
        .value("NO_DIALOG", ccGlobalShiftManager::Mode::NO_DIALOG)
        .value("NO_DIALOG_AUTO_SHIFT", ccGlobalShiftManager::Mode::NO_DIALOG_AUTO_SHIFT)
        .value("DIALOG_IF_NECESSARY", ccGlobalShiftManager::Mode::DIALOG_IF_NECESSARY)
        .value("ALWAYS_DISPLAY_DIALOG", ccGlobalShiftManager::Mode::ALWAYS_DISPLAY_DIALOG)
        .export_values();

    py::class_<FileIOFilter> PyFileIOFilter(m, "FileIOFilter");
    PyFileIOFilter.def_static(
        "LoadFromFile",
        [](const QString &filename, FileIOFilter::LoadParameters &parameters) {
            CC_FILE_ERROR result = CC_FERR_NO_ERROR;
            ccHObject *newGroup = FileIOFilter::LoadFromFile(filename, parameters, result);
            return newGroup;
        },
        py::return_value_policy::reference);

    py::class_<FileIOFilter::LoadParameters>(PyFileIOFilter, "LoadParameters")
        .def(py::init<>())
        .def_readwrite("shiftHandlingMode", &FileIOFilter::LoadParameters::shiftHandlingMode)
        .def_readwrite("alwaysDisplayLoadDialog", &FileIOFilter::LoadParameters::alwaysDisplayLoadDialog)
        .def_readwrite("coordinatesShiftEnabled", &FileIOFilter::LoadParameters::coordinatesShiftEnabled)
        .def_readwrite("coordinatesShift",
                       &FileIOFilter::LoadParameters::coordinatesShift,
                       py::return_value_policy::reference)
        .def_readwrite("preserveShiftOnSave", &FileIOFilter::LoadParameters::preserveShiftOnSave)
        .def_readwrite("autoComputeNormals", &FileIOFilter::LoadParameters::autoComputeNormals)
        .def_readwrite(
            "parentWidget", &FileIOFilter::LoadParameters::parentWidget, py::return_value_policy::reference)
        .def_readwrite("sessionStart", &FileIOFilter::LoadParameters::sessionStart);
}