import pycc
from cccorelib import CloudSamplingTools


def main():
    CC = pycc.GetCmdLineInstance()

    assert CC is not None
    cloudDescs = CC.clouds()
    print(cloudDescs)
    assert len(cloudDescs) == 1

    cloud = cloudDescs[0].pc

    # At level
    refCloud = CloudSamplingTools.subsampleCloudWithOctreeAtLevel(
        cloud,
        1,
        CloudSamplingTools.RANDOM_POINT
    )
    assert refCloud.size() <= (cloud.size() // 2)

    # subsampleCloudWithOctree
    refCloud = CloudSamplingTools.subsampleCloudWithOctree(
        cloud,
        cloud.size() // 2,
        CloudSamplingTools.NEAREST_POINT_TO_CELL_CENTER
    )
    assert refCloud.size() <= (cloud.size() // 2)

    # Random
    refCloud = CloudSamplingTools.subsampleCloudRandomly(cloud, cloud.size() // 2)
    assert refCloud.size() == (cloud.size() // 2)


if __name__ == '__main__':
    main()
