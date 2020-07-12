import subprocess
import os


abspath = os.path.dirname(os.path.abspath(__file__))


def test_random_subsampling(cloudcompare_exe):
    completed_proc = subprocess.run(
        [cloudcompare_exe, "-SILENT", "-O", f"{abspath}/data/a_cloud.bin", "-PYTHON_SCRIPT", f"{abspath}/scripts/cloud_sampling_tools.py"])
    assert completed_proc.returncode == 0
