import cccorelib
import pycc
import pytest
import numpy as np
import os
from pathlib import Path

abspath = os.path.dirname(os.path.abspath(__file__))


def test_cc_point_cloud_from_sequences():
    xs = np.array([1.0, 2.0, 3.0, 4.0, 5.0], pycc.PointCoordinateType)
    ys = np.ones(5, pycc.PointCoordinateType) * 18.5
    zs = np.ones(5, pycc.PointCoordinateType) * 17.33

    pc = pycc.ccPointCloud(xs, ys, zs)
    assert pc.size() == 5

    assert np.all(pc.points()[..., 0] == xs)
    assert np.all(pc.points()[..., 1] == ys)
    assert np.all(pc.points()[..., 2] == zs)

    # Test that we actually made copies
    xs[:] = 17.0
    assert np.all(pc.points()[..., 0] != xs)

    idx = pc.addScalarField("Intensity")

    sfArray = pc.getScalarField(idx).toArray()
    sfArray[:] = np.ones(pc.size()) * 2
    pc.getScalarField(idx)[:] = sfArray
    assert np.all(pc.getScalarField(idx).toArray() == 2)

    # test our helper function
    pc.addScalarField("something", np.array([1.0, 2.0, 3.0, 4.0, 5.0], pycc.ScalarType))

    # cannot add a scalar field with same name
    with pytest.raises(RuntimeError):
        pc.addScalarField("something", np.array([1.0, 2.0, 3.0, 4.0, 5.0], pycc.ScalarType))

    with pytest.raises(ValueError):
        pc.addScalarField("wrong_size", np.array([1.0], pycc.ScalarType))


def test_add_remove_scalar_field():
    pc = pycc.ccPointCloud("po")

    assert pc.getNumberOfScalarFields() == 0

    pc.addScalarField("SomeName")
    assert pc.getNumberOfScalarFields() == 1
    assert pc.getScalarFieldIndexByName("SomeName") == 0

    pc.addScalarField("OtherName")
    assert pc.getNumberOfScalarFields() == 2
    assert pc.getScalarFieldIndexByName("SomeName") == 0
    assert pc.getScalarFieldIndexByName("OtherName") == 1

    pc.deleteScalarField(0)
    assert pc.getNumberOfScalarFields() == 1
    assert pc.getScalarFieldIndexByName("SomeName") == -1
    assert pc.getScalarFieldIndexByName("OtherName") == 0


def test_len():
    pc = pycc.ccPointCloud()

    pc.resize(10)

    assert pc.size() == 10
    assert len(pc) == pc.size()


def test_metadata():
    obj = pycc.ccPointCloud()

    assert obj.hasMetaData("int value") is False
    obj.setMetaData("int value", 3)
    assert obj.hasMetaData("int value") is True
    assert obj.getMetaData("int value") == 3

    obj.removeMetaData("int value")
    assert obj.hasMetaData("int value") is False

    obj.setMetaData("string value", "ayaya")
    assert obj.getMetaData("string value") == "ayaya"

    obj.setMetaData("double value", 4536.135)
    assert obj.getMetaData("double value") == 4536.135


def test_colors_of_loaded_file():
    p = pycc.FileIOFilter.LoadParameters()
    p.alwaysDisplayLoadDialog = False

    filepath = Path(abspath).parent.parent.parent / "tests" / "data" / "cloud_with_tls_sensor.bin"

    f = pycc.FileIOFilter.LoadFromFile(str(filepath), p)
    cloud = f.getChild(0)

    assert (len(cloud.colors()) == cloud.size())
    assert (np.all(cloud.colors()[0] == [43, 48, 52, 255]))


def test_new_cloud_with_colors():
    points = np.array([
        [0, 0, 0],
        [1, 1, 0],
        [2, 2, 0]
    ])
    cloud = pycc.ccPointCloud(points[:, 0], points[:, 1], points[:, 2])
    # This point cloud does not yet have colors:
    assert cloud.hasColors() == False
    assert cloud.colors() is None


    red = pycc.Rgb(255, 0, 0)
    green = pycc.Rgb(0, 255, 0)
    blue = pycc.Rgb(0, 0, 255)

    with pytest.raises(RuntimeError):
        # resizeTheRGB table must have been called before
        cloud.setPointColor(0, red)

    cloud.resizeTheRGBTable()

    cloud.setPointColor(0, red)
    cloud.setPointColor(1, green)
    cloud.setPointColor(2, blue)

    with pytest.raises(IndexError):
        cloud.setPointColor(4, red)

    assert cloud.getPointColor(0) == pycc.Rgba(red, 255)
    assert cloud.getPointColor(1) == pycc.Rgba(green, 255)
    assert cloud.getPointColor(2) == pycc.Rgba(blue, 255)

    with pytest.raises(IndexError):
        cloud.getPointColor(4)

    colors = cloud.colors()
    assert np.all(colors[0] == [255, 0, 0, 255])
    assert np.all(colors[1] == [0, 255, 0, 255])
    assert np.all(colors[2] == [0, 0, 255, 255])

    colors[0][:] = [255, 0, 255, 255]
    colors = cloud.colors()
    assert np.all(colors[0] == [255, 0, 255, 255])


def test_new_cloud_with_colors_2():
    # Another way is to use the setColors function
    points = np.array([
        [3, 3, 0],
        [4, 4, 0],
        [5, 5, 0]
    ])
    cloud = pycc.ccPointCloud(points[:, 0], points[:, 1], points[:, 2])

    # This point cloud does not yet have colors:
    assert cloud.hasColors() == False
    assert cloud.colors() is None

    pointColors = np.array([[255, 0, 0], [0, 255, 0], [0, 0, 255]])
    cloud.setColors(pointColors)

    colors = cloud.colors()
    assert np.all(colors[:, :3] == pointColors)
    colors[0][:] = [255, 0, 255, 255]

    colors = cloud.colors()
    assert np.all(colors[0] == [255, 0, 255, 255])


def test_new_cloud_with_normals():
    """ Test using setPointNormal"""
    points = np.array([
        [0, 0, 0],
        [1, 1, 0],
        [2, 2, 0]
    ])
    cloud = pycc.ccPointCloud(points[:, 0], points[:, 1], points[:, 2])
    # This point cloud does not yet have colors:
    assert cloud.hasNormals() == False
    assert cloud.normals() is None

    norm1 = cccorelib.CCVector3(1, 1, 1)
    norm2 = cccorelib.CCVector3(2, 2, 2)
    norm3 = cccorelib.CCVector3(3, 3, 3)

    norm1.normalize()
    norm2.normalize()
    norm3.normalize()

    with pytest.raises(RuntimeError):
        # resizeTheNormsTable table must have been called before
        cloud.setPointNormal(0, norm1)

    cloud.resizeTheNormsTable()

    cloud.setPointNormal(0, norm1)
    cloud.setPointNormal(1, norm2)
    cloud.setPointNormal(2, norm3)

    with pytest.raises(IndexError):
        cloud.setPointNormal(4, norm1)

    assert np.allclose(list(cloud.getPointNormal(0)), list(norm1))
    assert np.allclose(list(cloud.getPointNormal(1)), list(norm2))
    assert np.allclose(list(cloud.getPointNormal(2)), list(norm3))

    with pytest.raises(IndexError):
        cloud.getPointNormal(4)

    normals = cloud.normals()
    assert np.allclose(normals[0], list(norm1))
    assert np.allclose(normals[1], list(norm2))
    assert np.allclose(normals[2], list(norm3))

    normals[0][:] = [0, 0, 0]
    # normals is a copy, so changing it did not reflect of the point cloud
    normals2 = cloud.normals()
    assert np.allclose(normals2[0], list(norm1))
    assert np.allclose(normals2[1], list(norm2))
    assert np.allclose(normals2[2], list(norm3))


def test_new_cloud_with_normals_2():
    """ Test using setNormals first
    """
    points = np.array([
        [0, 0, 0],
        [1, 1, 0],
        [2, 2, 0]
    ])
    cloud = pycc.ccPointCloud(points[:, 0], points[:, 1], points[:, 2])
    # This point cloud does not yet have colors:
    assert cloud.hasNormals() == False
    assert cloud.normals() is None

    norm1 = cccorelib.CCVector3(1, 1, 1)
    norm2 = cccorelib.CCVector3(2, 2, 2)
    norm3 = cccorelib.CCVector3(3, 3, 3)

    norm1.normalize()
    norm2.normalize()
    norm3.normalize()

    normals = np.array([list(norm1), list(norm2), list(norm3)])
    cloud.setNormals(normals)

    normals = cloud.normals()
    assert np.allclose(normals[0], list(norm1))
    assert np.allclose(normals[1], list(norm2))
    assert np.allclose(normals[2], list(norm3))

    with pytest.raises(IndexError):
        cloud.setPointNormal(4, norm1)

    assert np.allclose(list(cloud.getPointNormal(0)), list(norm1))
    assert np.allclose(list(cloud.getPointNormal(1)), list(norm2))
    assert np.allclose(list(cloud.getPointNormal(2)), list(norm3))