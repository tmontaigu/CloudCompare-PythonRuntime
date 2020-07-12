import cccorelib
import pycc


def main():
    CC = pycc.GetCmdLineInstance()

    assert CC is not None
    cloudDescs = CC.clouds()
    print(cloudDescs)
    assert len(cloudDescs) == 1

    cloud = cloudDescs[0].pc
    _numLabels = cccorelib.AutoSegmentationTools.labelConnectedComponents(cloud, level=1)

    referenceClouds = cccorelib.ReferenceCloudContainer()
    success = cccorelib.AutoSegmentationTools.extractConnectedComponents(cloud, referenceClouds)
    assert success


if __name__ == '__main__':
    main()
