# CloudCompare Python Plugin

Early step attempt at allowing to use Python to automate some stuff in CloudCompare.

# Dependencies

 - Python (ofc)
 - pybind11

# Building

Clone this project, in CloudCompare/plugins/private

In the cmake Invocation use:
```shell script
-DPLUGIN_PYTHON=ON
-DPYTHON_INCLUDE_DIR=$(python-c"from distutils.sysconfig import get_python_inc; print(get_python_inc())")
-DPYTHON_LIBRARY=$(python-c"import distutils.sysconfig as sysconfig; print(sysconfig.get_config_var('LIBDIR'))")
```

It is very likely that you get an error like so when trying to run a script:
```
Fatal Python error: initfsencoding: unable to load the file system codec
ModuleNotFoundError: No module named 'encodings'
```

The current solution is to set the PYTHONPATH and PYTHONHOME env vars.
https://stackoverflow.com/questions/5694706/py-initialize-fails-unable-to-load-the-file-system-codec

using and anaconda env
```powershell
conda activate pyccenv
$env:pythonhome = "$env:CONDA_PREFIX" # (eg C:\Users\Yoshi\Miniconda3\envs\pyccenv)
$env:pythonpath += ";$(Join-Path $env:pythonhome DLLs)"
$env:pythonpath += ";$(Join-Path $env:pythonhome  Lib\site-packages)"

# Currently, the PYTHONPATH must also be modified to have the path to the "PythonPlugin\wrapper" folder in the build dir
# to be able to load the pycc python extension
# eg:
$env:pythonpath += ";C:\Users\Thomas\CMakeBuilds\CloudCompare-Release\build\plugins\private\PythonPlugin\wrapper" 
```

# Running

Right now the only action the Plugin proposes is to run a python script named `script.py`.
Right now the path is hard coded , and so the script can only be found in the current working dir of when CloudCompare
was launched.


## notes
editor icon base on https://www.flaticon.com/free-icon/blank-page_18530
python icon from https://python.org