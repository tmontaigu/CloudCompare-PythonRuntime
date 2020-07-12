#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <ccCommandLineInterface.h>
#include <ccGenericMesh.h>

#include "casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccCommandLine(py::module &m) {

	py::enum_<CL_ENTITY_TYPE>(m, "CL_ENTITY_TYPE")
			.value("GROUP", CL_ENTITY_TYPE::GROUP)
			.value("CLOUD", CL_ENTITY_TYPE::CLOUD)
			.value("MESH", CL_ENTITY_TYPE::MESH);

	py::class_<CLEntityDesc>(m, "CLEntityDesc")
			.def_readwrite("basename", &CLEntityDesc::basename)
			.def_readwrite("path", &CLEntityDesc::path)
			.def_readwrite("indexInFile", &CLEntityDesc::indexInFile)
			.def("getEntity", (ccHObject *(CLEntityDesc::*)()) (&CLEntityDesc::getEntity),
			     py::return_value_policy::reference);

	py::class_<CLGroupDesc, CLEntityDesc>(m, "CLGroupDesc")
			.def_readwrite("groupEntity", &CLGroupDesc::groupEntity, py::return_value_policy::reference);

	py::class_<CLCloudDesc, CLEntityDesc>(m, "CLCloudDesc")
			.def_readwrite("pc", &CLCloudDesc::pc, py::return_value_policy::reference);

	py::class_<CLMeshDesc, CLEntityDesc>(m, "CLMeshDesc")
			.def_readwrite("mesh", &CLMeshDesc::mesh, py::return_value_policy::reference);


	py::class_<ccCommandLineInterface> PyccCommandLineInterface(m, "ccCommandLineInterface");

	py::enum_<ccCommandLineInterface::ExportOption>(PyccCommandLineInterface, "ExportOption");

	PyccCommandLineInterface.def("clouds",
	                             (std::vector<CLCloudDesc> &(ccCommandLineInterface::*)()) (&ccCommandLineInterface::clouds))
			.def("meshes", (std::vector<CLMeshDesc> &(ccCommandLineInterface::*)()) (&ccCommandLineInterface::meshes))
			.def("getExportFilename", &ccCommandLineInterface::getExportFilename, "entityDesc"_a,
			     "extension"_a = QString(), "suffix"_a = QString(), "baseOutputFilename"_a = nullptr,
			     "forceNoTimestamp"_a = false)
			.def("exportEntity", &ccCommandLineInterface::exportEntity, "entityDesc"_a, "suffix"_a = QString(),
			     "outputFilename"_a = nullptr, "options"_a = ccCommandLineInterface::ExportOption::NoOptions)
			     .def("saveCloud", &ccCommandLineInterface::saveClouds, "suffix"_a = QString(), "allAtOnce"_a=false, "allAtOnceFileName"_a= nullptr)
			.def("saveMeshes", &ccCommandLineInterface::saveMeshes, "suffix"_a = QString(), "allAtOnce"_a = false,
			     "allAtOnceFileName"_a = nullptr)
			.def("importFile", [](ccCommandLineInterface &self, QString filename) {
				return self.importFile(filename);
			}, "filename"_a);
}