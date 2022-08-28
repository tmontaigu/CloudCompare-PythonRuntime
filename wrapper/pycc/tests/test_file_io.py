import pycc
import os
from pathlib import Path

abspath = os.path.dirname(os.path.abspath(__file__))


def test_loading_a_bin_file():
    p = pycc.FileIOFilter.LoadParameters()
    p.alwaysDisplayLoadDialog = False

    filepath = Path(abspath).parent.parent.parent / "tests" / "data" / "a_cloud.bin"

    f = pycc.FileIOFilter.LoadFromFile(str(filepath), p)
    cloud = f.getChild(0)

    assert cloud.size() == 10_683
