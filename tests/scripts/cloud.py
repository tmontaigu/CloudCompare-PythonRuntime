import math
import sys
import numpy as np

import cccorelib
import pycc


def test_foreach_get_bounding_box(cloud):
    bbMin = cccorelib.CCVector3(sys.float_info.max, sys.float_info.max, sys.float_info.max)

    def updatebbMin(point, _scalarValue):
        bbMin.x = min(point.x, bbMin.x)
        bbMin.y = min(point.y, bbMin.y)
        bbMin.z = min(point.z, bbMin.z)

    cloud.forEach(updatebbMin)

    actualMin = cccorelib.CCVector3()
    actualMax = cccorelib.CCVector3()
    cloud.getBoundingBox(actualMin, actualMax)

    assert math.isclose(actualMin.x, bbMin.x)
    assert math.isclose(actualMin.y, bbMin.y)
    assert math.isclose(actualMin.z, bbMin.z)


def main():
    CC = pycc.GetCmdLineInstance()

    assert CC is not None
    cloudDescs = CC.clouds()
    assert len(cloudDescs) == 1

    cloud = cloudDescs[0].pc

    test_foreach_get_bounding_box(cloud)

    assert cloud.size() == 10683
    point0 = cloud.getNextPoint()
    assert cloud.testVisibility(point0) == cccorelib.POINT_VISIBLE

    point1 = cloud.getNextPoint()
    assert point0 != point1
    cloud.placeIteratorAtBeginning()
    assert point0 == cloud.getNextPoint()

    assert cloud.enableScalarField() is True
    assert cloud.isScalarFieldEnabled() is True
    cloud.setCurrentScalarField(cloud.getScalarFieldIndexByName("Intensity"))

    cloud.setPointScalarValue(17, 42.1337)
    print(cloud.getPointScalarValue(17))
    assert cloud.getPointScalarValue(17) == float(np.float32(42.1337))

    p = cloud.getNextPoint()
    assert p.x == point1.x
    assert p.y == point1.y
    assert p.z == point1.z

    p2 = cccorelib.CCVector3()
    cloud.getPoint(1, p2)
    assert p2.x == point1.x
    assert p2.y == point1.y
    assert p2.z == point1.z


if __name__ == '__main__':
    main()
