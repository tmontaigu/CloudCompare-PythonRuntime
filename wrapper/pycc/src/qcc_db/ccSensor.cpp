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

#include <ccGenericGLDisplay.h>
#include <ccSensor.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccSensor(py::module &m)
{
    py::enum_<CC_SENSOR_TYPE>(m, "CC_SENSOR_TYPE")
        .value("UNKNOWN_SENSOR", CC_SENSOR_TYPE::UNKNOWN_SENSOR)
        .value("GROUND_BASED_LIDAR", CC_SENSOR_TYPE::GROUND_BASED_LIDAR);

    py::class_<ccSensor, ccHObject>(m, "ccSensor")
        .def(py::init<const QString &>())
        .def("checkVisibility", &ccSensor::checkVisibility, "P"_a)
        //    TODO.def("getPositions", &ccSensor::getPositions)
        .def("addPosition", &ccSensor::addPosition, "trans"_a, "index"_a)
        .def(
            "getAbsoluteTransformation", &ccSensor::getAbsoluteTransformation, "trans"_a, "index"_a)
        .def("getActiveAbsoluteTransformation",
             &ccSensor::getActiveAbsoluteTransformation,
             "trans"_a)
        .def("getActiveAbsoluteCenter", &ccSensor::getActiveAbsoluteCenter, "vec"_a)
        .def("getActiveAbsoluteRotation", &ccSensor::getActiveAbsoluteRotation, "rotation"_a)
        .def("setRigidTransformation", &ccSensor::setRigidTransformation, "mat"_a)
        .def("getRigidTransformation",
             static_cast<ccGLMatrix &(ccSensor::*)()>(&ccSensor::getRigidTransformation))
        .def("getIndexBounds",
             [](const ccSensor &self)
             {
                 double min, max;
                 self.getIndexBounds(min, max);

                 return py::make_tuple(min, max);
             })
        .def("getActiveIndex", &ccSensor::getActiveIndex)
        .def("setActiveIndex", &ccSensor::setActiveIndex, "index"_a)
        .def("setGraphicScale", &ccSensor::setGraphicScale, "scale"_a)
        .def("getGraphicScale", &ccSensor::getGraphicScale)
        .def("applyViewport", &ccSensor::applyViewport, "win"_a = nullptr);
}
