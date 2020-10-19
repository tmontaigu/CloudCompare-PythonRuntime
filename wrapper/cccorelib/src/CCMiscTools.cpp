#include <pybind11/pybind11.h>

#include <CCMiscTools.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_CCMiscTools(py::module &cccorelib)
{
    py::class_<CCCoreLib::CCMiscTools> CCMiscTools(cccorelib, "CCMiscTools");
    CCMiscTools.def_static("EnlargeBox", &CCCoreLib::CCMiscTools::EnlargeBox);
    CCMiscTools.def_static("MakeMinAndMaxCubical", &CCCoreLib::CCMiscTools::MakeMinAndMaxCubical);

    CCMiscTools.def_static(
        "ComputeBaseVectors",
        (void (*)(const CCVector3 &, CCVector3 &, CCVector3 &))(&CCCoreLib::CCMiscTools::ComputeBaseVectors),
        "N"_a,
        "X"_a,
        "Y"_a);

    CCMiscTools.def_static("ComputeBaseVectors",
                           (void (*)(const CCVector3d &, CCVector3d &, CCVector3d &))(
                               &CCCoreLib::CCMiscTools::ComputeBaseVectors),
                           "N"_a,
                           "X"_a,
                           "Y"_a);

    //	CCMiscTools.def_static("TriBoxOverlap", &CCCoreLib::CCMiscTools::TriBoxOverlap,
    //	                       "boxcenter"_a, "boxhalfsize"_a, "triverts"_a);

    //	CCMiscTools.def_static("TriBoxOverlapd", CCCoreLib::CCMiscTools::TriBoxOverlapd,
    //	                       "boxcenter"_a, "boxhalfsize"_a, "triverts"_a);
}