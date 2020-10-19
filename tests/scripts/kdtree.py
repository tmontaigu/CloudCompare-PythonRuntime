import cccorelib
import pycc


def main():
    CC = pycc.GetCmdLineInstance()

    assert CC is not None
    cloudDescs = CC.clouds()
    print(cloudDescs)
    assert len(cloudDescs) == 1

    cloud = cloudDescs[0].pc

    kdtree = cccorelib.KDTree()
    kdtree.buildFromCloud(cloud)

    assert cloud is kdtree.getAssociatedCloud()

    point = [-13, 17.0, 1.5]
    assert kdtree.findPointBelowDistance(point, 2.0) is True
    nearestIndex = kdtree.findNearestNeighbour(point, 2.0)
    assert nearestIndex is not None

    point = [0.0, 0.0, 0.0]
    assert kdtree.findPointBelowDistance(point, 2.0) is False
    nearestIndex = kdtree.findNearestNeighbour(point, 2.0)
    assert nearestIndex is None

    point = [-13, 17.0, 1.5]
    points = cccorelib.KDTree.IndicesVector()
    numPts = kdtree.findPointsLyingToDistance(point, distance=1, tolerance=0.5, points=points)
    assert numPts == len(points)


if __name__ == '__main__':
    main()
