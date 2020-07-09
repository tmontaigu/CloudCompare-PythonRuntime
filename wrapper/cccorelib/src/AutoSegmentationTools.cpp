#include <pybind11/pybind11.h>

#include <AutoSegmentationTools.h>
#include <GenericIndexedCloudPersist.h>
#include <GenericProgressCallback.h>
#include <DgmOctree.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_AutoSegmentationTools(py::module& cccorelib) {
	py::class_<CCCoreLib::AutoSegmentationTools> AutoSegmentationTools(cccorelib, "AutoSegmentationTools");


	AutoSegmentationTools.def_static("labelConnectedComponents",
	                                 &CCCoreLib::AutoSegmentationTools::labelConnectedComponents, "theCloud"_a,
	                                 "level"_a,
	                                 "sixConnexity"_a = false, "progressCb"_a = nullptr, "inputOctree"_a = nullptr);

	AutoSegmentationTools.def_static("extractConnectedComponents",
	                                 &CCCoreLib::AutoSegmentationTools::extractConnectedComponents);

	AutoSegmentationTools.def_static("frontPropagationBasedSegmentation",
	                                 &CCCoreLib::AutoSegmentationTools::frontPropagationBasedSegmentation,
	                                 "theCloud"_a, "radius"_a, "minSeeDist"_a, "octreeLevel"_a, "theSegmentedLists"_a,
	                                 "progressCb"_a = nullptr,
	                                 "inputOctree"_a = nullptr, "applyGaussianFilter"_a = false, "alpha"_a = 2.0f);
}