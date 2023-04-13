import pycc

def test_cc_hobject_child_deletion():
    """Test that deleting the child removed it from the parent container"""
    parent = pycc.ccPointCloud("parent")
    child = pycc.ccPointCloud("child")

    # DP_PARENT_OF_OTHER is the default, we make it explicit for the test
    parent.addChild(child, pycc.DEPENDENCY_FLAGS.DP_PARENT_OF_OTHER)
    assert parent.getChildrenNumber() == 1
    del child
    assert parent.getChildrenNumber() == 0

def test_cc_hobject_parent_deletion():
    """Test that deleting the a parent keep the child alive"""
    parent = pycc.ccPointCloud("parent")
    child = pycc.ccPointCloud("child")

    # DP_PARENT_OF_OTHER is the default, we make it explicit for the test
    parent.addChild(child, pycc.DEPENDENCY_FLAGS.DP_PARENT_OF_OTHER)
    del parent
    assert child is not None
