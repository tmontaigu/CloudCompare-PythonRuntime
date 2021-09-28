import pycc
import cccorelib

CC = pycc.GetInstance()

pc = CC.getSelectedEntities()[0]

bbMin, bbMax = cccorelib.CCVector3(), cccorelib.CCVector3()
pc.getBoundingBox(bbMin, bbMax)
center = (bbMax - bbMin) / 2
center = cccorelib.CCVector3d(*center)

vp = pycc.ccViewportParameters()
vp.setCameraCenter(center, False)
vp.setFocalDistance(150)

glWindow = pc.getDisplay()
glWindow.setViewportParameters(vp)
glWindow.redraw()
