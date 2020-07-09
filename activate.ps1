#$env:CONDA_PREFIX = "C:\ProgramData\Miniconda3\envs\pyccenv";

$env:pythonhome = "$env:CONDA_PREFIX"
$env:pythonpath += ";$( Join-Path $env:pythonhome DLLs )"
$env:pythonpath += ";$( Join-Path $env:pythonhome  Lib\site-packages )"
$env:pythonpath += ";C:\Users\t.montaigu\Projects\CloudCompare\build\release\plugins\private\CloudCompare-PythonPlugin\wrapper"


