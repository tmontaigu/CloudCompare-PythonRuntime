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

#include <ccGLWindowInterface.h>
#include <ccHObject.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccGLWindowInterface(py::module &m)
{
    py::class_<ccGLWindowInterface, ccGenericGLDisplay> PyccGLWindow(m, "ccGLWindowInterface");
    py::enum_<ccGLWindowInterface::PICKING_MODE> PyPickingMode(PyccGLWindow, "PICKING_MODE");
    py::enum_<ccGLWindowInterface::INTERACTION_FLAG> PyInteractionFlag(
        PyccGLWindow, "INTERACTION_FLAG", py::arithmetic());
    py::enum_<ccGLWindowInterface::MessagePosition> PyMessagePosition(PyccGLWindow,
                                                                      "MessagePosition");
    py::enum_<ccGLWindowInterface::MessageType> PyMessageType(PyccGLWindow, "MessageType");
    py::enum_<ccGLWindowInterface::PivotVisibility> PyPivotVisibility(PyccGLWindow,
                                                                      "PivotVisibility");

    PyPickingMode.value("NO_PICKING", ccGLWindowInterface::PICKING_MODE::NO_PICKING)
        .value("ENTITY_PICKING", ccGLWindowInterface::PICKING_MODE::ENTITY_PICKING)
        .value("ENTITY_RECT_PICKING", ccGLWindowInterface::PICKING_MODE::ENTITY_RECT_PICKING)
        .value("FAST_PICKING", ccGLWindowInterface::PICKING_MODE::FAST_PICKING)
        .value("POINT_PICKING", ccGLWindowInterface::PICKING_MODE::POINT_PICKING)
        .value("TRIANGLE_PICKING", ccGLWindowInterface::PICKING_MODE::TRIANGLE_PICKING)
        .value("POINT_OR_TRIANGLE_PICKING",
               ccGLWindowInterface::PICKING_MODE::POINT_OR_TRIANGLE_PICKING)
        .value("POINT_OR_TRIANGLE_OR_LABEL_PICKING",
               ccGLWindowInterface::PICKING_MODE::POINT_OR_TRIANGLE_OR_LABEL_PICKING)
        .value("LABEL_PICKING", ccGLWindowInterface::PICKING_MODE::LABEL_PICKING)
        .value("DEFAULT_PICKING", ccGLWindowInterface::PICKING_MODE::DEFAULT_PICKING)
        .export_values();

    PyInteractionFlag
        .value("INTERACT_NONE", ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_NONE)
        .value("INTERACT_ROTATE",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_ROTATE)
        .value("INTERACT_PAN", ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_PAN)
        .value("INTERACT_CTRL_PAN",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_CTRL_PAN)
        .value("INTERACT_ZOOM_CAMERA",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_ZOOM_CAMERA)
        .value("INTERACT_2D_ITEMS",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_2D_ITEMS)
        .value("INTERACT_CLICKABLE_ITEMS",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_CLICKABLE_ITEMS)
        .value("INTERACT_TRANSFORM_ENTITIES",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_TRANSFORM_ENTITIES)
        .value("INTERACT_SIG_RB_CLICKED",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_SIG_RB_CLICKED)
        .value("INTERACT_SIG_LB_CLICKED",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_SIG_LB_CLICKED)
        .value("INTERACT_SIG_MOUSE_MOVED",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_SIG_MOUSE_MOVED)
        .value("INTERACT_SIG_BUTTON_RELEASED",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_SIG_BUTTON_RELEASED)
        .value("INTERACT_SIG_MB_CLICKED",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_SIG_MB_CLICKED)
        .value("INTERACT_SEND_ALL_SIGNALS",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::INTERACT_SEND_ALL_SIGNALS)
        .value("MODE_PAN_ONLY", ccGLWindowInterface::INTERACTION_FLAGS::enum_type::MODE_PAN_ONLY)
        .value("MODE_TRANSFORM_CAMERA",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::MODE_TRANSFORM_CAMERA)
        .value("MODE_TRANSFORM_ENTITIES",
               ccGLWindowInterface::INTERACTION_FLAGS::enum_type::MODE_TRANSFORM_ENTITIES)
        .export_values();

    PyMessagePosition
        .value("LOWER_LEFT_MESSAGE", ccGLWindowInterface::MessagePosition::LOWER_LEFT_MESSAGE)
        .value("UPPER_CENTER_MESSAGE", ccGLWindowInterface::MessagePosition::UPPER_CENTER_MESSAGE)
        .value("SCREEN_CENTER_MESSAGE", ccGLWindowInterface::MessagePosition::SCREEN_CENTER_MESSAGE)
        .export_values();

    PyMessageType.value("CUSTOM_MESSAGE", ccGLWindowInterface::MessageType::CUSTOM_MESSAGE)
        .value("SCREEN_SIZE_MESSAGE", ccGLWindowInterface::MessageType::SCREEN_SIZE_MESSAGE)
        .value("PERSPECTIVE_STATE_MESSAGE",
               ccGLWindowInterface::MessageType::PERSPECTIVE_STATE_MESSAGE)
        .value("SUN_LIGHT_STATE_MESSAGE", ccGLWindowInterface::MessageType::SUN_LIGHT_STATE_MESSAGE)
        .value("CUSTOM_LIGHT_STATE_MESSAGE",
               ccGLWindowInterface::MessageType::CUSTOM_LIGHT_STATE_MESSAGE)
        .value("MANUAL_TRANSFORMATION_MESSAGE",
               ccGLWindowInterface::MessageType::MANUAL_TRANSFORMATION_MESSAGE)
        .value("MANUAL_SEGMENTATION_MESSAGE",
               ccGLWindowInterface::MessageType::MANUAL_SEGMENTATION_MESSAGE)
        .value("ROTAION_LOCK_MESSAGE", ccGLWindowInterface::MessageType::ROTAION_LOCK_MESSAGE)
        .value("FULL_SCREEN_MESSAGE", ccGLWindowInterface::MessageType::FULL_SCREEN_MESSAGE)
        .export_values();

    PyPivotVisibility.value("PIVOT_HIDE", ccGLWindowInterface::PivotVisibility::PIVOT_HIDE)
        .value("PIVOT_SHOW_ON_MOVE", ccGLWindowInterface::PivotVisibility::PIVOT_SHOW_ON_MOVE)
        .value("PIVOT_ALWAYS_SHOW", ccGLWindowInterface::PivotVisibility::PIVOT_ALWAYS_SHOW)
        .export_values();

    PyccGLWindow.def("isStereo", &ccGLWindowInterface::isStereo)
        .def("getDevicePixelRatio", &ccGLWindowInterface::getDevicePixelRatio)
        .def("doResize",
             static_cast<void (ccGLWindowInterface::*)(int, int)>(&ccGLWindowInterface::doResize),
             "w"_a,
             "h"_a)
        .def("setSceneDB", &ccGLWindowInterface::setSceneDB, "root"_a)
        .def("getSceneDB", &ccGLWindowInterface::getSceneDB)
        .def("renderText",
             static_cast<void (ccGLWindowInterface::*)(
                 int, int, const QString &, uint16_t, const QFont &)>(
                 &ccGLWindowInterface::renderText),
             "x"_a,
             "y"_a,
             "str"_a,
             "uniqueID"_a = 0,
             "font"_a = QFont())
        .def("renderText",
             static_cast<void (ccGLWindowInterface::*)(
                 double, double, double, const QString &, const QFont &)>(
                 &ccGLWindowInterface::renderText),
             "x"_a,
             "y"_a,
             "z"_a,
             "str"_a,
             "font"_a = QFont())
        // TODO as widget
        .def("getScreenSize", &ccGLWindowInterface::getScreenSize)
        .def("getGLCameraParameters", &ccGLWindowInterface::getGLCameraParameters, "params"_a)
        .def("displayNewMessage",
             &ccGLWindowInterface::displayNewMessage,
             "message"_a,
             "pos"_a,
             "append"_a = false,
             "displayMaxDelay_sec"_a = 2,
             "type"_a = ccGLWindowInterface::MessageType::CUSTOM_MESSAGE)
        .def("setSunLight", &ccGLWindowInterface::setSunLight, "state"_a)
        .def("toggleSunLight", &ccGLWindowInterface::toggleSunLight)
        .def("sunLightEnabled", &ccGLWindowInterface::sunLightEnabled)
        .def("setCustomLight", &ccGLWindowInterface::setCustomLight, "state"_a)
        .def("toggleCustomLight", &ccGLWindowInterface::toggleCustomLight)
        .def("customLightEnabled", &ccGLWindowInterface::customLightEnabled)
        .def("setPivotVisibility", &ccGLWindowInterface::setPivotVisibility, "vis"_a)
        .def("getPivotVisibility", &ccGLWindowInterface::getPivotVisibility)
        .def("showPivotSymbol", &ccGLWindowInterface::showPivotSymbol, "state"_a)
        .def("setPivotPoint",
             &ccGLWindowInterface::setPivotPoint,
             "P"_a,
             "autoUpdateCameraPos"_a = false,
             "verbose"_a = false)
        .def("setCameraPos", &ccGLWindowInterface::setCameraPos, "P"_a)
        .def("moveCamera", &ccGLWindowInterface::moveCamera, "v"_a)
        .def("setCameraFocalToFitWidth", &ccGLWindowInterface::setCameraFocalToFitWidth, "width"_a)
        .def("setFocalDistance", &ccGLWindowInterface::setFocalDistance, "focalDistance"_a)
        .def("setPerspectiveState",
             &ccGLWindowInterface::setPerspectiveState,
             "state"_a,
             "objectCenteredView"_a)
        .def("togglePerspective", &ccGLWindowInterface::togglePerspective, "objectCentered"_a)
        .def("getPerspectiveState", &ccGLWindowInterface::getPerspectiveState, "objectCentered"_a)
        .def("objectPerspectiveEnabled", &ccGLWindowInterface::objectPerspectiveEnabled)
        .def("viewerPerspectiveEnabled", &ccGLWindowInterface::viewerPerspectiveEnabled)
        .def("setBubbleViewMode", &ccGLWindowInterface::setBubbleViewMode, "state"_a)
        .def("bubbleViewModeEnabled", &ccGLWindowInterface::bubbleViewModeEnabled)
        .def("setBubbleViewFov", &ccGLWindowInterface::setBubbleViewFov, "fov_deg"_a)
        .def("updateConstellationCenterAndZoom",
             &ccGLWindowInterface::updateConstellationCenterAndZoom,
             "boundingBox"_a = nullptr)
        .def("getVisibleObjectsBB", &ccGLWindowInterface::getVisibleObjectsBB, "box"_a)
        .def("rotateBaseViewMat", &ccGLWindowInterface::rotateBaseViewMat, "rotMat"_a)
        .def("getBaseViewMat", &ccGLWindowInterface::getBaseViewMat)
        .def("setBaseViewMat", &ccGLWindowInterface::setBaseViewMat, "mat"_a)
        .def("setView", &ccGLWindowInterface::setView, "orientation"_a, "redraw"_a = true)
        .def("setCustomView",
             &ccGLWindowInterface::setCustomView,
             "forward"_a,
             "up"_a,
             "forceRedraw"_a = true)
        .def("setInteractionMode", &ccGLWindowInterface::setInteractionMode, "flags"_a)
        .def("getInteractionMode", &ccGLWindowInterface::getInteractionMode)
        .def("setPickingMode",
             &ccGLWindowInterface::setPickingMode,
             "mode"_a = ccGLWindowInterface::PICKING_MODE::DEFAULT_PICKING,
             "defaultCursorShape"_a = Qt::ArrowCursor)
        .def("getPickingMode", &ccGLWindowInterface::getPickingMode)
        .def("lockPickingMode", &ccGLWindowInterface::lockPickingMode, "state"_a)
        .def("isPickingModeLocked", &ccGLWindowInterface::isPickingModeLocked)
        .def("setUnclosable", &ccGLWindowInterface::setUnclosable, "state"_a)
        .def("getContext", &ccGLWindowInterface::getContext, "context"_a)
        // TODO static constexprs
        .def("setPointSize", &ccGLWindowInterface::setPointSize, "size"_a, "silent"_a = false)
        .def("setLineWidth", &ccGLWindowInterface::setPointSize, "width"_a, "silent"_a = false)
        .def("getFontPointSize", &ccGLWindowInterface::getFontPointSize)
        .def("getLabelFontPointSize", &ccGLWindowInterface::getLabelFontPointSize)
        .def("getOwnDB", &ccGLWindowInterface::getOwnDB)
        .def("addToOwnDB", &ccGLWindowInterface::addToOwnDB, "obj"_a, "noDependency"_a = false)
        .def("removeFromOwnDB", &ccGLWindowInterface::removeFromOwnDB, "obj"_a)
        .def("setViewportParameters", &ccGLWindowInterface::setViewportParameters, "parameters"_a)
        .def("setFov", &ccGLWindowInterface::setFov, "fov"_a)
        .def("getFov", &ccGLWindowInterface::getFov)
        .def("setFarClippingPlaneDepth", &ccGLWindowInterface::setFarClippingPlaneDepth, "depth"_a)
        .def(
            "setClippingPlanesEnabled", &ccGLWindowInterface::setClippingPlanesEnabled, "enabled"_a)
        .def("clippingPlanesEnabled", &ccGLWindowInterface::clippingPlanesEnabled)
        .def("invalidateVisualization", &ccGLWindowInterface::invalidateVisualization)
        // TODO renderToImage
        .def("renderToFile",
             &ccGLWindowInterface::renderToFile,
             "filename"_a,
             "zoomFactor"_a = 1.0f,
             "dontScaleFeatures"_a = false,
             "renderOverlayItems"_a = false)
        .def_static("SetShaderPath", &ccGLWindowInterface::SetShaderPath, "path"_a)
        .def_static("GetShaderPath", &ccGLWindowInterface::GetShaderPath)
        //        .def("setShader", &ccGLWindowInterface::setShader, "shader"_a)
        //        .def("setGlFilter", &ccGLWindowInterface::setGlFilter, "shader"_a)
        //        .def("getGlFilter", &ccGLWindowInterface::getGlFilter)
        .def("areShadersEnabled", &ccGLWindowInterface::areShadersEnabled)
        .def("areGLFiltersEnabled", &ccGLWindowInterface::areGLFiltersEnabled)
        .def("computeActualPixelSize", &ccGLWindowInterface::computeActualPixelSize)
        .def("hasColorRampShader", &ccGLWindowInterface::hasColorRampShader)
        .def("isRectangularPickingAllowed", &ccGLWindowInterface::isRectangularPickingAllowed)
        .def("setRectangularPickingAllowed",
             &ccGLWindowInterface::setRectangularPickingAllowed,
             "state"_a)
        // TODO: this would need a ccGui::ParamStruct binding
        //        .def("setShader", &ccGLWindowInterface::getDisplayParameters)
        //        .def("setDisplayParameter", &ccGLWindowInterface::getDisplayParameters,
        //        "params"_a, thisWindowOnly)
        .def("hasOverriddenDisplayParameters", &ccGLWindowInterface::hasOverriddenDisplayParameters)
        .def("setPickingRadius", &ccGLWindowInterface::setPickingRadius, "radius"_a)
        .def("getPickingRadius", &ccGLWindowInterface::getPickingRadius)
        .def("displayOverlayEntities",
             &ccGLWindowInterface::displayOverlayEntities,
             "showScale"_a,
             "showTrihedron"_a)

        // TODO
        ;
}
