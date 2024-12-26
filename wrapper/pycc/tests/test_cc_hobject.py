import pycc

def test_cc_hobject_child_deletion():
    parent = pycc.ccPointCloud("parent")
    child = pycc.ccPointCloud("child")

    # DP_PARENT_OF_OTHER is the default, we make it explicit for the test
    parent.addChild(child, pycc.DEPENDENCY_FLAGS.DP_PARENT_OF_OTHER)
    assert parent.getChildrenNumber() == 1
    del child
    # DP_PARENT_OF_OTHER makes the parent take ownership
    # so deleting the child does nothing
    assert parent.getChildrenNumber() == 1


def test_cc_hobject_parent_deletion():
    cloud = pycc.ccPointCloud("Yoshi")
    folder = pycc.ccHObject("Folder")

    folder.addChild(cloud)

    del cloud
    assert folder.getChild(0).size() == 0