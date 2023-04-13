import pycc
import cccorelib

CC = pycc.GetInstance()
pc_to_crop = CC.getSelectedEntities()[0]
bbMin, bbMax = cccorelib.CCVector3(), cccorelib.CCVector3()
pc_to_crop.getBoundingBox(bbMin, bbMax)
print(f"Min {bbMin}, Max: {bbMax}")
diag = bbMax - bbMin

bbMin = bbMin + cccorelib.CCVector3(diag.x / 2, 0, 0)

vertices = pycc.ccPointCloud()
vertices.setName("polyline.vertices")
vertices.addPoint(bbMin)
vertices.addPoint(bbMin + cccorelib.CCVector3(0, diag.y / 2, 0))
vertices.addPoint(bbMin + cccorelib.CCVector3(diag.x / 2, diag.y / 2, 0))
vertices.addPoint(bbMin + cccorelib.CCVector3(diag.x / 2, 0, 0))

polyline = pycc.ccPolyline(vertices)
polyline.setClosed(True)
# This is important, otherwise the polyline would have a size of 0
polyline.addPointIndex(0, vertices.size())

# To see the crop area
# CC.addToDB(vertices)

cropped_ref = pc_to_crop.crop2D(polyline, 2, True)
if cropped_ref is None:
    raise RuntimeError("Failed to crop")

if cropped_ref.size() != 0:
    cropped = pc_to_crop.partialClone(cropped_ref)
    CC.addToDB(cropped)
else:
    print("No points fall in crop area")
