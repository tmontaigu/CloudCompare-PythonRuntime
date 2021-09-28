import pycc
import laspy
import numpy as np

# This example shows how to create a ccPointCloud from your custom data
# here, we will load data from a las file using laspy.

# Note that the resulting ccPointCloud's point values and scalar field values
# will be copies.

path_to_las = r"L:\R1_F_0+000_0+050.las"

las = laspy.read(path_to_las)

# Be aware that CloudCompare stores coordinates on 32 bit floats.
# To avoid losing too much precision you should 'shift' your coordinates
# if they are 64 bit floats (which is the default in python land)

xs = (las.x - las.header.x_min).astype(pycc.PointCoordinateType)
ys = (las.y - las.header.y_min).astype(pycc.PointCoordinateType)
zs = (las.z - las.header.z_min).astype(pycc.PointCoordinateType)

point_cloud = pycc.ccPointCloud(xs, ys, zs)
# Add the global shift to CloudCompare so that it can use it,
# for example to display the real coordinates in point picking tool
point_cloud.setGlobalShift(-las.header.x_min, -las.header.y_min, -las.header.z_min)
point_cloud.setName(path_to_las)
print(point_cloud.size())

assert np.all(xs == point_cloud.points()[..., 0])

# Adding scalar field & copying values the manual way
idx = point_cloud.addScalarField("classification")

classification_array = point_cloud.getScalarField(idx).asArray()
classification_array[:] = las.classification[:]
print(classification_array)

# Or give the values directly
idx = point_cloud.addScalarField("intensity", las.intensity)
intensity_array = point_cloud.getScalarField(idx).asArray()
print(intensity_array[:])



try:
    cc = pycc.GetInstance()
except AttributeError:
    exit(0)
# We are in "embedded mode", add the point cloud to the DB
# but first we should compute min and max for the scalarfields
# so that their color ranges displays properly
point_cloud.getScalarField(point_cloud.getScalarFieldIndexByName("intensity")).computeMinAndMax()
point_cloud.getScalarField(point_cloud.getScalarFieldIndexByName("classification")).computeMinAndMax()

cc.addToDB(point_cloud)
cc.updateUI()
