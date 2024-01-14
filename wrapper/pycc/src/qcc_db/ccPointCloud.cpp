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
#include "ccAdvancedTypes.h"
#include "ccColorTypes.h"

#include <ccPointCloud.h>
#include <ccPolyline.h>
#include <ccProgressDialog.h>
#include <ccScalarField.h>
#include <climits>
#include <cstdint>
#include <pybind11/pytypes.h>
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
                                                                                     "ccPointCloud",
                                                                                     R"doc(
    The main class to represent point clouds in CloudCompare.



    Two constructor are available:

    Parameters
    ----------
    name: str,
        The name of the point cloud
    uniqueID: int
        Unique ID, you practically never need to give a value as it is
        one is already selected by default.

    Example
    -------

    .. code:: Python

        import pycc
        pc = pycc.ccPointCloud("Yoshi's Island")



    Parameters
    ----------
    x: numpy.array
    y: numpy.array
    z: numpy.array

    Example
    -------

    .. code:: Python

        import numpy as np
        import pycc

        x = np.array([1, 2, 3])
        y = np.array([1, 2, 3])
        z = np.array([1, 2, 3])
        pc = pycc.ccPointCloud(x, y, z)

)doc")
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
        // normals computation/orientation
        .def("computeNormalsWithGrids",
             &ccPointCloud::computeNormalsWithGrids,
             "minTriangleAngle_deg"_a = 1.0,
             "pDlg"_a = nullptr)
        .def("orientNormalsTowardViewPoint",
             &ccPointCloud::orientNormalsTowardViewPoint,
             "VP"_a,
             "pDlg"_a = nullptr)
        .def("computeNormalsWithOctree",
             &ccPointCloud::computeNormalsWithOctree,
             "model"_a,
             "preferredOrientation"_a,
             "defaultRadius"_a,
             "pDlg"_a = nullptr)
        .def("orientNormalsWithMST",
             &ccPointCloud::orientNormalsWithMST,
             "kNN"_a,
             "pDlg"_a = nullptr)
        .def("orientNormalsWithFM",
             &ccPointCloud::orientNormalsWithFM,
             "level"_a,
             "pDlg"_a = nullptr)
        // Special functions added by pycc
        .def("addPoints", &PyCC::addPointsFromArrays<ccPointCloud>)
        .def("setColor",
             [](ccPointCloud &self,
                ColorCompType r,
                ColorCompType g,
                ColorCompType b,
                ColorCompType a) { self.setColor(r, g, b, a); })
        .def("setPointNormal", &ccPointCloud::setPointNormal, "index"_a, "normal"_a)
        .def("addNorm", &ccPointCloud::addNorm, "normal"_a)
        .def("addNormals",
             [](ccPointCloud &self,
                const py::array_t<PointCoordinateType> &nx,
                const py::array_t<PointCoordinateType> &ny,
                const py::array_t<PointCoordinateType> &nz)
             {
                 if (nx.size() != ny.size() || nx.size() != nz.size())
                 {
                     throw py::value_error("nx, ny, nz must have the same size");
                 }

                 const py::ssize_t numToReserve = self.size() + nx.size();
                 if (numToReserve > std::numeric_limits<unsigned int>::max())
                 {
                     throw std::out_of_range(std::to_string(numToReserve) +
                                             " cannot be casted to unsigned int");
                 }
                 self.reserve(static_cast<unsigned int>(numToReserve));

                 auto xs_it = nx.begin();
                 auto ys_it = ny.begin();
                 auto zs_it = nz.begin();

                 while (xs_it != nx.end())
                 {
                     self.addNorm({xs_it->cast<PointCoordinateType>(),
                                   ys_it->cast<PointCoordinateType>(),
                                   zs_it->cast<PointCoordinateType>()});
                     ++xs_it;
                     ++ys_it;
                     ++zs_it;
                 }
             })
        .def("colorize", &ccPointCloud::colorize)
        .def("crop2D", &ccPointCloud::crop2D, "poly"_a, "orthodDim"_a, "inside"_a = true)
        .def("colors",
             [](ccPointCloud &self) -> py::object
             {
                 RGBAColorsTableType *colorsTable = self.rgbaColors();
                 if (colorsTable == nullptr)
                 {
                     return py::none();
                 }

                 ccColor::Rgba *colors = colorsTable->data();
                 if (colors == nullptr)
                 {
                     return py::none();
                 }

                 static_assert(CHAR_BIT == 8, "A char must have 8 bits");
                 static_assert(sizeof(ColorCompType) == 1, "ColorCompType must have 8 bit");
                 static_assert(sizeof(ccColor::Rgba) == 4 * sizeof(ColorCompType), "");
                 static_assert(alignof(uint8_t) == alignof(ccColor::Rgba), "Not same alignment");

                 auto *ptr = reinterpret_cast<uint8_t *>(colors);

                 auto capsule = py::capsule(ptr, [](void *) {});
                 py::array a(py::dtype("4u1"), colorsTable->size(), ptr, capsule);

                 return a;
             })
        .def("points",
             [](ccPointCloud &self)
             {
                 if (self.size() > 0)
                 {
                     // FIXME, ideally, ccPointCloud would have a .points() method returning a ref
                     // to the std::vector of points, and we would avoid the const cast
                     auto *ptr = const_cast<CCVector3 *>(self.getPoint(0));
                     auto capsule = py::capsule(ptr, [](void *) {});
                     py::array a(py::dtype("3f"), self.size(), ptr, capsule);
                     // Make the array non-writeable to make up for the const cast
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
