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

#include <GenericIndexedMesh.h>
#include <GenericMesh.h>
#include <GenericProgressCallback.h>
#include <MeshSamplingTools.h>
#include <PointCloud.h>
#include <ScalarField.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_MeshSamplingTools(py::module &cccorelib)
{
    using CCCoreLib::MeshSamplingTools;

    py::class_<MeshSamplingTools> PyMST(cccorelib, "MeshSamplingTools", R"doc(
Static utilities for analysing and sampling triangular meshes
(area, volume, edge-connectivity statistics, point sampling).
)doc");

    py::class_<MeshSamplingTools::EdgeConnectivityStats>(PyMST, "EdgeConnectivityStats", R"doc(
Statistics on the edge connectivity of a mesh.

An edge used by exactly one triangle signals a border/hole; an edge
shared by more than two triangles signals a non-manifold edge.
)doc")
        .def(py::init<>())
        .def_readonly("edgesCount",
                      &MeshSamplingTools::EdgeConnectivityStats::edgesCount,
                      "Total number of edges.")
        .def_readonly("edgesNotShared",
                      &MeshSamplingTools::EdgeConnectivityStats::edgesNotShared,
                      "Edges used by only one triangle (border/hole).")
        .def_readonly("edgesSharedByTwo",
                      &MeshSamplingTools::EdgeConnectivityStats::edgesSharedByTwo,
                      "Edges shared by exactly two triangles (manifold).")
        .def_readonly("edgesSharedByMore",
                      &MeshSamplingTools::EdgeConnectivityStats::edgesSharedByMore,
                      "Edges shared by more than two triangles (non-manifold).");

    py::enum_<MeshSamplingTools::VertexFlags>(PyMST, "VertexFlags")
        .value("VERTEX_NORMAL", MeshSamplingTools::VERTEX_NORMAL)
        .value("VERTEX_BORDER", MeshSamplingTools::VERTEX_BORDER)
        .value("VERTEX_NON_MANIFOLD", MeshSamplingTools::VERTEX_NON_MANIFOLD)
        .export_values();

    PyMST
        .def_static("computeMeshArea",
                    &MeshSamplingTools::computeMeshArea,
                    "mesh"_a,
                    "Returns the total surface area of the mesh.")

        .def_static("computeMeshVolume",
                    &MeshSamplingTools::computeMeshVolume,
                    "mesh"_a,
                    R"doc(
Returns the volume enclosed by the mesh.

Warning: the mesh must be closed. Use computeMeshEdgesConnectivity to
check — any non-zero ``edgesNotShared`` means the mesh has holes.
)doc")

        .def_static(
            "computeMeshEdgesConnectivity",
            [](CCCoreLib::GenericIndexedMesh *mesh)
            {
                MeshSamplingTools::EdgeConnectivityStats stats;
                if (!MeshSamplingTools::computeMeshEdgesConnectivity(mesh, stats))
                {
                    throw std::runtime_error(
                        "MeshSamplingTools.computeMeshEdgesConnectivity failed "
                        "(invalid mesh or not enough memory).");
                }
                return stats;
            },
            "mesh"_a,
            R"doc(
Returns an EdgeConnectivityStats for the mesh.

Counts how many edges are used by 1, 2 or more triangles — useful to
decide whether a mesh is closed and/or manifold before calling
computeMeshVolume.
)doc")

        .def_static(
            "flagMeshVerticesByType",
            [](CCCoreLib::GenericIndexedMesh *mesh, CCCoreLib::ScalarField *flags)
            {
                MeshSamplingTools::EdgeConnectivityStats stats;
                if (!MeshSamplingTools::flagMeshVerticesByType(mesh, flags, &stats))
                {
                    throw std::runtime_error(
                        "MeshSamplingTools.flagMeshVerticesByType failed "
                        "(invalid mesh or not enough memory).");
                }
                return stats;
            },
            "mesh"_a,
            "flags"_a,
            R"doc(
Flags each vertex of the mesh by type (see VertexFlags).

``flags`` must be an already-allocated ScalarField big enough to hold one
value per vertex. On return, values are VERTEX_NORMAL / VERTEX_BORDER /
VERTEX_NON_MANIFOLD. Returns the EdgeConnectivityStats computed along
the way.
)doc")

        .def_static(
            "samplePointsOnMesh",
            [](CCCoreLib::GenericMesh *mesh,
               double samplingDensity,
               CCCoreLib::GenericProgressCallback *progressCb)
            {
                std::vector<unsigned> triIndices;
                CCCoreLib::PointCloud *pc = MeshSamplingTools::samplePointsOnMesh(
                    mesh, samplingDensity, progressCb, &triIndices);
                if (!pc)
                {
                    throw std::runtime_error(
                        "MeshSamplingTools.samplePointsOnMesh (by density) failed.");
                }
                // take_ownership: CCCoreLib new's a fresh PointCloud; transfer
                // ownership to Python so it's deleted when the handle is GC'd.
                return py::make_tuple(py::cast(pc, py::return_value_policy::take_ownership),
                                      triIndices);
            },
            "mesh"_a,
            "samplingDensity"_a,
            "progressCb"_a = nullptr,
            R"doc(
Sample points on the mesh with a given surface density.

Parameters
----------
mesh : GenericMesh
samplingDensity : float
    Target number of samples per unit area.
progressCb : GenericProgressCallback, optional

Returns
-------
tuple (cloud, triIndices)
    ``cloud`` is a newly allocated PointCloud (owned by Python).
    ``triIndices`` is a list giving the source triangle index for each
    sampled point.

Raises
------
RuntimeError
    If sampling fails.
)doc")

        .def_static(
            "samplePointsOnMesh",
            [](CCCoreLib::GenericMesh *mesh,
               unsigned numberOfPoints,
               CCCoreLib::GenericProgressCallback *progressCb)
            {
                std::vector<unsigned> triIndices;
                CCCoreLib::PointCloud *pc = MeshSamplingTools::samplePointsOnMesh(
                    mesh, numberOfPoints, progressCb, &triIndices);
                if (!pc)
                {
                    throw std::runtime_error(
                        "MeshSamplingTools.samplePointsOnMesh (by count) failed.");
                }
                return py::make_tuple(py::cast(pc, py::return_value_policy::take_ownership),
                                      triIndices);
            },
            "mesh"_a,
            "numberOfPoints"_a,
            "progressCb"_a = nullptr,
            R"doc(
Sample an approximate number of points on the mesh.

Parameters
----------
mesh : GenericMesh
numberOfPoints : int
    Approximate total number of samples to draw over the whole mesh.
progressCb : GenericProgressCallback, optional

Returns
-------
tuple (cloud, triIndices)
    Same shape as the density-based overload.
)doc");
}
