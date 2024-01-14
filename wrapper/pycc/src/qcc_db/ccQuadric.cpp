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

#include <ccQuadric.h>

#include <utility>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccQuadric(py::module &m)
{
    py::class_<ccQuadric, ccGenericPrimitive>(m, "ccQuadric", R"doc(
    ccQuadric

    Parameters
    ----------
    minCorner : cccorelib.CCVector2
        min corner of the 'representation' base area
    maxCorner : cccorelib.CCVector2
        max corner of the 'representation' base area
    eq: list of PointCoordinateType
        equation coefficients ( Z = a + b.X + c.Y + d.X^2 + e.X.Y + f.Y^2)
        6 coefficients
    dims: list of int, optional
        optional dimension indexes
    transMat : , optional
        optional 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
    name : str, default: Sphere
        name of the sphere object

    Example
    -------

    .. code:: Python

        quadric = pycc.ccQuadric(
            cccorelib.CCVector2(5.0, 10.0),
            cccorelib.CCVector2(10.0, 20.0),
            [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
        )
    )doc")
        .def(
            py::init(
                [](CCVector2 minCorner,
                   CCVector2 maxCorner,
                   const py::sequence eq,
                   const Tuple3ub *dims = nullptr,
                   const ccGLMatrix *transMat = nullptr,
                   QString name = QString("Quadric"),
                   unsigned precision = ccQuadric::DEFAULT_DRAWING_PRECISION)
                {
                    PointCoordinateType eqC[6];
                    if (eq.size() != 6)
                    {
                        throw py::value_error("eq must have 6 elements");
                    }

                    for (size_t i{0}; i < 6; i++)
                    {
                        eqC[i] = eq[i].cast<PointCoordinateType>();
                    }
                    return new ccQuadric(
                        minCorner, maxCorner, eqC, dims, transMat, std::move(name), precision);
                }),
            "minCorner"_a,
            "maxCorner"_a,
            "eq"_a,
            "dims"_a = nullptr,
            "transMat"_a = nullptr,

            "name"_a = QString("Quadric"),
            "precision"_a = []() { return ccQuadric::DEFAULT_DRAWING_PRECISION; }())
        .def_property_readonly_static("DEFAULT_DRAWING_PRECISION",
                                      []() { return ccQuadric::DEFAULT_DRAWING_PRECISION; })
        .def("getMinCorner", &ccQuadric::getMinCorner)
        .def("getMaxCorner", &ccQuadric::getMaxCorner)
        .def("getEquationCoefs", &ccQuadric::getEquationCoefs)
        .def("getEquationDims", &ccQuadric::getEquationDims)
        .def("projectOnQuadric", &ccQuadric::projectOnQuadric, "P"_a, "Q"_a)
        .def("getEquationString", &ccQuadric::getEquationString)
        .def_static("Fit",
                    [](CCCoreLib::GenericIndexedCloudPersist *cloud)
                    {
                        double rms;
                        ccQuadric::Fit(cloud, &rms);
                        return rms;
                    });
}
