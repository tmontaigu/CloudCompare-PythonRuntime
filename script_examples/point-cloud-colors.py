"""
An example on how to create a point cloud and add colors
And how to get colors of a point clouds
"""
import pycc
import numpy as np

CC = pycc.GetInstance()

points = np.array([
	[0, 0, 0],
	[1, 1, 0],
	[2, 2, 0]
])
cloud = pycc.ccPointCloud(points[:, 0], points[:, 1], points[:, 2])
cloud.setName("Colored Cloud 1")
cloud.setPointSize(9) # So that the point are visible

# This point cloud does not yet have colors:
assert cloud.hasColors() == False
assert cloud.colors() is None

red = pycc.Rgb(255, 0, 0)
green = pycc.Rgb(0, 255, 0)
blue = pycc.Rgb(0, 0, 255)

# This needs to be called after points have been added
cloud.resizeTheRGBTable()

cloud.setPointColor(0, red)
cloud.setPointColor(1, green)
cloud.setPointColor(2, blue)
# So that the colors are show by default
# without having to select in the UI
cloud.showColors(True)

print(f"First point color: {cloud.getPointColor(0)}")
color = cloud.getPointColor(1)
print(f"Second point r: {color.r}, g: {color.g}, b: {color.b}, a: {color.a}")

# Get the colors as a numpy array
colors = cloud.colors()
print(colors)
# This array is a view so modifications made on it will reflect on
# the point cloud
colors[0][:] = [255, 0, 255, 255]

CC.addToDB(cloud)

# Another way is to use the setColors function
points = np.array([
	[3, 3, 0],
	[4, 4, 0],
	[5, 5, 0]
])
cloud = pycc.ccPointCloud(points[:, 0], points[:, 1], points[:, 2])
cloud.setName("Colored Cloud 2")
cloud.setPointSize(9) # So that the point are visible
pointColors = np.array([[255, 0, 0], [0, 255, 0], [0, 0, 255]])
cloud.setColors(pointColors)
cloud.showColors(True)
# We can then also get colors as an array
colors = cloud.colors()
colors[0][:] = [255, 0, 255, 255]

CC.addToDB(cloud)
