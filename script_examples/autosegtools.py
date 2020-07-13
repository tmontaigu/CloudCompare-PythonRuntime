import cccorelib
import pycc

CC = pycc.GetInstance()


def main():
    if not CC.haveSelection():
        entities = CC.dbRootObject()
        pc = entities.getChild(0).getChild(0);
    else:
        pc = CC.getSelectedEntities()[0]

    print(pc, pc.size())

    numLabels = pycc.RunInThread(cccorelib.AutoSegmentationTools.labelConnectedComponents, pc, 11, False)
    print("There are {} labels".format(numLabels))

    labelsSfIdx = pc.getScalarFieldIndexByName("Default");
    labelsSf = pc.getScalarField(labelsSfIdx)
    labelsSf.computeMinAndMax()
    pc.setCurrentDisplayedScalarField(labelsSfIdx)
    CC.updateUI()
    CC.redrawAll(False)

    referenceClouds = cccorelib.ReferenceCloudContainer()
    if not pycc.extractConnectedComponents(pc, referenceClouds):
        print("Failed to extract the connected Components")
    else:
        assert len(referenceClouds) == numLabels


if __name__ == '__main__':
    main()
