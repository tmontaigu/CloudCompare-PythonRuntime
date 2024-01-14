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

#include <ccViewportParameters.h>

#include <QRect>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccViewPortParameters(py::module &m)
{
    py::class_<ccViewportParameters>(m, "ccViewportParameters")
        .def_readwrite("viewMat", &ccViewportParameters::viewMat)
        .def_readwrite("defaultPointSize", &ccViewportParameters::defaultPointSize)
        .def_readwrite("defaultLineWidth", &ccViewportParameters::defaultLineWidth)
        .def_readwrite("perspectiveView", &ccViewportParameters::perspectiveView)
        .def_readwrite("objectCenteredView", &ccViewportParameters::objectCenteredView)
        .def_readwrite("zNearCoef", &ccViewportParameters::zNearCoef)
        .def_readwrite("zNearCoef", &ccViewportParameters::zNearCoef)
        .def_readwrite("zNear", &ccViewportParameters::zNear)
        .def_readwrite("zFar", &ccViewportParameters::zFar)
        .def_readwrite("fov_deg", &ccViewportParameters::fov_deg)
        .def_readwrite("nearClippingDepth", &ccViewportParameters::nearClippingDepth)
        .def_readwrite("farClippingDepth", &ccViewportParameters::farClippingDepth)
        .def_readwrite("cameraAspectRatio", &ccViewportParameters::cameraAspectRatio)
        .def(py::init<>())
        .def("setPivotPoint", &ccViewportParameters::setPivotPoint, "P"_a, "autoUpdateFocal"_a)
        .def("getPivotPoint", &ccViewportParameters::getPivotPoint)
        .def("setCameraCenter", &ccViewportParameters::setCameraCenter, "C"_a, "autoUpdateFocal"_a)
        .def("getCameraCenter", &ccViewportParameters::getCameraCenter)
        .def("setFocalDistance", &ccViewportParameters::setFocalDistance, "distance"_a)
        .def("getFocalDistance", &ccViewportParameters::getFocalDistance)
        .def("computeViewMatrix", &ccViewportParameters::computeViewMatrix)
        .def("computeScaleMatrix",
             [](const ccViewportParameters &self, const py::sequence &glViewPort)
             {
                 const QRect vp(glViewPort[0].cast<int>(),
                                glViewPort[1].cast<int>(),
                                glViewPort[2].cast<int>(),
                                glViewPort[3].cast<int>());
                 return self.computeScaleMatrix(vp);
             })
        .def("getViewDir", &ccViewportParameters::getViewDir)
        .def("getUpDir", &ccViewportParameters::getUpDir)
        .def("getRotationCenter", &ccViewportParameters::getRotationCenter)
        .def("computeDistanceToHalfWidthRation",
             &ccViewportParameters::computeDistanceToHalfWidthRatio)
        .def("computeDistanceToWidthRatio", &ccViewportParameters::computeDistanceToWidthRatio)
        .def("computeWidthAtFocalDist", &ccViewportParameters::computeWidthAtFocalDist)
        .def("computePixelSize", &ccViewportParameters::computePixelSize);
}
