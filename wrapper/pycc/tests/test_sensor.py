import pycc
import pytest
import numpy as np
from pathlib import Path
import os

abspath = os.path.dirname(os.path.abspath(__file__))


def test_camera_sensor_intrinsic_parameters():
    params = pycc.ccCameraSensor.IntrinsicParameters()

    params.vertFocal_pix = 5.0
    assert params.vertFocal_pix == 5.0

    params.pixelSize_mm = (1.0, 2.0)
    assert params.pixelSize_mm == (1.0, 2.0)

    params.skew = 5.0
    assert params.skew == 5.0

    params.vFOV_rad = 5.0
    assert params.vFOV_rad == 5.0

    params.zNear_mm = 5.0
    assert params.zNear_mm == 5.0

    params.zFar_mm = 5.0
    assert params.zFar_mm == 5.0

    params.arrayWidth = 12
    assert params.arrayWidth == 12

    params.arrayHeight = 13
    assert params.arrayHeight == 13


def find_child_by_name(parent, child_name):
    for i in range(parent.getChildrenNumber()):
        child = parent.getChild(i)
        if child.getName() == child_name:
            return child
    return None


def test_file_with_tls_sensor():
    p = pycc.FileIOFilter.LoadParameters()
    p.alwaysDisplayLoadDialog = False

    filepath = Path(abspath).parent.parent.parent / "tests" / "data" / "cloud_with_tls_sensor.bin"

    f = pycc.FileIOFilter.LoadFromFile(str(filepath), p)
    cloud = f.getChild(0)

    gbl_sensor = find_child_by_name(cloud, "TLS/GBL")
    if gbl_sensor is None:
        raise RuntimeError("no gbl sensor found")

    transform = pycc.ccIndexedTransformation()
    i = gbl_sensor.getActiveIndex()
    succeeded = gbl_sensor.getAbsoluteTransformation(transform, i)
    if not succeeded:
        raise RuntimeError("Failed to get absolute transformation")

    expected_matrix = np.array([
        [-0.709079, -0.705129, 0.000000, 813.169739],
        [0.705129, -0.709079, 0.000000, 599.101624],
        [0.000000, 0.000000, 1.000000, 29.616304],
        [0.000000, 0.000000, 0.000000, 1.000000],
    ])

    assert np.allclose(expected_matrix, transform.asArray().transpose())
