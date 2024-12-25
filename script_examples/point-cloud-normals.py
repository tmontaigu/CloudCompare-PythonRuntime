"""
An example of creating a ploint cloud and adding normals, and getting
normals stored in it
"""

import pycc
import cccorelib
import numpy as np

points = np.array([
    [0, 0, 0],
    [1, 1, 0],
    [2, 2, 0]
])
cloud = pycc.ccPointCloud(points[:, 0], points[:, 1], points[:, 2])
# This point cloud does not yet have colors:
assert cloud.hasNormals() == False
assert cloud.normals() is None

norm1 = cccorelib.CCVector3(1, 1, 1)
norm2 = cccorelib.CCVector3(2, 2, 2)
norm3 = cccorelib.CCVector3(3, 3, 3)
print("Some vectors:", norm1, norm2, norm3)

# Explicitely normalize (if not done, setPointNormal will)
norm1.normalize()
norm2.normalize()
norm3.normalize()
print("Normalized normals", norm1, norm2, norm3)

cloud.resizeTheNormsTable()

cloud.setPointNormal(0, norm1)
cloud.setPointNormal(1, norm2)
cloud.setPointNormal(2, norm3)

print("Normals using getPointNormal:", 
    cloud.getPointNormal(0),
    cloud.getPointNormal(1),
    cloud.getPointNormal(2)
)

normals = cloud.normals()
print("Normals as array:", normals)
 
normals[0][:] = [0, 0, 0]
# normals is a copy, so changing it did not reflect of the point cloud
print("Normals:", cloud.normals())

# Another way is to use setNormals
points = np.array([
    [0, 0, 0],
    [1, 1, 0],
    [2, 2, 0]
])
cloud = pycc.ccPointCloud(points[:, 0], points[:, 1], points[:, 2])
# This point cloud does not yet have colors:
assert cloud.hasNormals() == False
assert cloud.normals() is None
normals = np.array([
    [0.57735026, 0.57735026, 0.57735026],
    [0.57735026, 0.57735026, 0.57735026],
    [0.57735026, 0.57735026, 0.57735026],
])
# Note that this takes care of calling resizeTheNormsTable
cloud.setNormals(normals)
print(cloud.normals())