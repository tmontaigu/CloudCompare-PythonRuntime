"""
An example of how to create a mesh or query a mesh
"""
import cccorelib
import pycc
import numpy as np

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


def main():
    # Creating mesh
    vertices = pycc.ccPointCloud(VERTICES[:, 0], VERTICES[:, 1], VERTICES[:, 2])
    mesh = pycc.ccMesh(vertices)
    for (i1, i2, i3) in INDICES:
        mesh.addTriangle(i1, i2, i3)

    assert vertices.size() == len(VERTICES)
    assert mesh.size() == len(INDICES)
    assert vertices == mesh.getAssociatedCloud()

    # Query triangles
    x, y, z = cccorelib.CCVector3(), cccorelib.CCVector3(), cccorelib.CCVector3()
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

        # or
        mesh.getTriangleVertices(i, x, y, z)

    CC = pycc.GetInstance()
    CC.addToDB(mesh)
    CC.updateUI()

if __name__ == '__main__':
    main()
