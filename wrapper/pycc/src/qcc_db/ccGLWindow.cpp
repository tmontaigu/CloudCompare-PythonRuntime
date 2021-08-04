//##########################################################################
//#                                                                        #
//#                CLOUDCOMPARE PLUGIN: PythonPlugin                       #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                   COPYRIGHT: Thomas Montaigu                           #
//#                                                                        #
//##########################################################################

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <ccGLWindow.h>
#include <ccHObject.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccGLWindow(py::module &m)
{
    py::class_<ccGLWindow, QOpenGLWidget, ccGenericGLDisplay> PyccGLWindow(m, "ccGLWindow");
    py::enum_<ccGLWindow::PICKING_MODE> PyPickingMode(PyccGLWindow, "PICKING_MODE");
    py::enum_<ccGLWindow::INTERACTION_FLAG> PyInteractionFlag(
        PyccGLWindow, "INTERACTION_FLAG", py::arithmetic());
    py::enum_<ccGLWindow::MessagePosition> PyMessagePosition(PyccGLWindow, "MessagePosition");
    py::enum_<ccGLWindow::MessageType> PyMessageType(PyccGLWindow, "MessageType");
    py::enum_<ccGLWindow::PivotVisibility> PyPivotVisibility(PyccGLWindow, "PivotVisibility");

    PyPickingMode.value("NO_PICKING", ccGLWindow::PICKING_MODE::NO_PICKING)
        .value("ENTITY_PICKING", ccGLWindow::PICKING_MODE::ENTITY_PICKING)
        .value("ENTITY_RECT_PICKING", ccGLWindow::PICKING_MODE::ENTITY_RECT_PICKING)
        .value("FAST_PICKING", ccGLWindow::PICKING_MODE::FAST_PICKING)
        .value("POINT_PICKING", ccGLWindow::PICKING_MODE::POINT_PICKING)
        .value("TRIANGLE_PICKING", ccGLWindow::PICKING_MODE::TRIANGLE_PICKING)
        .value("POINT_OR_TRIANGLE_PICKING", ccGLWindow::PICKING_MODE::POINT_OR_TRIANGLE_PICKING)
        .value("LABEL_PICKING", ccGLWindow::PICKING_MODE::LABEL_PICKING)
        .value("DEFAULT_PICKING", ccGLWindow::PICKING_MODE::DEFAULT_PICKING)
        .export_values();

    PyInteractionFlag
        .value("INTERACT_NONE", ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_NONE)
        .value("INTERACT_ROTATE", ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_ROTATE)
        .value("INTERACT_PAN", ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_PAN)
        .value("INTERACT_CTRL_PAN", ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_CTRL_PAN)
        .value("INTERACT_ZOOM_CAMERA",
               ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_ZOOM_CAMERA)
        .value("INTERACT_2D_ITEMS", ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_2D_ITEMS)
        .value("INTERACT_CLICKABLE_ITEMS",
               ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_CLICKABLE_ITEMS)
        .value("INTERACT_TRANSFORM_ENTITIES",
               ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_TRANSFORM_ENTITIES)
        .value("INTERACT_SIG_RB_CLICKED",
               ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_SIG_RB_CLICKED)
        .value("INTERACT_SIG_LB_CLICKED",
               ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_SIG_LB_CLICKED)
        .value("INTERACT_SIG_MOUSE_MOVED",
               ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_SIG_MOUSE_MOVED)
        .value("INTERACT_SIG_BUTTON_RELEASED",
               ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_SIG_BUTTON_RELEASED)
        .value("INTERACT_SIG_MB_CLICKED",
               ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_SIG_MB_CLICKED)
        .value("INTERACT_SEND_ALL_SIGNALS",
               ccGLWindow::INTERACTION_FLAGS::enum_type::INTERACT_SEND_ALL_SIGNALS)
        .value("MODE_PAN_ONLY", ccGLWindow::INTERACTION_FLAGS::enum_type::MODE_PAN_ONLY)
        .value("MODE_TRANSFORM_CAMERA",
               ccGLWindow::INTERACTION_FLAGS::enum_type::MODE_TRANSFORM_CAMERA)
        .value("MODE_TRANSFORM_ENTITIES",
               ccGLWindow::INTERACTION_FLAGS::enum_type::MODE_TRANSFORM_ENTITIES)
        .export_values();

    PyMessagePosition.value("LOWER_LEFT_MESSAGE", ccGLWindow::MessagePosition::LOWER_LEFT_MESSAGE)
        .value("UPPER_CENTER_MESSAGE", ccGLWindow::MessagePosition::UPPER_CENTER_MESSAGE)
        .value("SCREEN_CENTER_MESSAGE", ccGLWindow::MessagePosition::SCREEN_CENTER_MESSAGE)
        .export_values();

    PyMessageType.value("CUSTOM_MESSAGE", ccGLWindow::MessageType::CUSTOM_MESSAGE)
        .value("SCREEN_SIZE_MESSAGE", ccGLWindow::MessageType::SCREEN_SIZE_MESSAGE)
        .value("PERSPECTIVE_STATE_MESSAGE", ccGLWindow::MessageType::PERSPECTIVE_STATE_MESSAGE)
        .value("SUN_LIGHT_STATE_MESSAGE", ccGLWindow::MessageType::SUN_LIGHT_STATE_MESSAGE)
        .value("CUSTOM_LIGHT_STATE_MESSAGE", ccGLWindow::MessageType::CUSTOM_LIGHT_STATE_MESSAGE)
        .value("MANUAL_TRANSFORMATION_MESSAGE",
               ccGLWindow::MessageType::MANUAL_TRANSFORMATION_MESSAGE)
        .value("MANUAL_SEGMENTATION_MESSAGE", ccGLWindow::MessageType::MANUAL_SEGMENTATION_MESSAGE)
        .value("ROTAION_LOCK_MESSAGE", ccGLWindow::MessageType::ROTAION_LOCK_MESSAGE)
        .value("FULL_SCREEN_MESSAGE", ccGLWindow::MessageType::FULL_SCREEN_MESSAGE)
        .export_values();

    PyPivotVisibility.value("PIVOT_HIDE", ccGLWindow::PivotVisibility::PIVOT_HIDE)
        .value("PIVOT_SHOW_ON_MOVE", ccGLWindow::PivotVisibility::PIVOT_SHOW_ON_MOVE)
        .value("PIVOT_ALWAYS_SHOW", ccGLWindow::PivotVisibility::PIVOT_ALWAYS_SHOW)
        .export_values();

    PyccGLWindow
        .def(py::init<QSurfaceFormat *, ccGLWindowParent *, bool>(),
             "format"_a = nullptr,
             "parent"_a = nullptr,
             "silentInitialization"_a = false)
        .def("setSceneDB", &ccGLWindow::setSceneDB, "root"_a)
        .def("getSceneDB", &ccGLWindow::getSceneDB)
        .def("renderText",
             static_cast<void (ccGLWindow::*)(int, int, const QString &, uint16_t, const QFont &)>(
                 &ccGLWindow::renderText),
             "x"_a,
             "y"_a,
             "str"_a,
             "uniqueID"_a = 0,
             "font"_a = QFont())
        .def("renderText",
             static_cast<void (ccGLWindow::*)(
                 double, double, double, const QString &, const QFont &)>(&ccGLWindow::renderText),
             "x"_a,
             "y"_a,
             "z"_a,
             "str"_a,
             "font"_a = QFont())
        // TODO as widget
        .def("getScreenSize", &ccGLWindow::getScreenSize)
        .def("getGLCameraParameters", &ccGLWindow::getGLCameraParameters, "params"_a)
        .def("displayNewMessage",
             &ccGLWindow::displayNewMessage,
             "message"_a,
             "pos"_a,
             "append"_a = false,
             "displayMaxDelay_sec"_a = 2,
             "type"_a = ccGLWindow::MessageType::CUSTOM_MESSAGE)
        .def("setSunLight", &ccGLWindow::setSunLight, "state"_a)
        .def("toggleSunLight", &ccGLWindow::toggleSunLight)
        .def("sunLightEnabled", &ccGLWindow::sunLightEnabled)
        .def("setCustomLight", &ccGLWindow::setCustomLight, "state"_a)
        .def("toggleCustomLight", &ccGLWindow::toggleCustomLight)
        .def("customLightEnabled", &ccGLWindow::customLightEnabled)
        .def("setPivotVisibility", &ccGLWindow::setPivotVisibility, "vis"_a)
        .def("getPivotVisibility", &ccGLWindow::getPivotVisibility)
        .def("showPivotSymbol", &ccGLWindow::showPivotSymbol, "state"_a)
        .def("setPivotPoint",
             &ccGLWindow::setPivotPoint,
             "P"_a,
             "autoUpdateCameraPos"_a = false,
             "verbose"_a = false)
        .def("setCameraPos", &ccGLWindow::setCameraPos, "P"_a)
        .def("moveCamera", &ccGLWindow::moveCamera, "v"_a)
        .def("setCameraFocalToFitWidth", &ccGLWindow::setCameraFocalToFitWidth, "width"_a)
        .def("setFocalDistance", &ccGLWindow::setFocalDistance, "focalDistance"_a)
        .def("setPerspectiveState",
             &ccGLWindow::setPerspectiveState,
             "state"_a,
             "objectCenteredView"_a)
        .def("togglePerspective", &ccGLWindow::togglePerspective, "objectCentered"_a)
        .def("getPerspectiveState", &ccGLWindow::getPerspectiveState, "objectCentered"_a)
        .def("objectPerspectiveEnabled", &ccGLWindow::objectPerspectiveEnabled)
        .def("viewerPerspectiveEnabled", &ccGLWindow::viewerPerspectiveEnabled)
        .def("setBubbleViewMode", &ccGLWindow::setBubbleViewMode, "state"_a)
        .def("bubbleViewModeEnabled", &ccGLWindow::bubbleViewModeEnabled)
        .def("setBubbleViewFov", &ccGLWindow::setBubbleViewFov, "fov_deg"_a)
        .def("updateConstellationCenterAndZoom",
             &ccGLWindow::updateConstellationCenterAndZoom,
             "boundingBox"_a = nullptr)
        .def("getVisibleObjectsBB", &ccGLWindow::getVisibleObjectsBB, "box"_a)
        .def("rotateBaseViewMat", &ccGLWindow::rotateBaseViewMat, "rotMat"_a)
        .def("getBaseViewMat", &ccGLWindow::getBaseViewMat)
        .def("setBaseViewMat", &ccGLWindow::setBaseViewMat, "mat"_a)
        .def("setView", &ccGLWindow::setView, "orientation"_a, "redraw"_a = true)
        .def("setCustomView",
             &ccGLWindow::setCustomView,
             "forward"_a,
             "up"_a,
             "forceRedraw"_a = true)
        .def("setInteractionMode", &ccGLWindow::setInteractionMode, "flags"_a)
        .def("getInteractionMode", &ccGLWindow::getInteractionMode)
        .def("setPickingMode",
             &ccGLWindow::setPickingMode,
             "mode"_a = ccGLWindow::PICKING_MODE::DEFAULT_PICKING)
        .def("getPickingMode", &ccGLWindow::getPickingMode)
        .def("lockPickingMode", &ccGLWindow::lockPickingMode, "state"_a)
        .def("isPickingModeLocked", &ccGLWindow::isPickingModeLocked)
        .def("setUnclosable", &ccGLWindow::setUnclosable, "state"_a)
        .def("getContext", &ccGLWindow::getContext, "context"_a)
        // TODO static constexprs
        .def("setPointSize", &ccGLWindow::setPointSize, "size"_a, "silent"_a = false)
        .def("setLineWidth", &ccGLWindow::setPointSize, "width"_a, "silent"_a = false)
        .def("getFontPointSize", &ccGLWindow::getFontPointSize)
        .def("getLabelFontPointSize", &ccGLWindow::getLabelFontPointSize)
        .def("getOwnDB", &ccGLWindow::getOwnDB)
        .def("addToOwnDB", &ccGLWindow::addToOwnDB, "obj"_a, "noDependency"_a = false)
        .def("removeFromOwnDB", &ccGLWindow::removeFromOwnDB, "obj"_a)
        .def("setViewportParameters", &ccGLWindow::setViewportParameters, "parameters"_a)
        .def("setFov", &ccGLWindow::setFov, "fov"_a)
        .def("getFov", &ccGLWindow::getFov)
        .def("setGLCameraAspectRatio", &ccGLWindow::setGLCameraAspectRatio, "ar"_a)
        .def("setZNearCoef", &ccGLWindow::setZNearCoef, "coef"_a)
        .def("invalidateVisualization", &ccGLWindow::invalidateVisualization)
        // TODO renderToImage
        .def("renderToFile",
             &ccGLWindow::renderToFile,
             "filename"_a,
             "zoomFactor"_a = 1.0f,
             "dontScaleFeatures"_a = false,
             "renderOverlayItems"_a = false)
        .def_static("setShaderPath", &ccGLWindow::setShaderPath, "path"_a)
        //        .def("setShader", &ccGLWindow::setShader, "shader"_a)
        //        .def("setGlFilter", &ccGLWindow::setGlFilter, "shader"_a)
        //        .def("getGlFilter", &ccGLWindow::getGlFilter)
        .def("areShadersEnabled", &ccGLWindow::areShadersEnabled)
        .def("areGLFiltersEnabled", &ccGLWindow::areGLFiltersEnabled)
        .def("computeActualPixelSize", &ccGLWindow::computeActualPixelSize)
        .def("hasColorRampShader", &ccGLWindow::hasColorRampShader)
        .def("isRectangularPickingAllowed", &ccGLWindow::isRectangularPickingAllowed)
        .def("setRectangularPickingAllowed", &ccGLWindow::setRectangularPickingAllowed, "state"_a)
        // TODO unfinished
        ;
}