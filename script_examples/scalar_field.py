"""Example that shows how to add a scalar field
as well as how to make it properly be displayed in the UI
"""
import pycc
import numpy as np

xs = np.arange(0.0, 5.0, 0.1, pycc.PointCoordinateType)
ys = np.ones(len(xs), pycc.PointCoordinateType) * 18.5
zs = np.ones(len(xs), pycc.PointCoordinateType) * 17.33

pc = pycc.ccPointCloud(xs, ys, zs)

# Create and add a new scalar field
idx = pc.addScalarField("Intensity")
scalar_field = pc.getScalarField(idx)

# Change the values
sfArray = pc.getScalarField(idx).asArray()
num_chunk = 4
chunk_size = len(xs) // num_chunk
for i in range(num_chunk):
    sfArray[i*chunk_size:(i+1)*chunk_size] = i

# Make it so the scalar field is displayed
scalar_field.computeMinAndMax()
pc.setCurrentDisplayedScalarField(idx)
pc.showSF(True)

# Optional: choose the color scale
scale = pycc.ccColorScalesManager.GetDefaultScale(
    pycc.ccColorScalesManager.YELLOW_BROWN
)
scalar_field.setColorScale(scale)

pc.setName("Scalar Field Example")
# Change the point size so that the cloud is more
# visible by default (only needed here as the point cloud
# is very small)
pc.setPointSize(7)

pycc.GetInstance().addToDB(pc)