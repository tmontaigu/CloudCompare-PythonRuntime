import pytest
import pycc
from cccorelib import CCVector3d
import numpy as np

def test_constructors():
    forward_vector = CCVector3d(0, 0, -1)
    up_vector = CCVector3d(0, 1, 0)
    rotation_matrix = pycc.ccGLMatrixd.FromViewDirAndUpDir(forward_vector, up_vector)
    rotation_matrix2 = pycc.ccGLMatrixd(forward=forward_vector, up=up_vector)
    assert np.all(rotation_matrix.asArray() == rotation_matrix2.asArray())

    view_mat = pycc.ccGLMatrixd()
    view_mat.initFromParameters(
        phi_rad=0.1,
        theta_rad=0.2,
        psi_rad=0.3,
        t3D=CCVector3d(0,0,0)
    )
