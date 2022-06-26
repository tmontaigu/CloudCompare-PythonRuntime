import pycc
import pytest
import numpy as np


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
