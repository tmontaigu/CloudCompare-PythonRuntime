import cccorelib
import pycc

import numpy as np

CC = pycc.GetInstance()


def main():
    if not CC.haveSelection():
        entities = CC.dbRootObject()
        pc = entities.getChild(0).getChild(0);
    else:
        pc = CC.getSelectedEntities()[0]

    print(pc, pc.size())


    print(pc.getNumberOfScalarFields())

    # Create (if it does not exists already)
    # a scalar field where we store the Labels
    labelsSfIdx = pc.getScalarFieldIndexByName("Labels")
    if labelsSfIdx == -1:
        labelsSfIdx = pc.addScalarField("Labels")
    pc.setCurrentScalarField(labelsSfIdx)


    numLabels = pycc.RunInThread(
        cccorelib.AutoSegmentationTools.labelConnectedComponents, pc, 11, False)
    print("There are {} labels".format(numLabels))

    # If we didn't create the Labels scalar field
    # we would do here:
    # labelsSfIdx = pc.getScalarFieldIndexByName("Default");

    labelsSf = pc.getScalarField(labelsSfIdx)
    pointLabels = labelsSf.asArray()
    pointsArray = pc.points()

    # Just to show access to x, y, z
    # print(f"x: {pointsArray[:, 0]}")
    # print(f"y: {pointsArray[:, 1]}")
    # print(f"z: {pointsArray[:, 2]}")

    allLabels = np.unique(pointLabels)

    for label in allLabels:
        pointsOfLabel = pointsArray[pointLabels == label]
        print(f"Label {label}: {len(pointsOfLabel)} points")


    referenceClouds = cccorelib.AutoSegmentationTools.ReferenceCloudContainer()
    if not cccorelib.extractConnectedComponents(pc, referenceClouds):
        print("Failed to extract the connected Components")
    assert(len(referenceClouds) == numLabels)


if __name__ == '__main__':
    main()
