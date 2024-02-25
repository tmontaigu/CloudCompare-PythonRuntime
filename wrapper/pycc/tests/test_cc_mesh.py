import cccorelib
import numpy as np
import pycc
import pytest

# Vertices of the cube
VERTICES = np.array([
    [-0.5, -0.5, -0.5],
    [-0.5, -0.5, 0.5],
    [-0.5, 0.5, -0.5],
    [-0.5, 0.5, 0.5],
    [0.5, -0.5, -0.5],
    [0.5, -0.5, 0.5],
    [0.5, 0.5, -0.5],
    [0.5, 0.5, 0.5]
])

# Indices of the cube
INDICES = np.array([
    [0, 1, 3],
    [0, 3, 2],
    [0, 2, 4],
    [2, 6, 4],
    [0, 4, 1],
    [1, 4, 5],
    [2, 3, 6],
    [3, 7, 6],
    [4, 6, 5],
    [5, 6, 7],
    [1, 5, 7],
    [1, 7, 3]
])

def assert_ccvector_are_equal(a: cccorelib.CCVector3, b: cccorelib.CCVector3):
    assert a.x == b.x
    assert a.y == b.y
    assert a.z == b.z

def expected_points_of_triangle(triangle_index):
    x = cccorelib.CCVector3(
        VERTICES[INDICES[triangle_index, 0], 0],
        VERTICES[INDICES[triangle_index, 0], 1],
        VERTICES[INDICES[triangle_index, 0], 2]
    )
    y = cccorelib.CCVector3(
        VERTICES[INDICES[triangle_index, 1], 0],
        VERTICES[INDICES[triangle_index, 1], 1],
        VERTICES[INDICES[triangle_index, 1], 2]
    )
    z = cccorelib.CCVector3(
        VERTICES[INDICES[triangle_index, 2], 0],
        VERTICES[INDICES[triangle_index, 2], 1],
        VERTICES[INDICES[triangle_index, 2], 2]
    )
    return x, y, z


def build_cube_mesh():
    vertices = pycc.ccPointCloud(VERTICES[:, 0], VERTICES[:, 1], VERTICES[:, 2])
    mesh = pycc.ccMesh(vertices)
    for (i1, i2, i3) in INDICES:
        mesh.addTriangle(i1, i2, i3)

    return mesh, vertices


def test_building_mesh():
    mesh, vertices = build_cube_mesh()
    assert vertices.size() == len(VERTICES)
    assert mesh.size() == len(INDICES)
    assert vertices == mesh.getAssociatedCloud()


def test_mesh_lifetime_1():
    mesh, vertices = build_cube_mesh()
    assert vertices == mesh.getAssociatedCloud()
    del mesh
    # If mesh is deleted, vertices should still be alive
    assert vertices.size() == len(VERTICES)


def test_mesh_lifetime_2():
    mesh, vertices = build_cube_mesh()
    assert vertices == mesh.getAssociatedCloud()
    del vertices
    # If vertices is deleted, then actually the mesh still have access to it
    # i.e. the mesh keeps it alive.
    # In CPP API, there is a HOjbect dependency between the two, and so when
    # the vertices are deleted, the mesh is notified of it and empties himself, so it is actually safe
    # to delete the vertices, but as lifetime is not manual in Python, it's better to keep it alive
    assert mesh.size() == len(INDICES)
    assert mesh.getAssociatedCloud().size() == len(VERTICES)


def test_getTriangleVertIndexes():
    x, y, z = cccorelib.CCVector3(), cccorelib.CCVector3(), cccorelib.CCVector3()
    mesh, vertices = build_cube_mesh()

    for i in range(mesh.size()):
        vert_indices = mesh.getTriangleVertIndexes(i)
        assert vert_indices.i1 == INDICES[i, 0]
        assert vert_indices.i2 == INDICES[i, 1]
        assert vert_indices.i3 == INDICES[i, 2]

        for j in range(3):
            assert vert_indices[j] == INDICES[i, j]

        vertices.getPoint(vert_indices.i1, x)
        vertices.getPoint(vert_indices.i2, y)
        vertices.getPoint(vert_indices.i3, z)
        ex, ey, ez = expected_points_of_triangle(i)
        assert_ccvector_are_equal(x, ex)
        assert_ccvector_are_equal(y, ey)
        assert_ccvector_are_equal(z, ez)

    with pytest.raises(IndexError):
        mesh.getTriangleVertIndexes(mesh.size())

    with pytest.raises(IndexError):
        mesh.getTriangleVertIndexes(mesh.size() + 1)


def test_getTriangleVertices():
    x, y, z = cccorelib.CCVector3(), cccorelib.CCVector3(), cccorelib.CCVector3()
    mesh, _ = build_cube_mesh()

    for i in range(mesh.size()):
        mesh.getTriangleVertices(i, x, y, z)
        ex, ey, ez = expected_points_of_triangle(i)
        assert_ccvector_are_equal(x, ex)
        assert_ccvector_are_equal(y, ey)
        assert_ccvector_are_equal(z, ez)

    with pytest.raises(IndexError):
        mesh.getTriangleVertices(mesh.size(), x, y, z)

    with pytest.raises(IndexError):
        mesh.getTriangleVertices(mesh.size() + 1, x, y, z)
