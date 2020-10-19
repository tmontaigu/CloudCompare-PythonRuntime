import cccorelib
import pycc

CC = pycc.GetInstance()


def doSubSampling(pc):
    refcloud = cccorelib.CloudSamplingTools.subsampleCloudRandomly(pc, pc.size() // 2);
    randomPc = pc.partialClone(refcloud, CC)
    randomPc.setName("Randomly subsampled")

    refcloud = cccorelib.CloudSamplingTools.subsampleCloudWithOctree(pc, pc.size() // 4,
                                                                     cccorelib.CloudSamplingTools.RANDOM_POINT)
    randomPc = pc.partialClone(refcloud, CC)
    randomPc.setName("Subsampled using octree (RANDOM_POINT)")

    refcloud = cccorelib.CloudSamplingTools.subsampleCloudWithOctree(pc, pc.size() // 4,
                                                                     cccorelib.CloudSamplingTools.NEAREST_POINT_TO_CELL_CENTER)
    randomPc = pc.partialClone(refcloud, CC)
    randomPc.setName("Subsampled using octree (NEAREST_POINT_TO_CELL_CENTER)")


def main():
    entities = CC.getSelectedEntities()
    print(entities)

    if not entities:
        raise RuntimeError("No entities selected")

    pointCloud = entities[0]
    print(pointCloud)

    pycc.RunInThread(doSubSampling, pointCloud)

    CC.updateUI()


if __name__ == '__main__':
    main()
