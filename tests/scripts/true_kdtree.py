import cccorelib
import pycc


def main():
    CC = pycc.GetCmdLineInstance()

    assert CC is not None
    cloudDescs = CC.clouds()
    print(cloudDescs)
    assert len(cloudDescs) == 1

    cloud = cloudDescs[0].pc

    kdtree = cccorelib.TrueKdTree(cloud)
    kdtree.build(2.5)

    leaves = cccorelib.TrueKdTree.LeafVector()
    kdtree.getLeaves(leaves)
    assert len(leaves) != 0


if __name__ == '__main__':
    main()
