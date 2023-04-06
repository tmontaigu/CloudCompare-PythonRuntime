import pycc
import os
from pathlib import Path
import pytest

try:
    from pycc.plugins import qM3C2
except ImportError:
    qM3C2 = None
else:
    # This also works:
    # from PyQt5.QtWidgets import QApplication
    # import sys
    #_app = QApplication(sys.argv)
    pycc.InitQApplication();


abspath = os.path.dirname(os.path.abspath(__file__))



@pytest.mark.skipif(qM3C2 is None, reason="Wrapper for qM3c2 not compiled")
def test_m3c2_plugin():
    p = pycc.FileIOFilter.LoadParameters()
    p.alwaysDisplayLoadDialog = False

    filepath = Path(abspath).parent.parent.parent / "tests" / "data" / "a_cloud.bin"

    f = pycc.FileIOFilter.LoadFromFile(str(filepath), p)

    cloud = f.getChild(0)

    m3c2_dialog = qM3C2.qM3C2Dialog(cloud, cloud)
    m3c2_dialog.setCorePointsCloud(cloud)

    allowsDialog = False
    _result = qM3C2.qM3C2Process.Compute(m3c2_dialog, allowsDialog)
