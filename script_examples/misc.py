import pycc
import cccorelib

CC = pycc.GetInstance()

pc = CC.getSelectedEntities()[0]

bbmin, bbmax = cccorelib.CCVector3(), cccorelib.CCVector3()
pc.getBoundingBox(bbmin, bbmax)
print(pc, bbmin, bbmax)

