import pycc

CC = pycc.GetInstance()

pc = CC.getSelectedEntities()[0]

bbmin, bbmax = pycc.Vector3(), pycc.Vector3()
pc.getBoundingBox(bbmin, bbmax)
print(pc, bbmin, bbmax)

