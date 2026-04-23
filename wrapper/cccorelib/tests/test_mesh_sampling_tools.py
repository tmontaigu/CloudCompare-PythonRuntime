from cccorelib import (
    CCVector3,
    MeshSamplingTools,
    PointCloud,
    ScalarField,
    SimpleMesh,
)

import math
import pytest


def _make_vertices():
    """Corner tetrahedron vertices: origin + axis unit vectors."""
    verts = PointCloud()
    verts.reserve(4)
    verts.addPoint(CCVector3(0, 0, 0))
    verts.addPoint(CCVector3(1, 0, 0))
    verts.addPoint(CCVector3(0, 1, 0))
    verts.addPoint(CCVector3(0, 0, 1))
    return verts


def _closed_tetrahedron():
    """Unit-corner tetrahedron; windings chosen so every normal points outward."""
    verts = _make_vertices()
    mesh = SimpleMesh(verts, False)
    mesh.reserve(4)
    mesh.addTriangle(0, 2, 1)  # z=0 face
    mesh.addTriangle(0, 1, 3)  # y=0 face
    mesh.addTriangle(0, 3, 2)  # x=0 face
    mesh.addTriangle(1, 2, 3)  # slanted face
    return verts, mesh


def _open_tetrahedron():
    """Same as above but missing the slanted face — not a closed surface."""
    verts = _make_vertices()
    mesh = SimpleMesh(verts, False)
    mesh.reserve(3)
    mesh.addTriangle(0, 2, 1)
    mesh.addTriangle(0, 1, 3)
    mesh.addTriangle(0, 3, 2)
    return verts, mesh


def test_compute_mesh_area():
    _, mesh = _closed_tetrahedron()
    area = MeshSamplingTools.computeMeshArea(mesh)
    # 3 right-triangles with legs of length 1 (area 0.5 each) + 1 equilateral
    # face with edge sqrt(2) (area sqrt(3)/2).
    expected = 3 * 0.5 + math.sqrt(3) / 2
    assert area == pytest.approx(expected, rel=1e-5)


def test_compute_mesh_volume():
    _, mesh = _closed_tetrahedron()
    volume = MeshSamplingTools.computeMeshVolume(mesh)
    # Tetrahedron bounded by x+y+z <= 1 in the positive octant has volume 1/6.
    assert abs(volume) == pytest.approx(1.0 / 6.0, rel=1e-5)


def test_edges_connectivity_closed():
    _, mesh = _closed_tetrahedron()
    stats = MeshSamplingTools.computeMeshEdgesConnectivity(mesh)
    assert stats.edgesCount == 6
    assert stats.edgesNotShared == 0
    assert stats.edgesSharedByTwo == 6
    assert stats.edgesSharedByMore == 0


def test_edges_connectivity_open():
    _, mesh = _open_tetrahedron()
    stats = MeshSamplingTools.computeMeshEdgesConnectivity(mesh)
    assert stats.edgesCount == 6
    # 3 edges around vertex 0 are shared by two faces, the 3 "rim" edges are not.
    assert stats.edgesSharedByTwo == 3
    assert stats.edgesNotShared == 3
    assert stats.edgesSharedByMore == 0


def test_flag_mesh_vertices_closed():
    _, mesh = _closed_tetrahedron()
    flags = ScalarField("flags")
    flags.resize(4)
    stats = MeshSamplingTools.flagMeshVerticesByType(mesh, flags)
    # Closed manifold → no borders, no non-manifold.
    assert stats.edgesNotShared == 0
    assert stats.edgesSharedByMore == 0
    # All four vertices should be flagged VERTEX_NORMAL (value 0).
    for i in range(4):
        assert flags.getValue(i) == pytest.approx(MeshSamplingTools.VERTEX_NORMAL)


def test_sample_points_by_density():
    _, mesh = _closed_tetrahedron()
    cloud, tri_indices = MeshSamplingTools.samplePointsOnMesh(mesh, samplingDensity=100.0)
    assert isinstance(cloud, PointCloud)
    assert cloud.size() > 0
    assert len(tri_indices) == cloud.size()
    assert all(0 <= ti < 4 for ti in tri_indices)


def test_sample_points_by_count():
    _, mesh = _closed_tetrahedron()
    cloud, tri_indices = MeshSamplingTools.samplePointsOnMesh(mesh, numberOfPoints=200)
    assert isinstance(cloud, PointCloud)
    # The requested count is approximate; give it some slack.
    assert 50 <= cloud.size() <= 500
    assert len(tri_indices) == cloud.size()


def test_vertex_flag_enum_values():
    assert MeshSamplingTools.VERTEX_NORMAL.value == 0
    assert MeshSamplingTools.VERTEX_BORDER.value == 1
    assert MeshSamplingTools.VERTEX_NON_MANIFOLD.value == 2
