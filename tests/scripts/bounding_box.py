from cccorelib import BoundingBox, CCVector3

boxA = BoundingBox(CCVector3(-1, -1, -1), CCVector3(0, 0, 0))
boxB = BoundingBox(CCVector3(0, 0, 0), CCVector3(1, 1, 1))

box2 = BoundingBox(CCVector3(0, 0, 0), CCVector3(2, 2, 2))

box3 = boxA + boxB

assert box3.minCorner().x == -1.0
assert box3.minCorner().y == -1.0
assert box3.minCorner().z == -1.0

assert box3.maxCorner().x == 1.0
assert box3.maxCorner().y == 1.0
assert box3.maxCorner().z == 1.0

center = box3.getCenter()
assert center.x == 0
assert center.y == 0
assert center.z == 0

nullBox = BoundingBox()
nullBox += boxB
assert nullBox.maxCorner().x == 1.0
assert nullBox.maxCorner().y == 1.0
assert nullBox.maxCorner().z == 1.0

nullBox -= CCVector3(0.5, 0.5, 0.5)
assert nullBox.maxCorner().x == 0.5
assert nullBox.maxCorner().y == 0.5
assert nullBox.maxCorner().z == 0.5

nullBox *= 4
assert nullBox.maxCorner().x == 2.0
assert nullBox.maxCorner().y == 2.0
assert nullBox.maxCorner().z == 2.0

nullBox.clear()
assert nullBox.minCorner().x == 0.0
assert nullBox.minCorner().y == 0.0
assert nullBox.minCorner().z == 0.0

assert nullBox.maxCorner().x == 0.0
assert nullBox.maxCorner().y == 0.0
assert nullBox.maxCorner().z == 0.0
