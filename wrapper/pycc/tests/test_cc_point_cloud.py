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

    sfArray = pc.getScalarField(idx).asArray()
    sfArray[:] = np.ones(pc.size()) * 2
    assert np.all(sfArray == 2)

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


def test_colors():
    p = pycc.FileIOFilter.LoadParameters()
    p.alwaysDisplayLoadDialog = False

    filepath = Path(abspath).parent.parent.parent / "tests" / "data" / "cloud_with_tls_sensor.bin"

    f = pycc.FileIOFilter.LoadFromFile(str(filepath), p)
    cloud = f.getChild(0)

    assert (len(cloud.colors()) == cloud.size())
    assert (np.all(cloud.colors()[0] == [43, 48, 52, 255]))
