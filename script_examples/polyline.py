import pycc
import cccorelib
import numpy as np

CC = pycc.GetInstance()
VERTICES = np.array([
    [-0.5, -0.5, 0],
    [1, 1, 0],
[2, 2, 0]
])
vertices = pycc.ccPointCloud(VERTICES[:, 0], VERTICES[:, 1], VERTICES[:, 2])
polyline = pycc.ccPolyline(vertices)
polyline.setColor(pycc.Rgb(255, 0, 0)) # set the color to red
polyline.showColors(True)
polyline.setClosed(False)


# This is important, otherwise the polyline would have a size of 0
polyline.addPointIndex(0, 3)


CC.addToDB(polyline)
CC.updateUI()
polyline.getDisplay().display3DLabel("Hello, world", cccorelib.CCVector3(1, 1,0))

