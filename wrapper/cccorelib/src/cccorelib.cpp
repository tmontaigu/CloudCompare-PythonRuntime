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

#include <limits>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <AutoSegmentationTools.h>
#include <BoundingBox.h>
#include <CCGeom.h>
#include <ChamferDistanceTransform.h>
#include <GenericProgressCallback.h>
#include <GeometricalAnalysisTools.h>
#include <Grid3D.h>
#include <PointCloud.h>
#include <ReferenceCloud.h>
#include <TrueKdTree.h>

namespace py = pybind11;
using namespace pybind11::literals;

#include "cccorelib.h"

void define_AutoSegmentationTools(py::module &);

void define_BoundingBox(py::module &);

void define_CCGeom(py::module &);

void define_CCMath(py::module &);

void define_CCConst(py::module &);

void define_CCShareable(py::module &);

void define_ChamferDistanceTransform(py::module &);

void define_CloudSamplingTools(py::module &);

void define_DgmOctree(py::module &);

void define_DgmOctreeReferenceCloud(py::module &);

void define_GenericCloud(py::module &);

void define_TrueKdTree(py::module &);

void define_GenericDistribution(py::module &);

void define_StatisticalTestingTools(py::module &);

void define_GenericTriangle(py::module &);

void define_SimpleTriangle(py::module &);

void define_GenericIndexedCloud(py::module &);

void define_GenericProgressCallback(py::module &);

void define_GeometricalAnalysisTools(py::module &);

void define_GenericMesh(py::module &);

void define_SimpleMesh(py::module &cccorelib);

void define_GenericIndexedMesh(py::module &);

void define_WeibullDistribution(py::module &);

void define_ScalarField(py::module &cccorelib);

void define_ScalarFieldTools(py::module &cccorelib);

void define_RegistrationTools(py::module &cccorelib);

void define_ReferenceCloud(py::module &cccorelib);

void define_Polyline(py::module &cccorelib);

void define_PointProjectionTools(py::module &cccorelib);

void define_GenericIndexedCloudPersist(py::module &cccorelib);

void define_PointCloud(py::module &cccorelib);

void define_GenericOctree(py::module &cccorelib);

void define_KdTree(py::module &cccorelib);

void define_LocalModel(py::module &cccorelib);

void define_ManualSegmentationTools(py::module &);

void define_CCMiscTools(py::module &);

void define_NormalDistribution(py::module &);

void define_Delaunay2dMesh(py::module &);

void define_ErrorFunction(py::module &);

void define_FastMarching(py::module &);

void define_DistanceComputationTools(py::module &);

/* example of return a numpy float, in case we ever need this
py::object return_npy_float()
{

    // #include <numpy/ndarrayobject.h> get path from numpy.get_include()
    if (![]() { import_array() }()) // should be done in module init
    {
        throw std::runtime_error("init failed");
    }

    float a{1.1};
    PyArray_Descr *descr = PyArray_DescrFromType(NPY_FLOAT32);

    PyObject *ret1 = PyArray_Scalar(&a, descr, nullptr);
    return py::object(py::handle(ret1), false);
}
*/

class NumpyCloud : public CCCoreLib::GenericIndexedCloud
{
  public:
    explicit NumpyCloud(const py::array &array)
    {
        py::detail::PyArrayDescr_Proxy *descr =
            py::detail::array_descriptor_proxy(py::detail::array_proxy(array.ptr())->descr);

        py::list names = py::cast<py::list>(descr->names);
        if (names.size() >= 3)
        {
            m_xs = array[names[0]].cast<py::array_t<PointCoordinateType>>();
            m_ys = array[names[1]].cast<py::array_t<PointCoordinateType>>();
            m_zs = array[names[2]].cast<py::array_t<PointCoordinateType>>();
        }
        else
        {
            m_xs = array[py::make_tuple(py::ellipsis(), 0)].cast<py::array_t<PointCoordinateType>>();
            m_ys = array[py::make_tuple(py::ellipsis(), 1)].cast<py::array_t<PointCoordinateType>>();
            m_zs = array[py::make_tuple(py::ellipsis(), 2)].cast<py::array_t<PointCoordinateType>>();
        }
        // py::dict fields = py::cast<py::dict>(descr->fields);
        // py::print(fields);
    }

    const CCVector3 *getPoint(unsigned int index) const override
    {
        m_pts.x = m_xs.at(index);
        m_pts.y = m_ys.at(index);
        m_pts.z = m_zs.at(index);
        return &m_pts;
    }
    void getPoint(unsigned int index, CCVector3 &P) const override
    {
        P.x = m_xs.at(index);
        P.y = m_ys.at(index);
        P.z = m_zs.at(index);
    }
    unsigned int size() const override
    {
        assert(m_xs.size() <= std::numeric_limits<unsigned int>::max());
        assert(m_xs.size() >= std::numeric_limits<unsigned int>::min());
        return static_cast<unsigned int>(m_xs.size());
    }

    void forEach(genericPointAction action) override
    {
        auto xs = m_xs.mutable_unchecked<1>();
        auto ys = m_xs.mutable_unchecked<1>();
        auto zs = m_xs.mutable_unchecked<1>();

        CCVector3 p;
        for (size_t i{0}; i < size(); ++i)
        {
            p.x = xs(i);
            p.y = ys(i);
            p.z = zs(i);

            ScalarType todoScalar = 0;
            action(p, todoScalar);
        };
    }
    void getBoundingBox(CCVector3 &bbMin, CCVector3 &bbMax) override
    {
        const auto updateMinMax = [&bbMin, &bbMax](const CCVector3 &point, ScalarType _s)
        {
            bbMin.x = std::min(point.x, bbMin.x);
            bbMin.y = std::min(point.y, bbMin.y);
            bbMin.z = std::min(point.z, bbMin.z);
            bbMax.x = std::max(point.x, bbMax.x);
            bbMax.y = std::max(point.y, bbMax.y);
            bbMax.z = std::max(point.z, bbMax.z);
        };

        forEach(updateMinMax);
    }
    void placeIteratorAtBeginning() override {}
    const CCVector3 *getNextPoint() override
    {
        return &m_pts;
    }
    bool enableScalarField() override
    {
        return false;
    }
    bool isScalarFieldEnabled() const override
    {
        return false;
    }
    void setPointScalarValue(unsigned int pointIndex, ScalarType value) override {}
    ScalarType getPointScalarValue(unsigned int pointIndex) const override
    {
        return 0;
    }

  private:
    mutable CCVector3 m_pts{};
    py::array_t<PointCoordinateType> m_xs;
    py::array_t<PointCoordinateType> m_ys;
    py::array_t<PointCoordinateType> m_zs;
};

void define_cccorelib(py::module &m)
{
    define_CCShareable(m);

    define_CCGeom(m);
    define_CCMath(m);
    define_CCConst(m);
    define_BoundingBox(m);
    py::bind_vector<std::vector<CCVector2>>(m, "CCVector2List");

    define_ScalarField(m);

    define_GenericCloud(m);
    define_GenericIndexedCloud(m);
    define_GenericIndexedCloudPersist(m);
    define_PointCloud(m);

    define_GenericTriangle(m);
    define_GenericMesh(m);
    define_GenericIndexedMesh(m);
    define_Delaunay2dMesh(m);
    define_ReferenceCloud(m);
    define_Polyline(m);
    py::bind_vector<CCCoreLib::ReferenceCloudContainer>(m, "ReferenceCloudContainer");

    define_GenericDistribution(m);
    define_NormalDistribution(m);
    define_WeibullDistribution(m);

    define_GenericProgressCallback(m);

    define_GenericOctree(m);
    define_DgmOctree(m);
    define_DgmOctreeReferenceCloud(m);

    define_SimpleMesh(m);
    define_SimpleTriangle(m);

    define_ErrorFunction(m);
    define_FastMarching(m);

    define_CCMiscTools(m);
    define_AutoSegmentationTools(m);
    define_DistanceComputationTools(m);
    define_ManualSegmentationTools(m);
    define_ScalarFieldTools(m);
    define_ChamferDistanceTransform(m);
    define_CloudSamplingTools(m);
    define_PointProjectionTools(m);
    define_GeometricalAnalysisTools(m);
    define_StatisticalTestingTools(m);
    define_RegistrationTools(m);

    define_KdTree(m);
    define_TrueKdTree(m);

    define_LocalModel(m);

    py::class_<NumpyCloud, CCCoreLib::GenericIndexedCloud, CCCoreLib::GenericCloud>(m, "NumpyCloud")
        .def(py::init<py::array>());
    //        .def("forEach", &NumpyCloud::forEach)
    //        .def("getBoundingBox", &NumpyCloud::getBoundingBox);

    // Conjugate Gradient
    // Delaunay2dMesh
    // DistanceComputationTools
    // Error function
    // FastMarching
    // FastMarchingFor...
    // RayAndBox
    // SaitoSquared...
    // SquareMatrix
}

#ifdef USE_EMBEDDED_MODULES
#include <pybind11/embed.h>
PYBIND11_EMBEDDED_MODULE(cccorelib, m)
{
    define_cccorelib(m);
}
#else
PYBIND11_MODULE(cccorelib, m)
{
    define_cccorelib(m);
}
#endif
