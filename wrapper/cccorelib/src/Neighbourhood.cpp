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

#include <array>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <CCGeom.h>
#include <GenericIndexedCloudPersist.h>
#include <GenericIndexedMesh.h>
#include <Neighbourhood.h>
#include <SquareMatrix.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_Neighbourhood(py::module &cccorelib)
{
    using CCCoreLib::Neighbourhood;

    py::class_<Neighbourhood> PyNeighbourhood(cccorelib, "Neighbourhood", R"doc(
A point subset with several local geometric tools attached
(LS-plane fitting, quadric fitting, curvature, features, triangulation).

Typically suited to handle "nearest neighbours" of a point in a cloud.
)doc");

    py::enum_<Neighbourhood::GeomElement>(PyNeighbourhood, "GeomElement")
        .value("FLAG_DEPRECATED", Neighbourhood::FLAG_DEPRECATED)
        .value("FLAG_GRAVITY_CENTER", Neighbourhood::FLAG_GRAVITY_CENTER)
        .value("FLAG_LS_PLANE", Neighbourhood::FLAG_LS_PLANE)
        .value("FLAG_QUADRIC", Neighbourhood::FLAG_QUADRIC)
        .export_values();

    py::enum_<Neighbourhood::CurvatureType>(PyNeighbourhood, "CurvatureType")
        .value("GAUSSIAN_CURV", Neighbourhood::GAUSSIAN_CURV)
        .value("MEAN_CURV", Neighbourhood::MEAN_CURV)
        .value("NORMAL_CHANGE_RATE", Neighbourhood::NORMAL_CHANGE_RATE)
        .export_values();

    py::enum_<Neighbourhood::InputVectorsUsage>(PyNeighbourhood, "InputVectorsUsage")
        .value("UseOXYasBase", Neighbourhood::UseOXYasBase)
        .value("UseYAsUpDir", Neighbourhood::UseYAsUpDir)
        .value("None", Neighbourhood::None)
        .export_values();

    py::enum_<Neighbourhood::GeomFeature>(PyNeighbourhood, "GeomFeature")
        .value("EigenValuesSum", Neighbourhood::EigenValuesSum)
        .value("Omnivariance", Neighbourhood::Omnivariance)
        .value("EigenEntropy", Neighbourhood::EigenEntropy)
        .value("Anisotropy", Neighbourhood::Anisotropy)
        .value("Planarity", Neighbourhood::Planarity)
        .value("Linearity", Neighbourhood::Linearity)
        .value("PCA1", Neighbourhood::PCA1)
        .value("PCA2", Neighbourhood::PCA2)
        .value("SurfaceVariation", Neighbourhood::SurfaceVariation)
        .value("Sphericity", Neighbourhood::Sphericity)
        .value("Verticality", Neighbourhood::Verticality)
        .value("EigenValue1", Neighbourhood::EigenValue1)
        .value("EigenValue2", Neighbourhood::EigenValue2)
        .value("EigenValue3", Neighbourhood::EigenValue3)
        .export_values();

    PyNeighbourhood.attr("IGNORE_MAX_EDGE_LENGTH") = Neighbourhood::IGNORE_MAX_EDGE_LENGTH;
    PyNeighbourhood.attr("DUPLICATE_VERTICES") = Neighbourhood::DUPLICATE_VERTICES;
    PyNeighbourhood.attr("DO_NOT_DUPLICATE_VERTICES") = Neighbourhood::DO_NOT_DUPLICATE_VERTICES;

    PyNeighbourhood
        .def(py::init<CCCoreLib::GenericIndexedCloudPersist *>(),
             "associatedCloud"_a,
             // keep_alive<1, 2>: keep the associated cloud (arg 2) alive while the
             // Neighbourhood (arg 1 = this) is alive; Neighbourhood stores a raw
             // pointer to it and doesn't extend its lifetime otherwise.
             py::keep_alive<1, 2>())
        .def("reset",
             &Neighbourhood::reset,
             "Resets the structure (deprecates all associated geometrical features).")
        .def("associatedCloud",
             &Neighbourhood::associatedCloud,
             py::return_value_policy::reference,
             "Returns the associated cloud.")

        .def("getGravityCenter",
             &Neighbourhood::getGravityCenter,
             py::return_value_policy::copy,
             "Returns the gravity center, or None if the computation failed.")

        .def("setGravityCenter",
             &Neighbourhood::setGravityCenter,
             "G"_a,
             "Sets the gravity center directly. Handle with care!")

        .def(
            "getLSPlane",
            [](Neighbourhood &self) -> py::object
            {
                const PointCoordinateType *eq = self.getLSPlane();
                if (!eq)
                {
                    return py::none();
                }
                return py::make_tuple(eq[0], eq[1], eq[2], eq[3]);
            },
            R"doc(
Returns the best interpolating plane equation (least squares).

The returned tuple has the form (a, b, c, d) such that ax + by + cz = d.
Returns None if the computation failed.
)doc")

        .def(
            "setLSPlane",
            [](Neighbourhood &self,
               const std::array<PointCoordinateType, 4> &eq,
               const CCVector3 &X,
               const CCVector3 &Y,
               const CCVector3 &N) { self.setLSPlane(eq.data(), X, Y, N); },
            "eq"_a,
            "X"_a,
            "Y"_a,
            "N"_a,
            R"doc(
Sets the best interpolating plane (least squares) directly. Handle with care!

Parameters
----------
eq : sequence of 4 floats
    Plane equation (ax + by + cz = d).
X, Y : CCVector3
    Local base X and Y vectors.
N : CCVector3
    Plane normal.
)doc")

        .def("getLSPlaneX",
             &Neighbourhood::getLSPlaneX,
             py::return_value_policy::copy,
             R"doc(
Returns the LS plane's X base vector (largest eigen direction), or None
if the computation failed.
)doc")

        .def("getLSPlaneY",
             &Neighbourhood::getLSPlaneY,
             py::return_value_policy::copy,
             R"doc(
Returns the LS plane's Y base vector (second largest eigen direction),
or None if the computation failed.
)doc")

        .def("getLSPlaneNormal",
             &Neighbourhood::getLSPlaneNormal,
             py::return_value_policy::copy,
             R"doc(
Returns the LS plane normal (smallest eigen direction), or None if the
computation failed.
)doc")

        .def(
            "getQuadric",
            [](Neighbourhood &self) -> py::object
            {
                CCCoreLib::SquareMatrix orientation;
                const PointCoordinateType *q = self.getQuadric(&orientation);
                if (!q)
                {
                    return py::none();
                }
                return py::make_tuple(py::make_tuple(q[0], q[1], q[2], q[3], q[4], q[5]), orientation);
            },
            R"doc(
Returns the best interpolating 2.5D quadric and its local frame.

The quadric fits Z = a + b*X + c*Y + d*X^2 + e*X*Y + f*Y^2 in a local
coordinate system centered on the gravity center.

Returns a tuple ((a, b, c, d, e, f), SquareMatrix) — where the matrix is
the 3x3 orientation of the local frame — or None if the fit failed.
)doc")

        .def("computeFeature",
             &Neighbourhood::computeFeature,
             "feature"_a,
             R"doc(
Computes the given geometric feature on the point set.

Most features are defined in Hackel et al., "Contour detection in
unstructured 3D point clouds" (2016). PCA1 and PCA2 are from Brodu and
Lague, "3D terrestrial lidar data classification of complex natural
scenes using a multi-scale dimensionality criterion" (2012).
)doc")

        .def("computeMomentOrder1",
             &Neighbourhood::computeMomentOrder1,
             "P"_a,
             R"doc(
Computes the 1st order moment at point P (based on the eigenvalues).

Note: the reference article states that the result should be between 0
and 1, but this is actually wrong — (a + b)^2 can be > a^2 + b^2.
)doc")

        .def("computeRoughness",
             &Neighbourhood::computeRoughness,
             "P"_a,
             "roughnessUpDir"_a = nullptr,
             R"doc(
Computes the roughness of a point (by fitting a 2D plane on its neighbors).

Parameters
----------
P : CCVector3
    Point for which to compute the roughness.
roughnessUpDir : CCVector3, optional
    Up direction to compute a signed roughness value.

Warning: P should NOT be part of the set of points.
)doc")

        .def("computeCurvature",
             &Neighbourhood::computeCurvature,
             "P"_a,
             "cType"_a,
             R"doc(
Computes the curvature at point P by fitting a 2.5D quadric.

Returns the curvature at P, or CCCoreLib's NAN_VALUE if the computation
failed. The curvature value is always unsigned.
)doc")

        .def(
            "compute3DQuadric",
            [](Neighbourhood &self)
            {
                double q[10];
                if (!self.compute3DQuadric(q))
                {
                    throw std::runtime_error("Neighbourhood.compute3DQuadric failed.");
                }
                return py::make_tuple(q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7], q[8], q[9]);
            },
            R"doc(
Fits a full 3D quadric to the point set.

Returns a 10-tuple (a, b, c, d, e, f, g, l, m, n) of coefficients for
    a*x^2 + b*y^2 + c*z^2 + 2*e*x*y + 2*f*y*z + 2*g*z*x + 2*l*x + 2*m*y + 2*n*z + d = 0

Raises
------
RuntimeError
    If the quadric fit failed.
)doc")

        .def("computeCovarianceMatrix",
             &Neighbourhood::computeCovarianceMatrix,
             "Returns the 3x3 covariance matrix (SquareMatrixd) of the point set.")

        .def("computeLargestRadius",
             &Neighbourhood::computeLargestRadius,
             "Returns the distance between the gravity center and the farthest point.")

        .def(
            "triangulateOnPlane",
            [](Neighbourhood &self, bool duplicateVertices, PointCoordinateType maxEdgeLength)
            {
                std::string err;
                CCCoreLib::GenericIndexedMesh *mesh =
                    self.triangulateOnPlane(duplicateVertices, maxEdgeLength, err);
                if (mesh)
                {
                    return py::cast(mesh, py::return_value_policy::take_ownership);
                }
                else
                {
                    throw std::runtime_error(err);
                }
            },
            "duplicateVertices"_a,
            "maxEdgeLength"_a,
            R"doc(
Applies 2D Delaunay triangulation on the best-fit LS plane.

Returns a tuple (mesh, error_message). `mesh` is a GenericIndexedMesh
(owned by Python) or None on failure; `error_message` describes the
failure when `mesh` is None, otherwise it's empty.

Parameters
----------
duplicateVertices : bool
    Whether the returned mesh should own a fresh copy of the vertices
    (use DUPLICATE_VERTICES / DO_NOT_DUPLICATE_VERTICES).
maxEdgeLength : float
    Maximum edge length — triangles with longer edges are discarded.
    Pass IGNORE_MAX_EDGE_LENGTH (0) to skip this filter.
)doc")

        .def("triangulateFromQuadric",
             &Neighbourhood::triangulateFromQuadric,
             "stepsX"_a,
             "stepsY"_a,
             // take_ownership: same rationale as triangulateOnPlane above.
             py::return_value_policy::take_ownership,
             R"doc(
Fits a 2.5D quadric and triangulates it on a stepsX-by-stepsY grid inside
the point set's bounding box.

Returns a GenericIndexedMesh (owned by Python). See getQuadric() for the
underlying fit.
)doc")

        .def(
            "projectPointsOn2DPlane",
            [](Neighbourhood &self, py::object planeEquation, Neighbourhood::InputVectorsUsage usage)
            {
                std::vector<CCVector2> points2D;

                const PointCoordinateType *eqPtr = nullptr;
                std::array<PointCoordinateType, 4> eqBuf{};
                if (!planeEquation.is_none())
                {
                    eqBuf = planeEquation.cast<std::array<PointCoordinateType, 4>>();
                    eqPtr = eqBuf.data();
                }

                CCVector3 O;
                CCVector3 X;
                CCVector3 Y;
                const bool ok = self.projectPointsOn2DPlane<CCVector2>(points2D, eqPtr, &O, &X, &Y, usage);
                if (!ok)
                {
                    throw std::runtime_error("Neighbourhood.projectPointsOn2DPlane failed");
                }
                return py::make_tuple(py::cast(points2D), O, X, Y);
            },
            "planeEquation"_a = py::none(),
            "vectorsUsage"_a = Neighbourhood::None,
            R"doc(
Projects the points of the associated cloud onto the best-fitting LS plane.

Parameters
----------
planeEquation : sequence of 4 floats, optional
    Custom plane equation (ax + by + cz = d). If None, Neighbourhood's
    default LS plane is used.
vectorsUsage : InputVectorsUsage
    Controls how the local 2D base is chosen (see InputVectorsUsage).

Returns
-------
tuple (points2D, O, X, Y)
    `points2D` is a list of CCVector2; `O`, `X`, `Y` are CCVector3
    giving the local plane origin and X/Y base vectors that were used.

Raises
------
RuntimeError
    If the cloud is empty, no plane can be fitted, or an allocation fails.
)doc");
}
