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

#include <ccColorScale.h>

#include <QColor>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccColorScale(py::module &m)
{
    py::class_<ccColorScaleElement>(m, "ccColorScaleElement")
        .def(py::init<>())
        .def(py::init<double, const QColor &>(), "relativePos"_a, "color"_a)
        .def("setRelativePos", &ccColorScaleElement::setRelativePos, "pos"_a)
        .def("getRelativePos", &ccColorScaleElement::getRelativePos)
        .def("setColor", &ccColorScaleElement::setColor, "color"_a)
        .def("getColor", &ccColorScaleElement::getColor)
        .def_static("IsSmaller", &ccColorScaleElement::IsSmaller, "e1"_a, "e2"_a);

    py::class_<ccColorScale> pyColorScale(m, "ccColorScale");
    pyColorScale.def(py::init<const QString &, const QString &>(), "name"_a, "uuid"_a = QString())
        .def_property_readonly_static("MIN_STEPS", [] { return ccColorScale::MIN_STEPS; })
        .def_property_readonly_static("DEFAULT_STEPS", [] { return ccColorScale::DEFAULT_STEPS; })
        .def_property_readonly_static("MAX_STEPS", [] { return ccColorScale::MAX_STEPS; })
        .def("getName", &ccColorScale::getName)
        .def("setName", &ccColorScale::setName, "name"_a)
        .def("getUuid", &ccColorScale::getUuid)
        .def("setUuid", &ccColorScale::setUuid, "uuid"_a)
        .def("generateNewUuid", &ccColorScale::generateNewUuid)
        .def("isRelative", &ccColorScale::isRelative)
        .def("setRelative", &ccColorScale::setRelative)
        .def("setAbsolute", &ccColorScale::setAbsolute, "minVal"_a, "maxVal"_a)
        .def("setAbsolute", &ccColorScale::setAbsolute, "minVal"_a, "maxVal"_a)
        .def("getAbsoluteBoundaries",
             [](const ccColorScale &self)
             {
                 double minVal, maxVal;
                 self.getAbsoluteBoundaries(minVal, maxVal);
                 return py::make_tuple(minVal, maxVal);
             })
        .def("isLocked", &ccColorScale::isLocked)
        .def("setLocked", &ccColorScale::setLocked, "state"_a)
        .def("setCustomLabels", &ccColorScale::setCustomLabels, "labels"_a)
        .def("stepCount", &ccColorScale::stepCount)
        .def("step",
             static_cast<ccColorScaleElement &(ccColorScale::*)(int)>(&ccColorScale::step),
             "index"_a)
        .def("insert", &ccColorScale::insert, "step"_a, "autoUpdate"_a = true)
        .def("remove", &ccColorScale::remove, "index"_a, "autoUpdate"_a = true)
        .def("clear", &ccColorScale::clear)
        .def("update", &ccColorScale::update)
        .def("getRelativePosition", &ccColorScale::getRelativePosition, "value"_a)
        .def("getColorByValue",
             &ccColorScale::getColorByValue,
             "value"_a,
             "outOfRangeColor"_a = nullptr)
        .def("getColorByRelativePos",
             static_cast<const ccColor::Rgb *(ccColorScale::*)(double, const ccColor::Rgb *) const>(
                 &ccColorScale::getColorByRelativePos),
             "relativePos"_a,
             "outOfRangeColor"_a = nullptr)
        .def("getColorByRelativePos",
             static_cast<const ccColor::Rgb *(
                 ccColorScale::*)(double, unsigned, const ccColor::Rgb *) const>(
                 &ccColorScale::getColorByRelativePos),
             "relativePos"_a,
             "steps"_a,
             "outOfRangeColor"_a = nullptr)
        .def("getColorByIndex", &ccColorScale::getColorByIndex, "index"_a)
        .def_static("LoadFromXML", &ccColorScale::LoadFromXML, "filename"_a)
        .def("saveAsXML", &ccColorScale::saveAsXML, "filename"_a);

    py::class_<ccColorScale::Label>(pyColorScale, "Label")
        .def_readwrite("value", &ccColorScale::Label::value)
        .def_readwrite("text", &ccColorScale::Label::text, py::return_value_policy::reference)
        .def(py::init<double>(), "value"_a)
        .def(py::init<double, const QString &>(), "value"_a, "text"_a);

    pyColorScale
        .def("customLabels",
             static_cast<const std::set<ccColorScale::Label> &(ccColorScale::*)() const>(
                 &ccColorScale::customLabels))
        .def("customLabels",
             static_cast<std::set<ccColorScale::Label> &(ccColorScale::*)()>(
                 &ccColorScale::customLabels));

    // Thanks https://github.com/pybind/pybind11/issues/820#issuecomment-297894565
    py::class_<ccColorScale::Shared>(pyColorScale, "Shared");
}
