#include <pybind11/pybind11.h>


#include <DgmOctreeReferenceCloud.h>
#include <GenericIndexedCloudPersist.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_DgmOctreeReferenceCloud(py::module &cccorelib)
{
	py::class_<CCCoreLib::DgmOctreeReferenceCloud, CCCoreLib::GenericIndexedCloudPersist>
			(cccorelib, "DgmOctreeReferenceCloud")
			.def("forwardIterator", &CCCoreLib::DgmOctreeReferenceCloud::forwardIterator);
}