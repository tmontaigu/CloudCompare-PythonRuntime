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


def test_argv(cloudcompare_exe):
    assert_command_runs(
        cloudcompare_exe, "-SILENT", "-PYTHON_SCRIPT", os.path.join(abspath, "scripts", "cmdline_args.py"), "--city",
        "dalaran")


def test_pycc_tests(cloudcompare_exe):
    assert_command_runs(
        cloudcompare_exe, "-SILENT", "-PYTHON_SCRIPT", os.path.join(abspath, "pytest_runner.py"))
