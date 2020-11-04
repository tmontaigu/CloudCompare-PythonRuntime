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

#include <ccDrawableObject.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccDrawableObject(py::module &m)
{
    py::class_<ccDrawableObject>(m, "ccDrawableObject")
        .def("isVisible", &ccDrawableObject::isVisible)
        .def("setVisible", &ccDrawableObject::setVisible)
        .def("toggleVisibility", &ccDrawableObject::toggleVisibility)
        .def("isVisibilityLocked", &ccDrawableObject::isVisiblityLocked)
        .def("lockVisibility", &ccDrawableObject::lockVisibility)
        .def("isSelected", &ccDrawableObject::isSelected)
        .def("setSelected", &ccDrawableObject::setSelected)
        .def("hasColors", &ccDrawableObject::hasColors)
        .def("colorsShown", &ccDrawableObject::colorsShown)
        .def("showColors", &ccDrawableObject::showColors)
        .def("toggleColors", &ccDrawableObject::toggleColors)
        .def("hasNormals", &ccDrawableObject::hasNormals)
        .def("normalsShown", &ccDrawableObject::normalsShown)
        .def("showNormals", &ccDrawableObject::showNormals)
        .def("toggleNormals", &ccDrawableObject::toggleNormals)
        .def("hasDisplayedScalarField", &ccDrawableObject::hasDisplayedScalarField)
        .def("hasScalarFields", &ccDrawableObject::hasScalarFields)
        .def("showSF", &ccDrawableObject::showSF)
        .def("toggleSF", &ccDrawableObject::toggleSF)
        .def("sfShown", &ccDrawableObject::sfShown)
        .def("showNameIn3D", &ccDrawableObject::showNameIn3D)
        .def("nameShownIn3D", &ccDrawableObject::nameShownIn3D)
        .def("toggleShowName", &ccDrawableObject::toggleShowName);
}
