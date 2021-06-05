import pycc
import pytest
import numpy as np

def test_cc_point_cloud_from_sequences():
    xs = np.array([1.0, 2.0, 3.0, 4.0, 5.0], pycc.PointCoordinateType)
    ys =  np.ones(5, pycc.PointCoordinateType) * 18.5
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

    # cannot add the a scalar field with same name
    with pytest.raises(RuntimeError):
        pc.addScalarField("something", np.array([1.0, 2.0, 3.0, 4.0, 5.0], pycc.ScalarType))

    with pytest.raises(ValueError):
            pc.addScalarField("wrong_size", np.array([1.0], pycc.ScalarType))

def test_len_ccloud():
    pc = pycc.ccPointCloud()

    pc.resize(10)

    assert pc.size() == 10
    assert len(pc) == pc.size()
