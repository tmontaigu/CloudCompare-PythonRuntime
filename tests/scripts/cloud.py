import pycc
import cccorelib
import sys
import math


def main():
    CC = pycc.GetCmdLineInstance()

    assert CC is not None
    cloudDescs = CC.clouds()
    print(cloudDescs)
    assert len(cloudDescs) == 1

    cloud = cloudDescs[0].pc

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


if __name__ == '__main__':
    main()
