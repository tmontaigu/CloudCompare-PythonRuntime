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

#include "../casters.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <QPointF>
#include <QSize>

#include <ccGenericGLDisplay.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccGenericGLDisplay(py::module &m)
{
    py::class_<ccGLCameraParameters>(m, "ccGLCameraParameters")
        .def(py::init<>())
        .def("project",
             (bool(ccGLCameraParameters::*)(const CCVector3d &, CCVector3d &, bool *)
                  const)(&ccGLCameraParameters::project),
             "input3D"_a,
             "output2D"_a,
             "checkInFrustrum"_a = false)
        .def("project",
             (bool(ccGLCameraParameters::*)(const CCVector3 &, CCVector3d &, bool *)
                  const)(&ccGLCameraParameters::project),
             "input3D"_a,
             "output2D"_a,
             "checkInFrustrum"_a = false)
        .def("unproject",
             (bool(ccGLCameraParameters::*)(const CCVector3d &, CCVector3d &)
                  const)(&ccGLCameraParameters::unproject),
             "input3D"_a,
             "output2D"_a)

        .def("unproject",
             (bool(ccGLCameraParameters::*)(const CCVector3 &, CCVector3d &)
                  const)(&ccGLCameraParameters::unproject),
             "input3D"_a,
             "output2D"_a)

        .def_readwrite("modelViewMat", &ccGLCameraParameters::modelViewMat)
        .def_readwrite("projectionMat", &ccGLCameraParameters::projectionMat)
        //        .def_readwrite("viewport", &ccGLCameraParameters::viewport)
        .def_readwrite("perspective", &ccGLCameraParameters::perspective)
        .def_readwrite("fov_deg", &ccGLCameraParameters::fov_deg)
        .def_readwrite("pixelSize", &ccGLCameraParameters::pixelSize);

    py::class_<ccGenericGLDisplay> PyccGenericGLDisplay(m, "ccGenericGLDisplay");

    py::enum_<ccGenericGLDisplay::TextAlign>(PyccGenericGLDisplay, "TextAlign")
        .value("ALIGN_HLEFT", ccGenericGLDisplay::TextAlign::ALIGN_HLEFT)
        .value("ALIGN_HMIDDLE", ccGenericGLDisplay::TextAlign::ALIGN_HMIDDLE)
        .value("ALIGN_HRIGHT", ccGenericGLDisplay::TextAlign::ALIGN_HRIGHT)
        .value("ALIGN_VTOP", ccGenericGLDisplay::TextAlign::ALIGN_VTOP)
        .value("ALIGN_VMIDDLE", ccGenericGLDisplay::TextAlign::ALIGN_VMIDDLE)
        .value("ALIGN_VBOTTOM", ccGenericGLDisplay::TextAlign::ALIGN_VBOTTOM)
        .value("ALIGN_DEFAULT", ccGenericGLDisplay::TextAlign::ALIGN_DEFAULT);

    PyccGenericGLDisplay.def("getScreenSize", &ccGenericGLDisplay::getScreenSize)
        .def("redraw", &ccGenericGLDisplay::redraw, "only2D"_a = false, "resetLOD"_a = true)
        .def("toBeRefreshed", &ccGenericGLDisplay::toBeRefreshed)
        .def("refresh", &ccGenericGLDisplay::refresh, "only2D"_a = false)
        .def("invalidateViewport", &ccGenericGLDisplay::invalidateViewport)
        .def("deprecate3DLayer", &ccGenericGLDisplay::deprecate3DLayer)
        .def("getTextDisplayFont", &ccGenericGLDisplay::getTextDisplayFont)
        .def("getLabelDisplayFont", &ccGenericGLDisplay::getLabelDisplayFont)
        .def("displayText",
             &ccGenericGLDisplay::displayText,
             "text"_a,
             "x"_a,
             "y"_a,
             "align"_a = ccGenericGLDisplay::ALIGN_DEFAULT,
             "bkgAlpha"_a = 0.0f,
             "color"_a = nullptr,
             "font"_a = nullptr)
        .def("display3DLabel",
             &ccGenericGLDisplay::display3DLabel,
             "str"_a,
             "pos3D"_a,
             "color"_a = nullptr,
             "font"_a = QFont())
        .def("getGLCameraParameters", &ccGenericGLDisplay::getGLCameraParameters, "params"_a)
        .def("toCenteredGLCoordinates", &ccGenericGLDisplay::toCenteredGLCoordinates, "x"_a, "y"_a)
        .def("getViewportParameters", &ccGenericGLDisplay::getViewportParameters)
        .def("setupProjectiveViewport",
             &ccGenericGLDisplay::setupProjectiveViewport,
             "cameraMatrix"_a,
             "fov_deg"_a = 0.0f,
             "viewerBasedPerspective"_a = true,
             "bubbleViewMode"_a = false);
    // TODO as widget
}
