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

#include <ccCommandLineInterface.h>
#include <ccGenericMesh.h>

#include "../../wrapper/pycc/src/casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MAKE_OPAQUE(std::vector<CLCloudDesc>);
PYBIND11_MAKE_OPAQUE(std::vector<CLMeshDesc>);

void define_ccCommandLine(py::module &m)
{

    py::bind_vector<std::vector<CLCloudDesc>>(m, "CLCloudDescVector");
    py::bind_vector<std::vector<CLMeshDesc>>(m, "CLMeshDescVector");

    py::enum_<CL_ENTITY_TYPE>(m, "CL_ENTITY_TYPE")
        .value("GROUP", CL_ENTITY_TYPE::GROUP)
        .value("CLOUD", CL_ENTITY_TYPE::CLOUD)
        .value("MESH", CL_ENTITY_TYPE::MESH);

    py::class_<CLEntityDesc>(m, "CLEntityDesc")
        .def_readonly("basename", &CLEntityDesc::basename)
        .def_readonly("path", &CLEntityDesc::path)
        .def_readonly("indexInFile", &CLEntityDesc::indexInFile)
        .def("getEntity",
             (ccHObject * (CLEntityDesc::*)())(&CLEntityDesc::getEntity),
             py::return_value_policy::reference);

    py::class_<CLGroupDesc, CLEntityDesc>(m, "CLGroupDesc")
        .def_readonly("groupEntity", &CLGroupDesc::groupEntity, py::return_value_policy::reference);

    py::class_<CLCloudDesc, CLEntityDesc>(m, "CLCloudDesc")
        .def_readonly("pc", &CLCloudDesc::pc, py::return_value_policy::reference);

    py::class_<CLMeshDesc, CLEntityDesc>(m, "CLMeshDesc")
        .def_readonly("mesh", &CLMeshDesc::mesh, py::return_value_policy::reference);

    py::class_<ccCommandLineInterface> PyccCommandLineInterface(m, "ccCommandLineInterface");

    py::enum_<ccCommandLineInterface::ExportOption>(PyccCommandLineInterface, "ExportOption");

    py::class_<ccCommandLineInterface::GlobalShiftOptions> PyGlobalShiftOptions(
        PyccCommandLineInterface, "GlobalShiftOptions");

    py::enum_<ccCommandLineInterface::GlobalShiftOptions::Mode>(PyGlobalShiftOptions, "Mode")
        .value("NO_GLOBAL_SHIFT", ccCommandLineInterface::GlobalShiftOptions::Mode::NO_GLOBAL_SHIFT)
        .value("AUTO_GLOBAL_SHIFT",
               ccCommandLineInterface::GlobalShiftOptions::Mode::AUTO_GLOBAL_SHIFT)
        .value("FIRST_GLOBAL_SHIFT",
               ccCommandLineInterface::GlobalShiftOptions::Mode::FIRST_GLOBAL_SHIFT)
        .value("CUSTOM_GLOBAL_SHIFT",
               ccCommandLineInterface::GlobalShiftOptions::Mode::CUSTOM_GLOBAL_SHIFT)
        .export_values();

    PyGlobalShiftOptions.def_readwrite("mode", &ccCommandLineInterface::GlobalShiftOptions::mode)
        .def_readwrite("customGlobalShift",
                       &ccCommandLineInterface::GlobalShiftOptions::customGlobalShift)
        .def(py::init<>());

    PyccCommandLineInterface
        .def("clouds",
             (std::vector<CLCloudDesc> &
              (ccCommandLineInterface::*)())(&ccCommandLineInterface::clouds),
             py::return_value_policy::reference)
        .def("meshes",
             (std::vector<CLMeshDesc> &
              (ccCommandLineInterface::*)())(&ccCommandLineInterface::meshes))
        .def("getExportFilename",
             &ccCommandLineInterface::getExportFilename,
             "entityDesc"_a,
             "extension"_a = QString(),
             "suffix"_a = QString(),
             "baseOutputFilename"_a = nullptr,
             "forceNoTimestamp"_a = false)
        .def("exportEntity",
             &ccCommandLineInterface::exportEntity,
             "entityDesc"_a,
             "suffix"_a = QString(),
             "outputFilename"_a = nullptr,
             "options"_a = ccCommandLineInterface::ExportOption::NoOptions)
        .def("saveCloud",
             &ccCommandLineInterface::saveClouds,
             "suffix"_a = QString(),
             "allAtOnce"_a = false,
             "allAtOnceFileName"_a = nullptr)
        .def("saveMeshes",
             &ccCommandLineInterface::saveMeshes,
             "suffix"_a = QString(),
             "allAtOnce"_a = false,
             "allAtOnceFileName"_a = nullptr)
        .def(
            "importFile",
            [](ccCommandLineInterface &self,
               QString filename,
               const ccCommandLineInterface::GlobalShiftOptions &opts)
            { return self.importFile(filename, opts); },
            "filename"_a,
            "opts"_a);
}
