import math
import sys
import numpy as np

import cccorelib
import pycc


def main():
    CC = pycc.GetCmdLineInstance()

    assert CC is not None
    cloudDescs = CC.clouds()
    assert len(cloudDescs) == 1

    cloud = cloudDescs[0].pc

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
    assert np.isclose(cloud.getPointScalarValue(17), 42.1337)

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
