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

#include <ccGBLSensor.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccGBLSensor(py::module &m)
{
    py::class_<ccGBLSensor, ccSensor> pyccGBLSensor(m, "ccGBLSensor");

    py::enum_<ccGBLSensor::ROTATION_ORDER>(pyccGBLSensor, "ROTATION_ORDER")
        .value("YAW_THEN_PITCH", ccGBLSensor::ROTATION_ORDER::YAW_THEN_PITCH)
        .value("PITCH_THEN_YAW", ccGBLSensor::ROTATION_ORDER::PITCH_THEN_YAW)
        .export_values();

    pyccGBLSensor
        .def(py::init<ccGBLSensor::ROTATION_ORDER>(),
             "rotOrder"_a = ccGBLSensor::ROTATION_ORDER::YAW_THEN_PITCH)
        .def("computeAutoParameters", &ccGBLSensor::computeAutoParameters, "theCloud"_a)
        .def_static("GetErrorString", &ccGBLSensor::GetErrorString, "errorCode"_a)
        // setters and getters
        .def("setPitchRange", &ccGBLSensor::setPitchRange, "minPhi"_a, "maxPhi"_a)
        .def("getMinPitch", &ccGBLSensor::getMinPitch)
        .def("getMaxPitch", &ccGBLSensor::getMaxPitch)
        .def("setPitchStep", &ccGBLSensor::setPitchStep, "dPhi"_a)
        .def("getPitchStep", &ccGBLSensor::getPitchStep)
        .def("pitchIsShifted", &ccGBLSensor::pitchIsShifted)
        .def("setYawRange", &ccGBLSensor::setYawRange, "minTheta"_a, "maxTheta"_a)
        .def("getMinYaw", &ccGBLSensor::getMinYaw)
        .def("getMaxYaw", &ccGBLSensor::getMaxYaw)
        .def("setYawStep", &ccGBLSensor::setYawStep, "dTheta"_a)
        .def("getYawStep", &ccGBLSensor::getYawStep)
        .def("yawIsShifted", &ccGBLSensor::yawIsShifted)
        .def("getSensorRange", &ccGBLSensor::getSensorRange)
        .def("setSensorRange", &ccGBLSensor::setSensorRange, "range"_a)
        .def("getUncertainty", &ccGBLSensor::getUncertainty)
        .def("setUncertainty", &ccGBLSensor::setUncertainty, "u"_a)
        .def("getRotationOrder", &ccGBLSensor::getRotationOrder)
        .def("setRotationOrder", &ccGBLSensor::setRotationOrder, "rotOrder"_a)
        // projection tools
        .def(
            "projectPoint",
            [](const ccGBLSensor &self,
               const CCVector3 &sourcePoint,
               double posIndex = 0) -> py::tuple
            {
                CCVector2 destPoint;
                PointCoordinateType depth;
                self.projectPoint(sourcePoint, destPoint, depth, posIndex);
                return py::make_tuple(destPoint, depth);
            },
            "sourcePoint"_a,
            "posIndex"_a = 0);
    // TODO projectNormals
    // TODO projectColors
}
