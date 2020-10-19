import cccorelib

cloud = cccorelib.PointCloud()
assert cloud.size() == 0

cloud.reserve(2)

p1 = cccorelib.CCVector3(1.0, 1.1, 1.2)
p2 = cccorelib.CCVector3(2.0, 2.1, 2.2)

cloud.addPoint(p1)
cloud.addPoint(p2)

p = cloud.getPoint(0)
assert p1.x == p.x
assert p1.y == p.y
assert p1.z == p.z

try:
    p = cloud.getPoint(2)
except IndexError:
    assert True
else:
    assert False
