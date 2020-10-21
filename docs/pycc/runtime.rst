pycc Runtime
============

.. autofunction:: pycc.GetInstance

    This function returns an object that allows to interact with the running CC instance
    to get access to already loaded PointCloud and Meshes.

    .. warning::

        on command line mode, the object returned will be of type :class:`pycc.ccCommandLineInterface`.

        on gui mode, the object returned will be of type :class:`pycc.ccPythonInstance`


.. autofunction:: pycc.GetCmdLineInstance

    Returns the :class:`pycc.ccCommandLineInterface` object if the script is ran from
    the command line mode, otherwise returns `None`.

.. autofunction:: pycc.GetGUIInstance

    Returns the :class:`pycc.ccPythonInstance` object if the script is ran from
    the command line mode, otherwise returns `None`.


.. autoclass:: pycc.ccPythonInstance
    :members:
    :undoc-members:

.. autoclass:: pycc.ccCommandLineInterface
    :members:
    :undoc-members:
