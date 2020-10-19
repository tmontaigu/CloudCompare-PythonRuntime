Getting Started
===============


This plugin allows to write Python scripts to automate some tasks in CloudCompare

.. caution::
    This plugin is in its very early stage, so a lot of things are missings
    some things may not work, bugs are most probably common, etc


This plugin allows to run scripts in the 2 _CloudCompares_ 'modes': GUI & CommandLine.

GUI
---

In GUI, there are 2 actions:

*  The REPL (Read Print Eval Loop) which allows for quick testing of some actions.
    .. image:: ../images/repl-icon.png

*  The editor which allows to create,edit,save python script and most importantly run them.
    .. image:: ../images/python-editor-icon.png

In a your Python script you will probably want to access Objects (PointCloud, Meshes) already loaded
into the app and use them.

The typical python script will need to import Python modules that provide access to
functions and data-structures of cloudcompare:

.. code:: Python

    # cccorelib contains definitions of tools, such as Cloud2Cloud distance,
    # sampling algorithm, etc
    import cccorelib

    # pycc contains definitions of data structures and other things,
    # the most important being getting the current running CC instance
    # to retreive point cloud/meshes
    import pycc

    # The CC object is the interface that will allow you to navigate
    # the tree of files currently loaded, the list of files selected, etc
    CC = pycc.GetInstance()


Command Line
____________

To run in command line mode pass following option: `-PYTHON_SCRIPT`

.. code:: shell

    cloudcompare -PYTHON_SCRIPT my_script.py


The plugin will take all arguments that follows the script name and populate
python's sys.argv with them:

Example:

.. code:: shell

    /path/to/cloudcompare -PYTHON_SCRIPT my_script.py some_positional_arg --size 50

Python's sys.argv will be equal to:
['/path/to/cloudcompare', 'some_positional_arg', '--size', '50']
