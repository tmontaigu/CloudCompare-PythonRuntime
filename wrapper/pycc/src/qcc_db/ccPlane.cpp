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

#include "../casters.h"

#include <GenericIndexedCloudPersist.h>
#include <ccPlane.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccPlane(py::module &m)
{

    py::class_<ccPlanarEntityInterface>(m, "ccPlanarEntityInterface")
        .def("showNormalVector", &ccPlanarEntityInterface::showNormalVector, "state"_a)
        .def("normalVectorIsShown", &ccPlanarEntityInterface::normalVectorIsShown)
        .def("getNormal", &ccPlanarEntityInterface::getNormal);

    py::class_<ccPlane, ccGenericPrimitive, ccPlanarEntityInterface>(m,
                                                                     "ccPlane",
                                                                     R"doc(
    ccPlane

    Parameters
    ----------
    xWidth : PointCoordinateType
    yWidth : PointCoordinateType
    transMat : , optional
        optional 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
    name : str, default: Sphere
        name of the sphere object
    uniqueID : int, optional
        unique ID (handle with care)

    Example
    -------
    >>> import pycc
    >>> plane = pycc.ccPlane(10.0, 5.0)
    >>> plane.getXWidth()
    10.0
    >>> plane.getYWidth()
    5.0
    >>> plane.setXWidth(15.0)
    >>> plane.setYWidth(12.5)
    >>> plane.getXWidth()
    15.0
    >>> plane.getYWidth()
    12.5
    )doc")
        .def(py::init<PointCoordinateType, PointCoordinateType, const ccGLMatrix *, QString>(),
             "xWidth"_a,
             "yWidth"_a,
             "transMat"_a = nullptr,
             "name"_a = QString("Plane"))
        .def("getXWidth", &ccPlane::getXWidth)
        .def("getYWidth", &ccPlane::getYWidth)
        .def("getCenter", &ccPlane::getCenter)
        .def("setXWidth", &ccPlane::setXWidth, "w"_a, "autoUpdate"_a = true)
        .def("setYWidth", &ccPlane::setYWidth, "h"_a, "autoUpdate"_a = true)
        // TODO setAsTexture, will need QImage
        // TODO setQuadTexture, also needs QImage
        .def_static("Fit", &ccPlane::Fit, "cloud"_a, "rms"_a)
        .def("getEquation",
             [](const ccPlane &self)
             {
                 CCVector3 N;
                 PointCoordinateType constVal;

                 self.getEquation(N, constVal);
                 return py::make_tuple(N, constVal);
             })
        .def("flip", &ccPlane::flip);
}
