import os
import subprocess
import platform

abspath = os.path.dirname(os.path.abspath(__file__))


def assert_command_runs(*cmd):
    if platform.system() == 'Darwin':
        cmd = ['open', '-a'] + [cmd[0]] + ["--args"] + list(cmd[1:])
    completed_proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if completed_proc.returncode == 125:
         pytest.skip("test skipped")
    elif completed_proc.returncode != 0:
        print(f"CloudCompare exited with return code: {completed_proc.returncode}")
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


def test_normal_kdtree(cloudcompare_exe):
    assert_command_runs(
        cloudcompare_exe, "-SILENT", "-O", f"{abspath}/data/a_cloud.bin", "-PYTHON_SCRIPT",
        os.path.join(abspath, "scripts", "kdtree.py"))


def test_normal_kdtree(cloudcompare_exe):
    assert_command_runs(
        cloudcompare_exe, "-SILENT", "-O", f"{abspath}/data/a_cloud.bin", "-PYTHON_SCRIPT",
        os.path.join(abspath, "scripts", "true_kdtree.py"))


def test_cloud_for_each(cloudcompare_exe):
    assert_command_runs(
        cloudcompare_exe, "-SILENT", "-O", f"{abspath}/data/a_cloud.bin", "-PYTHON_SCRIPT",
        os.path.join(abspath, "scripts", "cloud.py"))


def test_cloud_creation(cloudcompare_exe):
    assert_command_runs(
        cloudcompare_exe, "-SILENT", "-O", f"{abspath}/data/a_cloud.bin", "-PYTHON_SCRIPT",
        os.path.join(abspath, "scripts", "point_cloud.py"))


def test_scalarfield_tools(cloudcompare_exe):
    assert_command_runs(
        cloudcompare_exe, "-SILENT", "-O", f"{abspath}/data/a_cloud.bin", "-PYTHON_SCRIPT",
        os.path.join(abspath, "scripts", "scalarfield.py"))


def test_cchobjects_lifetimes(cloudcompare_exe):
    assert_command_runs(
        cloudcompare_exe, "-SILENT", "-PYTHON_SCRIPT", os.path.join(abspath, "scripts", "ccHObject_keep_alive.py"))


def test_pycc_tests(cloudcompare_exe):
    assert_command_runs(
        cloudcompare_exe, "-SILENT", "-PYTHON_SCRIPT", os.path.join(abspath, "pytest_runner.py"))


def test_can_call_m3c2_plugin(cloudcompare_exe):
    assert_command_runs(
        cloudcompare_exe, "-SILENT", "-O", f"{abspath}/data/a_cloud.bin", "-PYTHON_SCRIPT",
        os.path.join(abspath, "scripts", "m3c2_plugin.py"))
