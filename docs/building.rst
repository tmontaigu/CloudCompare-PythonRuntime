Building from sources
=====================

There is 2 way in which you can build the project:

    1. As a CloudCompare Plugin
    2. As "standalone" Python packages

When built as CloudCompare plugin, at the end of the process,
you will have a standard CloudCompare plugin that embeds Python to
allow running Python script directly in the app and allow scripts to interact with
the currently running CloudCompare like a regular C++ plugin would.
The plugin built, also embeds various Python bindings of
CloudCompare's libraries to be used in your script.

When build as standalone packages, you will install the Python bindings
of the CloudCompare's libraries in your Python environment.

The process to build is explained in the sections below.


Building as Plugin
------------------

Supported platforms: [Windows, Linux, macOs]

In the following sections we assume that you know
how to compile CloudCompare from source, otherwise refer
to the CloudCompare documentation on how to do.


To Compile this plugin you need to have Python installed.


Windows
_______

1. Clone this project into CloudCompare/plugins/private
2. Create a virtual environment via ``venv`` or ``conda``

   .. code-block::

       # Python's venv
       python -m venv .\pyccvenv

       # Conda environment
       conda create -n pyccenv

3. Activate the environment

   .. code-block::

       # Python's venv (PowerShell)
       .\pyccenv\Scripts\Activate.ps1

       # Conda environment
       conda activate pyccenv

4. install dependencies

   .. code-block::

       pip install -r requirements-release.txt

5. Generate the cmake config

    To generate the cmake config you have 2 options:

    1. Start the cmake generation while still within the activated environment

    2. Add ``-DPTYHON_EXECUTABLE="somepath_to_python.exe"``
       Where ``somepath_to_python.exe`` is the absolute path to the python executable
       of the pyccenv created earlier.

       To get it, run: ``python -c "import sys;print(sys.executable)"`` (while in the activated env)

    In both cases the cmake option to add in order to build the plugin in ``-DPLUGIN_PYTHON=ON``


6. Build
7. Install


Linux
_____


1. Clone this project in CloudCompare/plugins/private
2. Install additional dependencies

    Ubuntu: ``libpython3-dev`` and ``pybind11-dev`` (only available since 20.04)

    Fedora: ``python3-devel`` and  ``pybind11-devel``


3. Run cmake with ``-DPLUGIN_PYTHON=ON``
4. Build
5. Install


macOs
_____

1. Clone this project in CloudCompare/plugins/private
2. Make sure you have python3 installed (and Qt)
3. Install additional dependencies

    In this example, we will use brew
    ``brew install pybind11``

4. Run cmake with ``-DPLUGIN_PYTHON=ON``
5. Build
5. Install

Building as independent wheels
-------------------------------

Supported platforms: [Windows, Linux, macOs]

It is also possible to build and install the bindings as standard python
packages to use them in python scripts (run by a standalone python executable
and not the embedded python in the plugin).

To build as standalone wheels, it is not necessary to clone the repo
in CloudCompare's `plugins/private` folder.


.. important::

    To be able to pip install from source, you need pip version >= 21.1
    Run  `pip install --upgrade pip` to get the latest version

To do so:

1. Clone this project

2. Set path to Qt (Windows/macOs)

.. code-block:: PowerShell

    # Windows (PowerShell)
    $env:CMAKE_PREFIX_PATH+=";C:\Qt\5.15.2\msvc2019_64"

.. code-block:: bash

    macOs (bash)
    export CMAKE_PREFIX_PATH=/usr/local/opt/qt@5

3. Run

.. code-block:: console

    # Building the wheels

    pip wheel --no-deps wrapper/cccorelib
    pip wheel --use-feature=in-tree-build --no-deps wrapper/pycc

    # Installing directly

    pip install wrapper/cccorelib
    pip install --use-feature=in-tree-build wrapper/pycc


Other development related things
--------------------------------


Running the tests
_________________

Tests works by running CloudCompare in command line mode.
To run them, you need pytest, and you need to provide to pytest the path the where CloudCompare is installed

.. code-block::

    # in CloudCompare/plugins/private/CloudCompare-PythonPlugin:

    pytest --cloudcompare_exe "C:\CMakeBuilds\CloudCompare\Release\install\CloudCompare\CloudCompare.exe" tests

Building the documentation
__________________________

Building the documentation also works by running CloudCompare in command line mode.
To build the documentation:

.. code-block::

    cd docs
    .\make-docs.ps1 "C:\CMakeBuilds\CloudCompare\Release\install\CloudCompare\CloudCompare.exe"

Which by default will build the docs in HTML in docs/_build. Open docs/_build/index.html to view it.
(The output dir can be changed using the `-OutputDir` option)
