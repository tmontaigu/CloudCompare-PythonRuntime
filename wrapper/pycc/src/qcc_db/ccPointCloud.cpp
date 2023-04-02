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

#include "../casters.h"

#include <ccPointCloud.h>
#include <ccPolyline.h>
#include <ccScalarField.h>
#include <type_traits>
#include <vector>

#undef slots
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include "wrappers.h"

static_assert(std::is_same<PointCoordinateType, float>::value,
              "PointCoordinateType is neither double or float");
static_assert(sizeof(CCVector3) == sizeof(PointCoordinateType) * 3,
              "Unexpected layout for CCVector3");

void define_ccPointCloud(py::module &m)
{
    DEFINE_POINTCLOUDTPL(ccGenericPointCloud, QString, m, "__ccGenericPointCloudTpl");
    py::class_<ccPointCloud, CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>>(m,
                                                                                     "ccPointCloud")
        .def(
            py::init<QString, unsigned>(),
            "name"_a = QString(),
            "uniqueID"_a = []() { return ccUniqueIDGenerator::InvalidUniqueID; }())
        .def(py::init(
            [](py::array_t<PointCoordinateType> &xs,
               py::array_t<PointCoordinateType> &ys,
               py::array_t<PointCoordinateType> &zs)
            {
                auto pointCloud = new ccPointCloud;
                try
                {
                    PyCC::addPointsFromArrays(*pointCloud, xs, ys, zs);
                }
                catch (const std::exception &)
                {
                    delete pointCloud;
                    throw;
                }

                return pointCloud;
            }))

        // features deletion/clearing
        .def(
            "partialClone",
            [](const ccPointCloud &self, const CCCoreLib::ReferenceCloud *refCloud)
            { return self.partialClone(refCloud); },
            "reference"_a)
        // features allocation/resize
        .def("reserveThePointsTable", &ccPointCloud::reserveThePointsTable, "_numberOfPoints"_a)
        .def("reserveTheRGBTable", &ccPointCloud::reserveThePointsTable)
        .def("reserveTheRGBTable", &ccPointCloud::reserveThePointsTable, "fillWithWhite"_a = false)
        .def("reserveTheNormsTable", &ccPointCloud::reserveTheNormsTable)
        .def("resizeTheNormsTable", &ccPointCloud::resizeTheNormsTable)
        .def("shrinkToFit", &ccPointCloud::shrinkToFit)
        // ScalarField management
        .def("getCurrentDisplayedScalarField", &ccPointCloud::getCurrentDisplayedScalarField)
        .def("getCurrentDisplayedScalarFieldIndex",
             &ccPointCloud::getCurrentDisplayedScalarFieldIndex)
        .def("setCurrentDisplayedScalarField",
             &ccPointCloud::setCurrentDisplayedScalarField,
             "index"_a)
        .def("setPointSize", &ccPointCloud::setPointSize, "size"_a)
        .def("sfColorScaleShown", &ccPointCloud::sfColorScaleShown)
        .def("showSFColorsScale", &ccPointCloud::showSFColorsScale, "state"_a)
        .def("addPoints", &PyCC::addPointsFromArrays<ccPointCloud>)
        .def("setColor",
             [](ccPointCloud &self,
                ColorCompType r,
                ColorCompType g,
                ColorCompType b,
                ColorCompType a) { self.setColor(r, g, b, a); })
        .def("colorize", &ccPointCloud::colorize)
        .def("crop2D", &ccPointCloud::crop2D, "poly"_a, "orthodDim"_a, "inside"_a = true)
        .def("points",
             [](ccPointCloud &self)
             {
                 if (self.size() > 0)
                 {
                     // FIXME, idealy, ccPointCloud would have a .points() method returning a ref
                     // to the std::vector of points, and we would avoid the const cast
                     auto *ptr = const_cast<CCVector3 *>(self.getPoint(0));
                     auto capsule = py::capsule(ptr, [](void *) {});
                     py::array a(py::dtype("3f"), self.size(), ptr, capsule);
                     // Make the array non-writreable to make up for the const cast
                     a.attr("flags").attr("writeable") = false;
                     return a;
                 }
                 else
                 {
                     return py::array(py::dtype("3f"), 0);
                 }
             })
        .def("__repr__",
             [](const ccPointCloud &self)
             {
                 return std::string("<ccPointCloud(") + "'" + self.getName().toStdString() + "', " +
                        std::to_string(self.size()) + " points)>";
             });
}
