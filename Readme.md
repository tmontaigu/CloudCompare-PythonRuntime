# CloudCompare Python Plugin

Early step attempt at allowing to use Python to automate some stuff in CloudCompare.

# Dependencies

 - Python
 - pybind11

# Building & Installing

## Windows

1) Clone this project in CloudCompare/plugins/private


2) Create a Virtual env or a conda env 

   (conda environment works, virtual envs may not as they were not as tested)

    ```shell script
    # Python's venv
    python -m venv --system-site-packages .\pyccvenv 
   
    # Conda environment
    conda create -n pyccenv
    ```

3) Activate the environment

    ```shell script
    # Python's venv (PowerShell)
    cd pyccenv\Scripts\Activate.ps1
    
    # Conda environment
    conda active pyccenv
    ```
   
4) Install dependencies

    ```shell script
   pip install -r requirements-release.txt
    ```
   
5) Start cmake or your IDE from within the environment

    The option to build the plugin is 
    ```shell script
    -DPLUGIN_PYTHON=ON
    ```

   Or, if you created a conda environment, you can add
    `-DCONDA_PREFIX=C:\path\to\pyccenv` to the cmake invocation


    In the cmake Invocation you may also have to use:
    ```shell script
    -DPYTHON_INCLUDE_DIR=$(python-c"from distutils.sysconfig import get_python_inc; print(get_python_inc())")
    -DPYTHON_LIBRARY=$(python-c"import distutils.sysconfig as sysconfig; print(sysconfig.get_config_var('LIBDIR'))")
    ```
5) Build & Install

    During the installation step, the virtual environment will be copied to the installation folder
    so launching CC from the install folder should just work.
    
    Use `-DPLUGIN_PYTHON_COPY_ENV=OFF` after the plugin has been installed once to gain time.

## Linux
1) Clone this project in CloudCompare/plugins/private
2) Install additional dependencies

    Ubuntu: `libpython3-dev ` and `pybind11-dev` (only available since 20.04)

    Fedora: `python3-devel` and  `pybind11-devel`


2) Run cmake with `-DPLUGIN_PYTHON=ON`
3) Build
4) Install

# Running

The plugin exposes a Python REPL and a mini code editor to run Python Code.

The Api available from Python is still limited (especially for creating / deleting entities) and
tools available from the gui are not (yet?) callable from python scripts. 


# Running tests

Tests works by running CloudCompare in command line mode.
To run them, you need pytest, and you need to provide to pytest the path the where CloudCompare is installed

```shell
# in CloudCompare/plugins/private/CloudCompare-PythonPlugin:

pytest --cloudcompare_exe "C:\CMakeBuilds\CloudCompare\Release\install\CloudCompare\CloudCompare.exe" tests
```

# Building the documentation

Building the documentation also works by running CloudCompare in command line mode.
To build the documentation:

``` powershell
cd docs
.\make-docs.ps1 "C:\CMakeBuilds\CloudCompare\Release\install\CloudCompare\CloudCompare.exe"
```

Which by default will build the docs in HTML in docs/_build. Open docs/_build/index.html to view it.
(The output dir can be changed using the `-OutputDir` option)


## notes
editor icon base on https://www.flaticon.com/free-icon/blank-page_18530

python icon from https://python.org

Parts of icons taken from [Flaticon.com](www.flaticon.com): 
https://www.flaticon.com/free-icon/pencil_2919592?term=edit&page=1&position=16&page=1&position=16&related_id=2919592&origin=search
https://www.flaticon.com/free-icon/py_180867?related_id=180867
https://www.flaticon.com/free-icon/play_915999

