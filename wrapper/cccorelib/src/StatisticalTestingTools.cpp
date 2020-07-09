#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <StatisticalTestingTools.h>
#include <GenericCloud.h>
#include <GenericDistribution.h>
#include <GenericIndexedCloudPersist.h>
#include <GenericProgressCallback.h>

namespace py = pybind11;
using namespace pybind11::literals;


void define_StatisticalTestingTools(py::module &cccorelib)
{
	py::class_<CCCoreLib::StatisticalTestingTools>(cccorelib, "StatisticalTestingTools")
			.def_static("computeAdaptativeChi2Dist", &CCCoreLib::StatisticalTestingTools::computeAdaptativeChi2Dist,
			            "distrib"_a, "cloud"_a, "numberOfClasses"_a, "finalNumberOfClasses"_a,
			            "noClassCompression"_a = false, "histoMin"_a = nullptr, "histoMax"_a = nullptr,
			            "histoValues"_a = nullptr, "npis"_a = nullptr)
			.def_static("computeChi2Fractile", &CCCoreLib::StatisticalTestingTools::computeChi2Fractile, "p"_a, "d"_a)
			.def_static("computeChi2Probability", &CCCoreLib::StatisticalTestingTools::computeChi2Probability,
			            "chi2result"_a, "d"_a)
			.def_static("testCloudWithStatisticalModel",
			            &CCCoreLib::StatisticalTestingTools::testCloudWithStatisticalModel,
			            "distrib"_a, "theCloud"_a, "numberOfNeighbours"_a, "pTrust"_a, "progressCb"_a = nullptr,
			            "inputOctree"_a = nullptr);
}