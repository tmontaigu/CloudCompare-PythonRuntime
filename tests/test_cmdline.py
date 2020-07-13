import os
import subprocess

abspath = os.path.dirname(os.path.abspath(__file__))


def assert_command_runs(*cmd):
    completed_proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if not completed_proc.returncode == 0:
        print(completed_proc.stdout.decode())
        assert False, "CloudCompare did not execute successfully"


def test_cloud_sampling(cloudcompare_exe):
    assert_command_runs(
        cloudcompare_exe, "-SILENT", "-O", f"{abspath}/data/a_cloud.bin", "-PYTHON_SCRIPT",
        f"{abspath}/scripts/cloud_sampling_tools.py")


def test_argv(cloudcompare_exe):
    assert_command_runs(
        cloudcompare_exe, "-SILENT", "-PYTHON_SCRIPT", os.path.join(abspath, "scripts", "cmdline_args.py"), "--city",
        "dalaran")


def test_auto_segmentation(cloudcompare_exe):
    assert_command_runs(cloudcompare_exe, "-SILENT", "-O", f"{abspath}/data/a_cloud.bin", "-PYTHON_SCRIPT",
                        f"{abspath}/scripts/auto_segmentation_tools.py")


def test_normal_distribution(cloudcompare_exe):
    assert_command_runs(
        cloudcompare_exe, "-SILENT", "-O", f"{abspath}/data/a_cloud.bin", "-PYTHON_SCRIPT",
        os.path.join(abspath, "scripts", "normal_distribution.py"))
