// ##########################################################################
// #                                                                        #
// #                CLOUDCOMPARE PLUGIN: PythonPlugin                       #
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

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccQuadric(py::module &m)
{
    py::class_<ccQuadric, ccGenericPrimitive>(m, "Quadric")
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
                        minCorner, maxCorner, eqC, dims, transMat, name, precision);
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
