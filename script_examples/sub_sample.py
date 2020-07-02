import pycc
import numpy as np

CC = pycc.GetInstance()

def main():
    entities = CC.getSelectedEntities()
    print(entities)

    if not entities:
        raise RuntimeError("No entities selected")
        return


    pc = entities[0]
    print(pc, pc.size())

    refcloud = pycc.subsampleCloudRandomly(pc, pc.size() // 2);
    randomPc = pc.partialClone(refcloud, CC)
    randomPc.setName("Randomly subsampled")

    refcloud = pycc.subsampleCloudWithOctree(pc, pc.size() // 4, pycc.CloudSamplingTools.RANDOM_POINT)
    randomPc = pc.partialClone(refcloud, CC)
    randomPc.setName("Subsampled using octree (RANDOM_POINT)")

    refcloud = pycc.subsampleCloudWithOctree(pc, pc.size() // 4, pycc.CloudSamplingTools.NEAREST_POINT_TO_CELL_CENTER)
    randomPc = pc.partialClone(refcloud, CC)
    randomPc.setName("Subsampled using octree (NEAREST_POINT_TO_CELL_CENTER)")

    CC.updateUI()
  


if __name__ == '__main__':
    main()
