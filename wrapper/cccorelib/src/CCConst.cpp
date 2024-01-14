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

#include <CCConst.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_CCConst(py::module &cccorelib)
{
    /* Constants */
    cccorelib.attr("SQRT_3") = CCCoreLib::SQRT_3;
    // Python's float are doubles
    cccorelib.attr("ZERO_TOLERANCE_F") = CCCoreLib::ZERO_TOLERANCE_F;
    cccorelib.attr("ZERO_TOLERANCE_D") = CCCoreLib::ZERO_TOLERANCE_D;
    cccorelib.attr("ZERO_TOLERANCE_SCALAR") = CCCoreLib::ZERO_TOLERANCE_SCALAR;
    cccorelib.attr("ZERO_TOLERANCE_POINT_COORDINATE") = CCCoreLib::ZERO_TOLERANCE_POINT_COORDINATE;
    cccorelib.attr("PC_ONE") = CCCoreLib::PC_ONE;
    cccorelib.attr("PC_NAN") = CCCoreLib::PC_NAN;
    cccorelib.attr("NAN_VALUE") = CCCoreLib::NAN_VALUE;

    // visibility
    cccorelib.attr("POINT_VISIBLE") = CCCoreLib::POINT_VISIBLE;
    cccorelib.attr("POINT_HIDDEN") = CCCoreLib::POINT_HIDDEN;
    cccorelib.attr("POINT_OUT_OF_RANGE") = CCCoreLib::POINT_OUT_OF_RANGE;
    cccorelib.attr("POINT_OUT_OF_FOV") = CCCoreLib::POINT_OUT_OF_FOV;

    py::enum_<CCCoreLib::CHAMFER_DISTANCE_TYPE>(cccorelib, "CHAMFER_DISTANCE_TYPE")
        .value("CHAMFER_111", CCCoreLib::CHAMFER_DISTANCE_TYPE::CHAMFER_111)
        .value("CHAMFER_345", CCCoreLib::CHAMFER_DISTANCE_TYPE::CHAMFER_345);

    py::enum_<CCCoreLib::LOCAL_MODEL_TYPES>(cccorelib, "LOCAL_MODEL_TYPES")
        .value("NO_MODEL", CCCoreLib::LOCAL_MODEL_TYPES::NO_MODEL)
        .value("LS", CCCoreLib::LOCAL_MODEL_TYPES::LS)
        .value("TRI", CCCoreLib::LOCAL_MODEL_TYPES::TRI)
        .value("QUADRIC", CCCoreLib::LOCAL_MODEL_TYPES::QUADRIC);

    cccorelib.attr("CC_LOCAL_MODEL_MIN_SIZE") = CCCoreLib::CC_LOCAL_MODEL_MIN_SIZE;
}
