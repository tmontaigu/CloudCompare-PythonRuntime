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

#include <CCMiscTools.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_CCMiscTools(py::module &cccorelib)
{
    py::class_<CCCoreLib::CCMiscTools> CCMiscTools(cccorelib, "CCMiscTools");
    CCMiscTools.def_static(
        "EnlargeBox", &CCCoreLib::CCMiscTools::EnlargeBox, "dimMin"_a, "dimMax"_a, "coef"_a);
    CCMiscTools.def_static("MakeMinAndMaxCubical",
                           &CCCoreLib::CCMiscTools::MakeMinAndMaxCubical,
                           "dimMin"_a,
                           "dimMax"_a,
                           "enlargeFactor"_a = 0.01);

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

    CCMiscTools.def_static(
        "TriBoxOverlap",
        [](const CCVector3 &boxCenter, const CCVector3 boxhalfsize, py::list &triverts)
        {
            const CCVector3 *trueTrivers[3] = {
                triverts[0].cast<CCVector3 *>(),
                triverts[1].cast<CCVector3 *>(),
                triverts[2].cast<CCVector3 *>(),
            };

            return CCCoreLib::CCMiscTools::TriBoxOverlap(boxCenter, boxhalfsize, trueTrivers);
        },
        "boxcenter"_a,
        "boxhalfsize"_a,
        "triverts"_a);

    CCMiscTools.def_static("TriBoxOverlapd",
                           CCCoreLib::CCMiscTools::TriBoxOverlapd,
                           "boxcenter"_a,
                           "boxhalfsize"_a,
                           "triverts"_a);
}
