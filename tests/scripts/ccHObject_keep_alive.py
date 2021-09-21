import pycc
import cccorelib

from pathlib import Path

CLOUD_PATH = str(Path(__file__).parent.parent / "data" / "a_cloud.bin")


def test_getChild():
    """
    Test that deleting the parent after getting the child
    still keeps the parent alive until the child is not alive
    """
    params = pycc.FileIOFilter.LoadParameters()
    parent = pycc.FileIOFilter.LoadFromFile(CLOUD_PATH, params)

    child = parent.getChild(0)
    assert child is not None
    del parent

    # Should crash if obj was delete as a result of the del parent
    _ = cccorelib.CloudSamplingTools.subsampleCloudRandomly(child, 10)


def test_getParent():
    """
    Test that deleting the parent after getting the child
    still keeps the parent alive until the child is not alive
    """
    params = pycc.FileIOFilter.LoadParameters()
    parent = pycc.FileIOFilter.LoadFromFile(CLOUD_PATH, params)

    child = parent.getChild(0)
    assert child is not None
    del parent

    parent = child.getParent()
    assert parent is not None
    del parent

    _ = cccorelib.CloudSamplingTools.subsampleCloudRandomly(child, 10)


def test_find():
    params = pycc.FileIOFilter.LoadParameters()
    parent = pycc.FileIOFilter.LoadFromFile(CLOUD_PATH, params)

    child = parent.find(parent.getFirstChild().getUniqueID())
    assert child is not None
    del parent

    _ = cccorelib.CloudSamplingTools.subsampleCloudRandomly(child, 10)


def test_getFirstChild():
    params = pycc.FileIOFilter.LoadParameters()
    parent = pycc.FileIOFilter.LoadFromFile(CLOUD_PATH, params)

    child = parent.getFirstChild()
    assert child is not None
    del parent

    _ = cccorelib.CloudSamplingTools.subsampleCloudRandomly(child, 10)


def test_getLastChild():
    params = pycc.FileIOFilter.LoadParameters()
    parent = pycc.FileIOFilter.LoadFromFile(CLOUD_PATH, params)

    child = parent.getLastChild()
    assert child is not None
    del parent

    _ = cccorelib.CloudSamplingTools.subsampleCloudRandomly(child, 10)


def test_detachChild():
    params = pycc.FileIOFilter.LoadParameters()
    parent = pycc.FileIOFilter.LoadFromFile(CLOUD_PATH, params)

    child = parent.getChild(0)
    assert child is not None

    parent.detachChild(child)
    assert parent.getChildrenNumber() == 0

    del parent
    _ = cccorelib.CloudSamplingTools.subsampleCloudRandomly(child, 10)


if __name__ == '__main__':
    test_getChild()
    test_getParent()
    test_find()
    test_getFirstChild()
    test_getLastChild()
    test_detachChild()
